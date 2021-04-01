/*
 * Controller.cpp
 *
 *  Created on: Fev, 15 2021
 *      Author: lincker
 */

#include "Controller.hpp"
#include "InvalidInputException.hpp"

Controller::Controller(const double dt,
                       const std::string& setpoint_name,
                       const std::map<std::string, double>& state_weights) :
                           ssc::solver::DiscreteSystem(dt),
                           setpoint_name(setpoint_name),
                           state_weights(state_weights)
{
}

Controller::~Controller()
{
}

/**
 * @brief Updates the controller output value in the datasource
    *
    * This method will be called by the "ssc::solver::DiscreteSystem::callback" method.
    * @param time Current simulation time (in seconds).
    * @param system The continuous system. Used to retrieve the continuous states.
    */
void Controller::set_discrete_state(ssc::solver::ContinuousSystem* sys, const std::string& command_name, const double command_value)
{
    sys->set_discrete_state(command_name, command_value);
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
