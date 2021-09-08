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

class StatesFilter
{
    public:
        static std::shared_ptr<StatesFilter> build(const std::string& yaml);
        double get_value(const History& h) const;

    private:
        StatesFilter();
};

#endif