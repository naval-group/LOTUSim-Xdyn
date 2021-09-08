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

#endif