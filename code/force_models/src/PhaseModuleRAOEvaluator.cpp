#include "EnvironmentAndFrames.hpp"
#include "HydroDBParser.hpp"
#include "InvalidInputException.hpp"
#include "PhaseModuleRAOEvaluator.hpp"
#include "YamlRAO.hpp"

#define TWOPI 6.283185307179586232

std::vector<std::vector<double> > convert_to_periods(std::vector<std::vector<double> > angular_frequencies);
std::vector<std::vector<double> > convert_to_periods(std::vector<std::vector<double> > angular_frequencies)
{
    for (auto& omegas:angular_frequencies)
    {
        for (auto& omega:omegas)
        {
            omega = TWOPI/omega;
        }
    }
    return angular_frequencies;
}

void check_all_omegas_are_within_bounds(const double min_bound, const std::vector<std::vector<double> >& vector_to_check, const double max_bound);
void check_all_omegas_are_within_bounds(const double min_bound, const std::vector<std::vector<double> >& vector_to_check, const double max_bound)
{
    const double eps = 0.01; // We don't care if we're above or below the bounds by 0.01 s: those are wave frequencies so not very precise.
    for (auto t:vector_to_check)
    {
        for (auto Tp:t)
        {
            if (Tp<(min_bound-eps))
            {
                THROW(__PRETTY_FUNCTION__, InvalidInputException, "HDB used by DiffractionForceModel only defines the RAO for wave period Tp within [" << min_bound << "," << max_bound << "]"
                     << " s, but wave spectrum discretization contains Tp = " << Tp
                     << " s which is below the min bound by " << min_bound-Tp << " s: you need to modify the section 'environment models/model: waves/discretization' in the YAML file or the 'spectrum' section or change the HDB file ")
                ;
            }
            if (Tp>(max_bound+eps))
            {
                THROW(__PRETTY_FUNCTION__, InvalidInputException, "HDB used by DiffractionForceModel only defines the RAO for wave period Tp within [" << min_bound << "," << max_bound << "]"
                     << " s, but wave spectrum discretization contains Tp = " << Tp
                     << " s which is above the max bound by " << Tp-max_bound << " s: you need to modify the section 'environment models/model: waves/discretization' in the YAML file or the 'spectrum' section or change the HDB file ")
                ;
            }
        }
    }
}

PhaseModuleRAOEvaluator::PhaseModuleRAOEvaluator(
    const YamlRAO& diffraction_yaml,
    const EnvironmentAndFrames& env,
    const HydroDBParser& parser,
    const std::string& body_name,
    const std::string& force_model_name):
                rao_interpolator(DiffractionInterpolator(parser, diffraction_yaml)),
                H0(rao_interpolator.get_rao_calculation_point()),
                use_encounter_period(false)
{
    if (env.w.use_count()>0)
    {
        std::vector<double> periods;
        try
        {
            periods = rao_interpolator.get_diffraction_module_periods();
        }
        catch(const ssc::exception_handling::Exception& e)
        {
            THROW(__PRETTY_FUNCTION__, ssc::exception_handling::Exception,
                    "This simulation uses the " + force_model_name + " force model which uses the frequency-domain "
                    "results of the HDB or PRECAL_R file. When querying the periods for the diffraction "
                    "forces, the following problem occurred:\n" << e.get_message());
        }
        if (not(periods.empty()))
        {
            check_all_omegas_are_within_bounds(periods.front(), convert_to_periods(env.w->get_wave_angular_frequency_for_each_model()), periods.back());
        }
    }
    else
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "Force model '"
                << force_model_name << "' needs a wave model, even if it's 'no waves'");
    }
    if (diffraction_yaml.use_encounter_period.is_initialized())
    {
        use_encounter_period = diffraction_yaml.use_encounter_period.get();
    }
}

