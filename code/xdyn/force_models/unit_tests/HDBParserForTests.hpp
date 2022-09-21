/*
 * HDBParserForTests.hpp
 *
 *  Created on: Dec 2, 2014
 *      Author: cady
 */

#ifndef HDBPARSERFORTESTS_HPP_
#define HDBPARSERFORTESTS_HPP_

#include "xdyn/hdb_interpolators/HDBParser.hpp"

class HDBParserForTests : public HDBParser
{
    public:
        HDBParserForTests(const std::vector<double>& omega, const std::vector<double>& Ma, const std::vector<double>& Br, const bool only_diagonal_terms_=false);
        std::vector<double> get_angular_frequencies() const override;
        std::vector<double> get_radiation_damping_coeff(const size_t i, const size_t j) const override;
        std::vector<double> get_added_mass_coeff(const size_t i, const size_t j) const override;
        Eigen::Matrix<double,6,6> get_added_mass() const override;

    private:
        HDBParserForTests();
        std::vector<double> omega;
        std::vector<double> Ma;
        std::vector<double> Br;
        bool only_diagonal_terms;
};

#endif /* HDBPARSERFORTESTS_HPP_ */
