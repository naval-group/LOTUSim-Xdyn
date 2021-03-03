/*
 * Controller.cpp
 *
 *  Created on: Fev, 15 2021
 *      Author: lincker
 */

#include "Controller.hpp"

Controller::Controller(const double dt,
                       const std::string& output_name,
                       const std::string& input_name,
                       const std::map<std::string, double>& states) :
                           ssc::solver::DiscreteSystem(dt),
                           output_name(output_name),
                           input_name(input_name),
                           states(states)
{
}

Controller::~Controller()
{
}

std::string Controller::get_name() const
{
    return output_name;
}

void Controller::update_discrete_states(const double , ssc::solver::ContinuousSystem* )
{
}
