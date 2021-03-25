/*
 * PIDController.cpp
 *
 *  Created on: Fev, 12 2021
 *      Author: lincker
 */

#include "PIDController.hpp"
#include "InvalidInputException.hpp"

PIDController::PIDController (const double dt, const std::string &output_name,
                              const std::string &input_name,
                              const std::map<std::string, double> &states,
                              const std::string &yaml)
    : Controller (dt, output_name, input_name, states), yaml (yaml), dt (dt),
      t_start (), i_previous_t (0), initialized (false), previous_error (),
      integral_term (0)
{
}

PIDController::Yaml::Yaml (const std::string &yaml) : Kp (), Ki (), Kd ()
{
    std::stringstream stream (yaml);
    std::stringstream ss;
    YAML::Parser parser (stream);
    YAML::Node node;
    parser.GetNextDocument (node);

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
    const double previous_t = get_previous_t ();
    const double next_meeting_point = get_current_t ();
    double derivative_term = 0;

    // Proportional term
    const double proportionalTerm = yaml.Kp * error;

    if (initialized && next_meeting_point <= t)
    {
        // Integral term
        integral_term = integral_term
                        + yaml.Ki * error * (next_meeting_point - previous_t);

        // Derivative term
        derivative_term = yaml.Kd * (error - previous_error)
                         / (next_meeting_point - previous_t);

        ++i_previous_t;
    }

    if (!initialized)
    {
        t_start = t;
        initialized = true;
    }

    // Store error for next time step
    previous_error = error;

    return proportionalTerm + integral_term + derivative_term;
}

double
PIDController::get_previous_t ()
{
    if (!initialized)
    {
        return 0;
    }
    return t_start + dt * i_previous_t;
}

double
PIDController::get_current_t ()
{
    if (!initialized)
    {
        return 0;
    }
    return t_start + dt * (i_previous_t + 1);
}