ssc::kinematics::Vector6d PhaseModuleRAOEvaluator::evaluate(const BodyStates& states, const double t, const EnvironmentAndFrames& env)
{
    ssc::kinematics::Vector6d w = ssc::kinematics::Vector6d::Zero();
    auto T = env.k->get("NED", states.name);
    T.swap();
    const Eigen::Vector2d x = (T*ssc::kinematics::Point(states.name,H0)).v.head(2); // Position on horizontal plane of calculation point
    const double psi = states.get_angles().psi;
    const Eigen::Vector2d Vs_NED = (T.get_rot()*Eigen::Vector3d(states.u(), states.v(), states.w())).head(2); // Ship velocity in NED frame
    if (env.w.use_count()>0)
    {
        try
        {
            for (size_t degree_of_freedom_idx = 0 ; degree_of_freedom_idx < 6 ; ++degree_of_freedom_idx) // For each degree of freedom (X, Y, Z, K, M, N)
            {
                const auto directional_spectra = env.w->get_flat_directional_spectra(x(0), x(1), t);
                for (const auto spectrum:directional_spectra) // For each directional spectrum
                {
                    const size_t nb_of_period_incidence_pairs = spectrum.omega.size();
                    for (size_t omega_beta_idx = 0 ; omega_beta_idx < nb_of_period_incidence_pairs ; ++omega_beta_idx) // For each incidence and each period (omega[i[omega_beta_idx]], beta[j[omega_beta_idx]])
                    {
                        // Wave vector, i.e. angular wave number k as a vector along the direction of propagation
                        const Eigen::Vector2d k(spectrum.k[omega_beta_idx]*spectrum.cos_psi[omega_beta_idx], spectrum.k[omega_beta_idx]*spectrum.sin_psi[omega_beta_idx]);
                        // Period
                        const double period = get_interpolation_period(spectrum.omega[omega_beta_idx], Vs_NED, k);
                        if (period > 0)
                        {
                            // Wave incidence
                            const double beta = psi - spectrum.psi[omega_beta_idx];
                            // Interpolate RAO module and phase for this axis, period and incidence
                            const double rao_module = rao_interpolator.interpolate_module(degree_of_freedom_idx, period, beta);
                            const double rao_phase = -rao_interpolator.interpolate_phase(degree_of_freedom_idx, period, beta);
                            // Evaluate force
                            const double rao_amplitude = rao_module * spectrum.a[omega_beta_idx];
                            const double omega_t = spectrum.omega[omega_beta_idx] * t;
                            const double k_x = k.dot(x);
                            const double theta = spectrum.phase[omega_beta_idx];
                            w((int)degree_of_freedom_idx) -= rao_amplitude * sin(-omega_t + k_x + theta + rao_phase);
                        }
                        else
                        {
                            std::cerr << "WARNING: The encounter period Te=" << period << "s is negative. This wave component will produce no diffraction force." << std::endl;
                        }
                    }
                }
            }
        }
        catch (const ssc::exception_handling::Exception& e)
        {
            THROW(__PRETTY_FUNCTION__, ssc::exception_handling::Exception, "This simulation uses the diffraction force model which evaluates a Response Amplitude Operator using a wave model. During this evaluation, the following problem occurred:\n" << e.get_message());
        }
    }
    const auto ww = express_aquaplus_wrench_in_xdyn_coordinates(w);
    return ww;
}

double PhaseModuleRAOEvaluator::get_interpolation_period(const double wave_angular_frequency, const Eigen::Vector2d& Vs, const Eigen::Vector2d& k) const
{
    double encounter_period;
    if (use_encounter_period)
    {
        encounter_period = TWOPI/(wave_angular_frequency - Vs.dot(k));
        if (encounter_period > 0 && (encounter_period < rao_interpolator.period_bounds.first || encounter_period > rao_interpolator.period_bounds.second))
        {
            std::cerr << "WARNING: The encounter period Te=" << abs(encounter_period) << "s is outside of the range [" << rao_interpolator.period_bounds.first << "," << rao_interpolator.period_bounds.second << "]s provided in the HDB file. The response will be interpolated outside the bounds." << std::endl;
        }
    }
    else
    {
        encounter_period = TWOPI/wave_angular_frequency;
    }
    return encounter_period;
}

ssc::kinematics::Vector6d PhaseModuleRAOEvaluator::express_aquaplus_wrench_in_xdyn_coordinates(ssc::kinematics::Vector6d v) const
{
    v(0) *= -1;
    v(3) *= -1;
    return v;
}

Eigen::Vector3d PhaseModuleRAOEvaluator::get_application_point() const
{
    return H0;
}