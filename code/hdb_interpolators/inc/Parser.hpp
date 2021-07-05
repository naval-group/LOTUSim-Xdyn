/*
 * Parser.hpp
 *
 *  Created on: July 5, 2021
 *      Author: lincker
 */

#ifndef __PARSERHPP__
#define __PARSERHPP__

#include <array>
#include <vector>


class Parser
{
    public:
        virtual ~Parser() = default;
        virtual std::array<std::vector<std::vector<double> >,6 > get_diffraction_module_tables() const = 0;
        virtual std::array<std::vector<std::vector<double> >,6 > get_diffraction_phase_tables() const = 0;
        virtual std::vector<double> get_diffraction_module_periods() const = 0;
        virtual std::vector<double> get_diffraction_phase_periods() const = 0;
        virtual std::vector<double> get_diffraction_module_psis() const = 0;
        virtual std::vector<double> get_diffraction_phase_psis() const = 0;

};


#endif