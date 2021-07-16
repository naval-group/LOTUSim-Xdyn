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
        /**
         * @brief Get the angular frequencies for the radiation damping and added mass matrices
         */
        virtual std::vector<double> get_angular_frequencies() const = 0;
        /**
         * @brief Speed at which the radiation damping matrices were calculated.
         * Used to determine if we can apply a forward-speed correction to the radiation damping
         * force model.
         */
        virtual double get_forward_speed() const = 0;
        /**
         * @brief Added mass matrix coefficient Ma(i,j) = f(omega) for all omegas
         */
        virtual std::vector<double> get_added_mass_coeff(const size_t i, const size_t j) const = 0;
        /**
         * @brief Radiation damping matrix coefficient Br(i,j) = f(omega) for all omegas
         */
        virtual std::vector<double> get_radiation_damping_coeff(const size_t i, const size_t j) const = 0;

};


#endif
