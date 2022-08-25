/*
 * DiscreteDirectionalWaveSpectrum.cpp
 *
 *  Created on: Jul 31, 2014
 *      Author: cady
 */

#include "DiscreteDirectionalWaveSpectrum.hpp"
#include "InvalidInputException.hpp"

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

FlatDiscreteDirectionalWaveSpectrum::FlatDiscreteDirectionalWaveSpectrum() :
    a(),
    omega(),
    psi(),
    cos_psi(),
    sin_psi(),
    k(),
    phase(),
    pdyn_factor(),
    pdyn_factor_sh()
{
}
std::vector<double> FlatDiscreteDirectionalWaveSpectrum::get_periods() const
{
    std::vector<double> periods;
    periods.reserve(omega.size());
    for (size_t i=0;i<omega.size();++i)
    {
        periods.push_back(2.0 * PI / omega.at(i));
    }
    return periods;
}

DiscreteDirectionalWaveSpectrum::DiscreteDirectionalWaveSpectrum() :
    Si(),
    Dj(),
    omega(),
    psi(),
    k(),
    phase(),
    pdyn_factor(),
    pdyn_factor_sh()
{
}

void DiscreteDirectionalWaveSpectrum::check_sizes() const
{
    const size_t n_si = Si.size();
    const size_t n_dj = Dj.size();
    const size_t n_omega = omega.size();
    const size_t n_psi = psi.size();
    const size_t n_k = k.size();
    const size_t n_phase = phase.size();
    if (n_si==0)
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "No data for Si");
    }
    if (n_omega==0)
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "No data for omega");
    }
    if (n_dj==0)
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "No data for Dj");
    }
    if (n_psi==0)
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "No data for psi");
    }
    if (n_k==0)
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "No data for k");
    }
    if (n_phase==0)
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "No data for phase");
    }
    if (n_si!=n_omega)
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException,
            "Si and omega should have the same size"+std::to_string(n_si)+"!="+std::to_string(n_omega));
    }
    if (n_si!=n_k)
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException,
            "Si and k should have the same size"+std::to_string(n_si)+"!="+std::to_string(n_k));
    }
    if (n_dj!=n_psi)
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException,
            "Dj and psi should have the same size"+std::to_string(n_dj)+"!="+std::to_string(n_psi));
    }
    if (n_si!=n_phase)
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException,
            "Si and phase should have the same size"+std::to_string(n_si)+"!="+std::to_string(n_phase));
    }
    for (size_t i_omega=0;i_omega<n_omega;++i_omega)
    {
        if (phase.at(i_omega).empty())
        {
            THROW(__PRETTY_FUNCTION__, InvalidInputException, "phase["+std::to_string(i_omega)+"] is empty");
        }
        if (phase.at(i_omega).size()!=n_dj)
        {
            THROW(__PRETTY_FUNCTION__, InvalidInputException,
                "phase["+std::to_string(i_omega)+"] and Dj should have the same size "+
                std::to_string(phase.at(i_omega).size())+"!="+std::to_string(n_dj));
        }
    }
}
