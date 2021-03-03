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

double Controller::get_setpoint(const Sim* sys) const
{
    return sys->get_command(input_name);
}

double Controller::get_measured_value(const Sim* sys) const
{
    double measured_state = 0;
    for (const auto& name_coeff : states)
    {
        measured_state += name_coeff.second * get_state_value_from_name(sys, name_coeff.first);
    }
    return measured_state;
}

double Controller::get_state_value_from_name(const Sim* sys, const std::string name) const
{
    if (name == "x") {
        return sys->get_bodies().front()->get_states().x();
    } else if (name == "y") {
        return sys->get_bodies().front()->get_states().y();
    } else if (name == "z") {
        return sys->get_bodies().front()->get_states().z();
    } else if (name == "u") {
        return sys->get_bodies().front()->get_states().u();
    } else if (name == "v") {
        return sys->get_bodies().front()->get_states().v();
    } else if (name == "w") {
        return sys->get_bodies().front()->get_states().w();
    } else if (name == "p") {
        return sys->get_bodies().front()->get_states().p();
    } else if (name == "q") {
        return sys->get_bodies().front()->get_states().q();
    } else if (name == "r") {
        return sys->get_bodies().front()->get_states().r();
    } else if (name == "qr") {
        return sys->get_bodies().front()->get_states().qr();
    } else if (name == "qi") {
        return sys->get_bodies().front()->get_states().qi();
    } else if (name == "qj") {
        return sys->get_bodies().front()->get_states().qj();
    } else if (name == "qk") {
        return sys->get_bodies().front()->get_states().qk();
    } else if (name == "phi") {
        return sys->get_bodies().front()->get_states().get_angles().phi;
    } else if (name == "theta") {
        return sys->get_bodies().front()->get_states().get_angles().theta;
    } else if (name == "psi") {
        return sys->get_bodies().front()->get_states().get_angles().psi;
    } else {
        THROW(__PRETTY_FUNCTION__, InvalidInputException,
              "Something is wrong with " << output_name << "controller YAML, more specifically in the 'states' section: '" << name << "' is not a valid state. Examples of valid states: 'x', 'u', 'qr', 'psi', etc.."
              );
    }
}
