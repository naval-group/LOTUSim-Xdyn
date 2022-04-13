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

/*
 * Tests for PIDController are found in `observers_and_api/unit_tests/src/PIDControllerTest.cpp`
 * because these tests need a Sim instance, which requires the observers_and_api include directory.
 * To avoid cross-dependencies, the corresponding unit tests are moved to observers_and_api/unit_tests.
 */

class PIDController : public Controller
{
    public:
        struct Yaml
        {
            Yaml(const std::string& yaml);
            /* PID Controller gains */
            double Kp; //!< Proportional gain
            double Ki; //!< Integral gain
            double Kd; //!< Derivative gain
            /* PID Controller input */
            std::map<std::string, double> state_weights;  //!< Weights associated to each state, used to compute the controller's measured input,
                                                          //   with the convention that any missing state has weight 0.
                                                          //   For example "u - 2v" -> { {"u", 1}, {"v", -2} }
            std::string setpoint_name;
            /* PID Controller output */
            std::string command_name;
        };

        PIDController(const double tstart,
                      const double dt,
                      const std::string& name,
                      const std::string& yaml
                      );

        std::vector<std::string> get_command_names() const;
        const Yaml yaml; //!< Controller-specific yaml

    private:
        /**
         * @brief Updates the controller output value in the datasource
         *
         * This method will be called by the "ssc::solver::DiscreteSystem::callback" method.
         * @param time Current simulation time (in seconds).
         * @param system The continuous system. Used to retrieve the continuous states.
         */
        void update_discrete_states(const double time, ssc::solver::ContinuousSystem& system);

        /** \brief Gets the value of the controller measured input used by `compute_command` from the system states
         */
        double get_measured_value(const ssc::solver::ContinuousSystem& sys) const;

        /** \brief Computes the command value from the input data
         */
        double compute_command(const double setpoint, const double measured_value, const double t);

        /* PID Controller "memory" */
        bool initialized;       //!< False if the controller has never been called and `t_start` and `previous_error` uninitialized.
        double previous_t;      //!< Previous time instant at which `compute_command` was called
        double previous_error;  //!< Previous error (expected - measured)
        double integral_term;   //!< Previous value of the integral term
};

#endif /* PIDCONTROLLER_HPP_ */
