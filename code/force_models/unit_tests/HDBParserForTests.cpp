/*
 * HDBParserForTests.cpp
 *
 *  Created on: Dec 2, 2014
 *      Author: cady
 */

#include "HDBParserForTests.hpp"

HDBParserForTests::HDBParserForTests(const std::vector<double>& omega_, const std::vector<double>& Ma_, const std::vector<double>& Br_, const bool only_diagonal_terms_)
: omega(omega_)
, Ma(Ma_), Br(Br_), only_diagonal_terms(only_diagonal_terms_)
{
}

std::vector<double> HDBParserForTests::get_angular_frequencies() const
{
    return omega;
}

std::vector<double> HDBParserForTests::get_radiation_damping_coeff(const size_t i, const size_t j) const
{
    if (only_diagonal_terms)
    {
        if (i == j)
        {
            return Br;
        }
    }
    else
    {
        std::vector<double> ret;
        ret.reserve(Br.size());
        const double i_double = static_cast<double>(i);
        const double j_double = static_cast<double>(j);
        for (const auto br:Br)
        {
            ret.push_back((10*(i_double+1)+(j_double+1))*br);
        }
        return ret;
    }
    return std::vector<double>(Br.size(),0);
}

std::vector<double> HDBParserForTests::get_added_mass_coeff(const size_t i, const size_t j) const
{
    if (only_diagonal_terms)
    {
        if (i == j)
        {
            return Ma;
        }
    }
    else
    {
        std::vector<double> ret;
        ret.reserve(Ma.size());
        const double i_double = static_cast<double>(i);
        const double j_double = static_cast<double>(j);
        for (const auto ma:Ma)
        {
            ret.push_back((10*(i_double+1)+(j_double+1))*ma);
        }
        return ret;
    }
    return std::vector<double>(Ma.size(),0);
}

Eigen::Matrix<double, 6, 6> HDBParserForTests::get_added_mass() const
{
    Eigen::Matrix<double, 6, 6> ret;
    for (size_t i = 0 ; i < 6 ; i++)
    {
        for(size_t j = 0 ; j < 6 ; j++)
        {
            ret(i, j) = get_added_mass_coeff(i, j).back();
        }
    }
    return ret;
}
