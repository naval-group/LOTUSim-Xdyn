/*
 * PIDController.hpp
 *
 *  Created on: Fev, 12 2021
 *      Author: lincker
 */

#ifndef PIDCONTROLLER_HPP_
#define PIDCONTROLLER_HPP_

#include <string>
#include <ssc/data_source.hpp>
#include "State.hpp"
#include "Controller.hpp"
#include "yaml.h"

class PIDController : public Controller
{
    public:
        struct Yaml
        {
            Yaml(const std::string& yaml);
            /* PID Controller gains */
            double Kp;
            double Ki;
            double Kd;
        };

        PIDController(const double dt,
                      const std::string& output_name,
                      const std::string& input_name,
                      const std::map<std::string, double>& states,
                      const std::string& yaml
                      );
        ~PIDController();

    private:
        /** \brief Returns the last time instant at which `callback` has been called
         */
        double get_previous_t();

        /** \brief Returns the next time instant at which `callback` should be called
         */
        double get_current_t();

        /** \brief Computes the command value from the input data
         */
        double compute_command(const double setpoint, const double measured_value, const double t);

        const Yaml yaml; //!< Controller-specific yaml

        /* PID Controller "memory" */
        const double dt; //!< Controller fixed time step
        double t_start;         //!< First time instant at which `compute_command` has been called
        double i_previous_t;    //!< Index of the last time instant at which `compute_command` was called (`previous_t = t_start + dt * i_previous_t`)
        bool initialized;       //!< False if the controller has never been called and `t_start` and `previous_error` uninitialized.
        double previous_error;  //!< Previous error (expected - measured)
        double integral_term;   //!< Previous value of the integral term
};

#endif /* PIDCONTROLLER_HPP_ */
