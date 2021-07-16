/*
 * DiffractionInterpolator.cpp
 *
 *  Created on: Dec 17, 2014
 *      Author: cady
 */

#include "DiffractionInterpolator.hpp"
#include "InternalErrorException.hpp"
#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

#include "HydroDBParser.hpp"
#include "YamlRAO.hpp"
#include <algorithm>

std::vector<double> reverse(std::vector<double> t);
std::vector<double> reverse(std::vector<double> t)
{
    std::reverse(t.begin(), t.end());
    return t;
}

DiffractionInterpolator::DiffractionInterpolator(const HydroDBParser& data, //!< Data read from the HDB or Precal_R file
                                                 const YamlRAO& diffraction_yaml //<! Contents of the force model's parsed YAML data
        ) : DiffractionInterpolator(data, {}, {}, diffraction_yaml)
{}

std::array<std::vector<std::vector<double> >,6 > DiffractionInterpolator::get_module_tables(const HydroDBParser& parser) const
{
    return parser.get_diffraction_module_tables();
}

std::array<std::vector<std::vector<double> >,6 > DiffractionInterpolator::get_phase_tables(const HydroDBParser& parser) const
{
    return parser.get_diffraction_phase_tables();
}

std::vector<double> DiffractionInterpolator::get_phase_periods(const HydroDBParser& parser) const
{
    return parser.get_diffraction_phase_periods();
}

std::vector<double> DiffractionInterpolator::get_module_incidence(const HydroDBParser& parser) const
{
    return parser.get_diffraction_module_psis();
}

std::vector<double> DiffractionInterpolator::get_phase_incidence(const HydroDBParser& parser) const
{
    return parser.get_diffraction_phase_psis();
}

DiffractionInterpolator::DiffractionInterpolator(const HydroDBParser& parser, //!< Data read from the HDB or Precal_R file
                                                 const std::vector<double>& omega, //!< Angular frequencies in the wave spectrum (points at which to interpolate the HDB data)
                                                 const std::vector<double>& psi, //!< Wave directions (points at which to interpolate the HDB data)
                                                 const YamlRAO& yaml_rao //<! Contents of the force model's parsed YAML data
        )
        : module()
        , phase()
        , mirror(yaml_rao.mirror)
        , omegas(omega)
        , psis(psi)
        , period_bounds()
        , module_periods(parser.get_diffraction_module_periods())
        , rao_calculation_point(yaml_rao.calculation_point.x,yaml_rao.calculation_point.y,yaml_rao.calculation_point.z)
        , use_encounter_period(false)
{
    const auto module_tables = get_module_tables(parser);
    const auto phase_tables = get_phase_tables(parser);
    const auto phase_periods = get_phase_periods(parser);
    const auto module_incidence = get_module_incidence(parser);
    const auto phase_incidence = get_phase_incidence(parser);
    std::reverse(omegas.begin(),omegas.end());
    period_bounds.first = *std::min_element(module_periods.begin(), module_periods.end());
    period_bounds.second = *std::max_element(module_periods.begin(), module_periods.end());
    
    // For each axis (X,Y,Z,phi,theta,psi)
    for (size_t i = 0 ; i < 6 ; ++i)
    {
        // module.at(i) and phase.at(i) are vectors of vectors: each element in the vector of vectors corresponds to a frequency omega
        // For each omega, we have a vector containing the RAO values for each incidence
        module.at(i) = Interpolator(module_periods, module_incidence, module_tables.at(i));
        phase.at(i) = Interpolator(phase_periods, phase_incidence, phase_tables.at(i));
    }
    if (yaml_rao.use_encounter_period.is_initialized())
    {
        use_encounter_period = yaml_rao.use_encounter_period.get();
    }
}

bool DiffractionInterpolator::using_encounter_period() const
{
    return use_encounter_period;
}

std::vector<double> DiffractionInterpolator::get_module_periods() const
{
    return module_periods;
}

std::vector<std::vector<double> > DiffractionInterpolator::get_array_cartesian(Interpolator& i) const
{
    std::vector<std::vector<double> > ret;
    for (auto omega:omegas)
    {
        std::vector<double> v;
        for (auto psi:psis)
        {
            if (mirror and (psi>PI)) v.push_back(i.f(2*PI/omega, 2*PI-psi));
            else                     v.push_back(i.f(2*PI/omega, psi));
        }
        ret.push_back(v);
    }
    return ret;
}

std::vector<std::vector<double> > DiffractionInterpolator::get_modules_cartesian(const size_t k //!< Axis index (0 for Fx, 1 for Fy, 2 for Fz, 3 for Mx, 4 for My and 5 for Mz)
                                                     )
{
    return get_array_cartesian(module.at(k));
}

std::vector<std::vector<double> > DiffractionInterpolator::get_phases_cartesian(const size_t k //!< Axis index (0 for Fx, 1 for Fy, 2 for Fz, 3 for Mx, 4 for My and 5 for Mz)
                                         )
{
    return get_array_cartesian(phase.at(k));
}

double DiffractionInterpolator::interpolate_module(const size_t axis, const double Tp, double beta)
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

double DiffractionInterpolator::interpolate_phase(const size_t axis, const double Tp, double beta)
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


Eigen::Vector3d DiffractionInterpolator::get_rao_calculation_point() const
{
    return rao_calculation_point;
}