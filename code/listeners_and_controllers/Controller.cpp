/*
 * Controller.cpp
 *
 *  Created on: Fev, 15 2021
 *      Author: lincker
 */

#include "Controller.hpp"
#include "InvalidInputException.hpp"

Controller::Controller(const double date_of_first_call_, const double dt, const std::string& name_)
: ssc::solver::DiscreteSystem(date_of_first_call_, dt)
, name(name_)
{
}

Controller::~Controller()
{
}

std::vector<std::string> Controller::get_outputs() const
{
    return get_command_names();
}

std::string Controller::get_name() const
{
    return name;
}

void Controller::set_discrete_state(ssc::solver::ContinuousSystem& sys, const std::string& command_name, const double command_value)
{
    sys.set_discrete_state(command_name, command_value);
}

double Controller::get_setpoint(const ssc::solver::ContinuousSystem& sys, const std::string& setpoint_name) const
{
    return sys.get_input_value(setpoint_name);
}

double Controller::get_system_output(const ssc::solver::ContinuousSystem& sys, const std::string& state_name) const
{
    return sys.get_state_value(state_name);
}
