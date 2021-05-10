/*
 * DiffractionForceModel.cpp
 *
 *  Created on: Oct 24, 2014
 *      Author: cady
 */

#include "DiffractionForceModel.hpp"

#include "Body.hpp"
#include "DiffractionInterpolator.hpp"
#include "HDBParser.hpp"
#include "InvalidInputException.hpp"
#include "InternalErrorException.hpp"
#include "SurfaceElevationInterface.hpp"
#include "yaml.h"
#include "external_data_structures_parsers.hpp"
#include "yaml2eigen.hpp"

#include <ssc/interpolation.hpp>
#include <ssc/text_file_reader.hpp>

#include <array>
#define TWOPI 6.283185307179586232

std::string DiffractionForceModel::model_name() { return "diffraction";}

HDBParser hdb_from_file(const std::string& filename);
HDBParser hdb_from_file(const std::string& filename)
{
    return HDBParser(ssc::text_file_reader::TextFileReader(filename).get_contents());
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


class DiffractionForceModel::Impl
{
    public:

        Impl(const YamlDiffraction& data, const EnvironmentAndFrames& env, const HDBParser& hdb, const std::string& body_name)
          : initialized(false),
        H0(data.calculation_point.x,data.calculation_point.y,data.calculation_point.z),
        rao(DiffractionInterpolator(hdb,std::vector<double>(),std::vector<double>(),data.mirror))
        {
            if (env.w.use_count()>0)
            {
                const auto hdb_periods = hdb.get_diffraction_module_periods();
                if (not(hdb_periods.empty()))
                {
                    check_all_omegas_are_within_bounds(hdb_periods.front(), convert_to_periods(env.w->get_wave_angular_frequency_for_each_model()), hdb_periods.back());
                }
            }
            else
            {
                THROW(__PRETTY_FUNCTION__, InvalidInputException, "Force model '" << DiffractionForceModel::model_name << "' needs a wave model, even if it's 'no waves'");
            }
        }

        ssc::kinematics::Vector6d evaluate(const BodyStates& states, const double t, const EnvironmentAndFrames& env)
        {
            ssc::kinematics::Vector6d w = ssc::kinematics::Vector6d::Zero();
            auto T = env.k->get("NED", states.name);
            T.swap();
            const Eigen::Vector2d x = (T*ssc::kinematics::Point(states.name,H0)).v.head(2); // Position on horizontal plane of calculation point
            const double psi = states.get_angles().psi;
            if (env.w.use_count()>0)
            {
                try
                {
                    for (size_t degree_of_freedom_idx = 0 ; degree_of_freedom_idx < 6 ; ++degree_of_freedom_idx) // For each degree of freedom (X, Y, Z, K, M, N)
                    {
                        const auto directional_spectra = env.w->get_flat_directional_spectra(x(0), x(1), t);
                        for (const auto spectrum:directional_spectra) // For each directional spectrum
                        {
                            const size_t nb_of_period_incidence_pairs = spectrum.k.size();
                            for (size_t omega_beta_idx = 0 ; omega_beta_idx < nb_of_period_incidence_pairs ; ++omega_beta_idx) // For each incidence and each period (omega[i[omega_beta_idx]], beta[j[omega_beta_idx]])
                            {
                                // Wave vector, i.e. angular wave number k as a vector along the direction of propagation
                                const Eigen::Vector2d k(spectrum.k[omega_beta_idx]*spectrum.cos_psi[omega_beta_idx], spectrum.k[omega_beta_idx]*spectrum.sin_psi[omega_beta_idx]);
                                // Period
                                const double period = TWOPI/spectrum.omega.at(omega_beta_idx);
                                // Wave incidence
                                const double beta = psi - spectrum.psi.at(omega_beta_idx);
                                // Interpolate RAO module and phase for this axis, period and incidence
                                const double rao_module = rao.interpolate_module(degree_of_freedom_idx, period, beta);
                                const double rao_phase = -rao.interpolate_phase(degree_of_freedom_idx, period, beta);
                                // Evaluate force
                                const double rao_amplitude = rao_module * spectrum.a[omega_beta_idx];
                                const double omega_t = spectrum.omega.at(omega_beta_idx) * t;
                                const double k_x = k.dot(x);
                                const double theta = spectrum.phase.at(omega_beta_idx);
                                w((int)degree_of_freedom_idx) -= rao_amplitude * sin(-omega_t + k_x + theta + rao_phase);
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

        ssc::kinematics::Vector6d express_aquaplus_wrench_in_xdyn_coordinates(ssc::kinematics::Vector6d v) const
        {
            v(0) *= -1;
            v(3) *= -1;
            return v;
        }

        Eigen::Vector3d get_application_point() const
        {
            return H0;
        }

    private:
        Impl();
        bool initialized;
        Eigen::Vector3d H0;
        DiffractionInterpolator rao;
};

DiffractionForceModel::DiffractionForceModel(const YamlDiffraction& data, const std::string& body_name_, const EnvironmentAndFrames& env):
        ForceModel("diffraction", {}, body_name_, env),
        pimpl(new Impl(data, env, hdb_from_file(data.hdb_filename), body_name_))
{
}

DiffractionForceModel::DiffractionForceModel(const Input& data, const std::string& body_name_, const EnvironmentAndFrames& env, const std::string& hdb_file_contents):
        ForceModel("diffraction", {}, body_name_, env),
        pimpl(new Impl(data, env, HDBParser(hdb_file_contents), body_name_))
{
}

Wrench DiffractionForceModel::get_force(const BodyStates& states, const double t, const EnvironmentAndFrames& env, const std::map<std::string,double>& commands) const
{
    return Wrench(ssc::kinematics::Point(body_name, pimpl->get_application_point()), body_name, pimpl->evaluate(states, t, env));
}

DiffractionForceModel::Input DiffractionForceModel::parse(const std::string& yaml)
{
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    YamlDiffraction ret;
    node["hdb"]                             >> ret.hdb_filename;
    node["calculation point in body frame"] >> ret.calculation_point;
    node["mirror for 180 to 360"]           >> ret.mirror;
    return ret;
}
