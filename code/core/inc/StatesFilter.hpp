/*
 * StatesFilter.hpp
 *
 *  Created on: Sep 08, 2021
 *      Author: cady
 */

#ifndef STATESFILTER_HPP_
#define STATESFILTER_HPP_

#include <memory>
#include <string>
#include "AbstractStates.hpp"

class History;

class StateFilter
{
    public:
        static std::shared_ptr<StateFilter> build(const std::string& yaml);
        double get_value(const History& h) const;

    private:
        StateFilter() = delete;
        StateFilter(const double duration_in_seconds);
        double duration_in_seconds;
};

struct YamlFilteredStates;
struct YamlRotation;

class StatesFilter
{
    public:
        StatesFilter(const YamlFilteredStates& input);
        double get_filtered_x(const AbstractStates<History>& history) const;
        double get_filtered_y(const AbstractStates<History>& history) const;
        double get_filtered_z(const AbstractStates<History>& history) const;
        double get_filtered_u(const AbstractStates<History>& history) const;
        double get_filtered_v(const AbstractStates<History>& history) const;
        double get_filtered_w(const AbstractStates<History>& history) const;
        double get_filtered_p(const AbstractStates<History>& history) const;
        double get_filtered_q(const AbstractStates<History>& history) const;
        double get_filtered_r(const AbstractStates<History>& history) const;
        double get_filtered_phi(const AbstractStates<History>& history, const YamlRotation& rotations) const;
        double get_filtered_theta(const AbstractStates<History>& history, const YamlRotation& rotations) const;
        double get_filtered_psi(const AbstractStates<History>& history, const YamlRotation& rotations) const;

    private:
        std::shared_ptr<StateFilter> x;
        std::shared_ptr<StateFilter> y;
        std::shared_ptr<StateFilter> z;
        std::shared_ptr<StateFilter> u;
        std::shared_ptr<StateFilter> v;
        std::shared_ptr<StateFilter> w;
        std::shared_ptr<StateFilter> p;
        std::shared_ptr<StateFilter> q;
        std::shared_ptr<StateFilter> r;
        std::shared_ptr<StateFilter> phi;
        std::shared_ptr<StateFilter> theta;
        std::shared_ptr<StateFilter> psi;
};

#endif