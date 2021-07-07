/*
 * HydroDBParser.hpp
 *
 *  Created on: July 5, 2021
 *      Author: lincker
 */

#ifndef __HYDRODBPARSERHPP__
#define __HYDRODBPARSERHPP__

#include <array>
#include <vector>
#include <Eigen/Dense>
#include "TimestampedMatrix.hpp"


class HydroDBParser
{
    public:
        virtual ~HydroDBParser() = default;

        virtual Eigen::Matrix<double,6,6> get_added_mass() const = 0;

        virtual std::array<std::vector<std::vector<double> >,6 > get_diffraction_module_tables() const = 0;
        virtual std::array<std::vector<std::vector<double> >,6 > get_diffraction_phase_tables() const = 0;
        virtual std::vector<double> get_diffraction_module_periods() const = 0;
        virtual std::vector<double> get_diffraction_phase_periods() const = 0;
        virtual std::vector<double> get_diffraction_module_psis() const = 0;
        virtual std::vector<double> get_diffraction_phase_psis() const = 0;

};


#endif