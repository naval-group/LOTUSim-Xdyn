/*
 * Controller.hpp
 *
 *  Created on: Fev, 15 2021
 *      Author: lincker
 */

#ifndef CONTROLLER_HPP_
#define CONTROLLER_HPP_

#include <string>
#include <ssc/data_source.hpp>
#include <ssc/solver/DiscreteSystem.hpp>
#include <ssc/solver/ContinuousSystem.hpp>
#include "Sim.hpp"
/**
 * @brief Constant step controller.
 * 
 */
class Controller : public ssc::solver::DiscreteSystem
{
    public:
        Controller(const double tstart, const double dt);

        virtual ~Controller();

        /**
         * @brief Get the list of all outputs (commands) computed by the controller.
         *
         */
        virtual std::vector<std::string> get_command_names() const = 0;

    protected:
        /**
         * @brief Sets a controller output value in the datasource
         *
         * This method will be called for each controller output by the "update_discrete_states" method.
         * @param system The continuous system in which the discrete states will be updated.
         * @param command_name The name of the datasource command that should be updated.
         * @param command_value The value of the datasource command that should replace the previous one.
         */
        void set_discrete_state(ssc::solver::ContinuousSystem* system, const std::string& command_name, const double command_value);

        /** \brief Gets the value of the controller setpoint input used by `compute_command` from the datasource
         */
        double get_setpoint(const ssc::solver::ContinuousSystem* sys, const std::string& setpoint_name) const;

        /** \brief Gets the value of the system state used by the controller to compute its input
         */
        double get_system_output(const ssc::solver::ContinuousSystem* sys, const std::string& state_name) const;

    private:
        /**
         * @brief Updates the controller output value in the datasource
         *
         * This method will be called by the "ssc::solver::DiscreteSystem::callback" method.
         * @param time Current simulation time (in seconds).
         * @param system The continuous system. Used to retrieve the continuous states.
         */
        virtual void update_discrete_states(const double time, ssc::solver::ContinuousSystem* system) = 0;

};

#endif /* CONTROLLER_HPP_ */
