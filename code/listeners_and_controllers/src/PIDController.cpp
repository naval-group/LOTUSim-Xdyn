/*
 * PIDController.cpp
 *
 *  Created on: Fev, 12 2021
 *      Author: lincker
 */

#include "PIDController.hpp"
#include "check_input_yaml.hpp"
#include "InvalidInputException.hpp"

PIDController::PIDController (const double tstart,
                              const double dt,
                              const std::string &yaml)
    : Controller (tstart, dt), yaml (yaml),
      initialized (false),
      previous_t (0), previous_error (0),
      integral_term (0)
{
}

PIDController::Yaml::Yaml (const std::string &yaml) : Kp (), Ki (), Kd (), state_weights (), setpoint_name (), command_name ()
{
    std::stringstream stream (yaml);
    std::stringstream ss;
    YAML::Parser parser (stream);
    YAML::Node node;
    parser.GetNextDocument (node);

    node["setpoint"] >> setpoint_name;
    node["command"] >> command_name;

    try
    {
        node["gains"]["Kp"] >> Kp;
        node["gains"]["Ki"] >> Ki;
        node["gains"]["Kd"] >> Kd;
    }
    catch (YAML::Exception &e)
    {
        THROW (__PRETTY_FUNCTION__, InvalidInputException,
               "Unable to parse 'gains': it should contain the sub-nodes "
               "'Kp', 'Ki' and 'Kd'.");
    }

    for(YAML::Iterator it=node["state weights"].begin();it!=node["state weights"].end();++it)
    {
        std::string key = "";
        it.first() >> key;
        try
        {
            check_state_name(key);
            double value;
            node["state weights"][key] >> value;
            state_weights[key] = value;
        }
        catch(const InvalidInputException& e)
        {
            THROW(__PRETTY_FUNCTION__, InvalidInputException, "Something is wrong with the YAML, more specifically in the 'state weights' section. When parsing the '" << key << "' state: " << e.get_message());
        }
        catch(const YAML::Exception& e)
        {
            THROW(__PRETTY_FUNCTION__, InvalidInputException, "Something is wrong with the YAML, more specifically in the 'state weights' section. When parsing the '" << key << "' values: " << e.msg);
        }
    }
}


/**
 * @brief Updates the controller output value in the datasource
 *
 * This method will be called by the "ssc::solver::DiscreteSystem::callback" method.
 * @param time Current simulation time (in seconds).
 * @param system The continuous system. Used to retrieve the continuous states.
 */
void
PIDController::update_discrete_states (const double time, ssc::solver::ContinuousSystem* sys)
{
    const double command_value = compute_command(Controller::get_setpoint(sys, yaml.setpoint_name),
                                                 get_measured_value(sys),
                                                 time);
    Controller::set_discrete_state(sys, yaml.command_name, command_value);
}

/** \brief Gets the value of the controller measured input used by `compute_command` from the system states
 */
double
PIDController::get_measured_value(const ssc::solver::ContinuousSystem* sys) const
{
    double measured_state = 0;
    for (const auto& name_coeff : yaml.state_weights)
    {
        measured_state += name_coeff.second * Controller::get_system_output(sys, name_coeff.first);
    }
    return measured_state;
}


/** \brief Computes the command value from the input data, using a PID
 * controller algorithm.
 * https://en.wikipedia.org/wiki/PID_controller#Pseudocode
 */
double
PIDController::compute_command (const double setpoint,
                                const double measured_value, const double t)
{
    const double error = setpoint - measured_value;
    double derivative_term = 0;

    // Proportional term
    const double proportional_term = yaml.Kp * error;

    if (initialized && t > previous_t)
    {
        // Integral term
        integral_term = integral_term + yaml.Ki * error * (t - previous_t);

        // Derivative term
        derivative_term = yaml.Kd * (error - previous_error) / (t - previous_t);
    }

    if (!initialized)
    {
        initialized = true;
    }

    // Store error and time for next time step
    previous_t = t;
    previous_error = error;

    return proportional_term + integral_term + derivative_term;
}
