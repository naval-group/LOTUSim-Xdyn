#include "RaoInterpolator.hpp"
#include "xdyn/exceptions/InternalErrorException.hpp"
#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

#include "HydroDBParser.hpp"
#include "xdyn/external_data_structures/YamlRAO.hpp"
#include <algorithm>


RaoInterpolator::~RaoInterpolator(){}

RaoInterpolator::RAO RaoInterpolator::RAO::for_diffraction(const HydroDBParser& parser)
{
    return RaoInterpolator::RAO(parser.get_diffraction_module_tables(),
               parser.get_diffraction_phase_tables(),
               parser.get_diffraction_module_periods(),
               parser.get_diffraction_phase_periods(),
               parser.get_diffraction_module_psis(),
               parser.get_diffraction_phase_psis());
}

RaoInterpolator::RAO RaoInterpolator::RAO::for_froude_krylov(const HydroDBParser& parser)
{
    return RaoInterpolator::RAO(parser.get_froude_krylov_module_tables(),
               parser.get_froude_krylov_phase_tables(),
               parser.get_froude_krylov_module_periods(),
               parser.get_froude_krylov_phase_periods(),
               parser.get_froude_krylov_module_psis(),
               parser.get_froude_krylov_phase_psis());
}

RaoInterpolator::RAO::RAO(const std::array<std::vector<std::vector<double> >,6 > module_tables_,
    const std::array<std::vector<std::vector<double> >,6 > phase_tables_,
    const std::vector<double> module_periods_,
    const std::vector<double> phase_periods_,
    const std::vector<double> module_incidence_,
    const std::vector<double> phase_incidence_)
    : module_tables(module_tables_)
    , phase_tables(phase_tables_)
    , module_periods(module_periods_)
    , phase_periods(phase_periods_)
    , module_incidence(module_incidence_)
    , phase_incidence(phase_incidence_)
        {}

RaoInterpolator::RAO RaoInterpolator::RAO::get(const YamlRAO::TypeOfRao& type_of_rao, const HydroDBParser& parser)
{
    switch(type_of_rao)
    {
        case YamlRAO::TypeOfRao::DIFFRACTION_RAO:
            return for_diffraction(parser);
        case YamlRAO::TypeOfRao::FROUDE_KRYLOV_RAO:
            return for_froude_krylov(parser);
        default:
            THROW(__PRETTY_FUNCTION__, InternalErrorException, "Switch case is not exhaustive.");
            break;
    }
    return for_diffraction(parser);
}


RaoInterpolator::RaoInterpolator(
    const HydroDBParser& parser, //!< Data read from the HDB or Precal_R file
    const YamlRAO& yaml_rao //<! Contents of the force model's parsed YAML data
    )
    : module()
    , phase()
    , mirror(yaml_rao.mirror)
    , period_bounds()
    , rao(RAO::get(yaml_rao.type_of_rao, parser))
    , rao_calculation_point(yaml_rao.calculation_point.x,yaml_rao.calculation_point.y,yaml_rao.calculation_point.z)
    , use_encounter_period(false)
{
    period_bounds.first = *std::min_element(rao.module_periods.begin(), rao.module_periods.end());
    period_bounds.second = *std::max_element(rao.module_periods.begin(), rao.module_periods.end());

    // For each axis (X,Y,Z,phi,theta,psi)
    for (size_t i = 0 ; i < 6 ; ++i)
    {
        // module.at(i) and phase.at(i) are vectors of vectors: each element in the vector of vectors corresponds to a frequency omega
        // For each omega, we have a vector containing the RAO values for each incidence
        module.at(i) = Interpolator(rao.module_periods, rao.module_incidence, rao.module_tables.at(i));
        phase.at(i) = Interpolator(rao.phase_periods, rao.phase_incidence, rao.phase_tables.at(i));
    }
    if (yaml_rao.use_encounter_period.is_initialized())
    {
        use_encounter_period = yaml_rao.use_encounter_period.get();
    }
}

bool RaoInterpolator::using_encounter_period() const
{
    return use_encounter_period;
}

std::vector<double> RaoInterpolator::get_module_periods() const
{
    return rao.module_periods;
}

double RaoInterpolator::interpolate_module(const size_t axis, const double Tp, double beta)
{
    double ret = 0;
    beta = beta - 2*PI * std::floor( beta / (2*PI) );
    try
    {

        if (mirror and (beta>PI))
        {
            ret = module.at(axis).f(Tp,2*PI-beta);
            // Cf; bug 3227: when RAO are given from 0 to 180 deg in the HDB file and mirroring option is one,
            // diffraction force Fy and torques Mx and Mz should have opposite signs
            if ((axis == 1) or (axis == 3) or (axis == 5))
            {
                ret *= -1;
            }
        }
        else
        {
            ret = module.at(axis).f(Tp,beta);
        }
    }
    catch(std::exception& e)
    {
        std::stringstream ss;
        const char* ax = "XYZKMN";
        ss << "Error interpolating RAO module for diffraction force: axis " << ax[axis] << ", Tp = " << Tp << " s, beta = " << beta*180/PI << "°: "  << e.what();
        THROW(__PRETTY_FUNCTION__, InternalErrorException, ss.str());
    }
    return ret;
}

double RaoInterpolator::interpolate_phase(const size_t axis, const double Tp, double beta)
{
    double ret = 0;
    beta = beta - 2*PI * std::floor( beta / (2*PI) );
    try
    {
        if (mirror and (beta>PI)) ret = phase.at(axis).f(Tp,2*PI-beta);
        else                      ret = phase.at(axis).f(Tp,beta);
    }
    catch(std::exception& e)
    {
        std::stringstream ss;
        const char* ax = "XYZKMN";
        ss << "Error interpolating RAO phase for diffraction force: axis " << ax[axis] << ", Tp = " << Tp << " s, beta = " << beta*180/PI << "°: "  << e.what();
        THROW(__PRETTY_FUNCTION__, InternalErrorException, ss.str());
    }
    return ret;
}


Eigen::Vector3d RaoInterpolator::get_rao_calculation_point() const
{
    return rao_calculation_point;
}