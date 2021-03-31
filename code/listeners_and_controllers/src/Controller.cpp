/*
 * Controller.cpp
 *
 *  Created on: Fev, 15 2021
 *      Author: lincker
 */

#include "Controller.hpp"
#include "InvalidInputException.hpp"

Controller::Controller(const double dt,
                       const std::string& output_name,
                       const std::string& setpoint_name,
                       const std::map<std::string, double>& state_weights) :
                           ssc::solver::DiscreteSystem(dt),
                           output_name(output_name),
                           setpoint_name(setpoint_name),
                           state_weights(state_weights)
{
}

Controller::~Controller()
{
}

std::string Controller::get_name() const
{
    return output_name;
}

void Controller::update_discrete_states(const double time, ssc::solver::ContinuousSystem* sys)
{
    const double command_value = compute_command(get_setpoint(sys), get_measured_value(sys), time);
    sys->set_discrete_state(get_name(), command_value);
}

double Controller::get_setpoint(const ssc::solver::ContinuousSystem* sys) const
{
    return sys->get_input_value(setpoint_name);
}

double Controller::get_measured_value(const ssc::solver::ContinuousSystem* sys) const
{
    double measured_state = 0;
    for (const auto& name_coeff : state_weights)
    {
        measured_state += name_coeff.second * sys->get_state_value(name_coeff.first);
    }
    return measured_state;
}
