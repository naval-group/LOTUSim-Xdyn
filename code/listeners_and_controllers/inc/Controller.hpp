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
#include "DiscreteSystem.hpp"
#include "ContinuousSystem.hpp"
#include "Sim.hpp"

class Controller : public ssc::solver::DiscreteSystem
{
    public:
        Controller(const double dt,
                   const std::string& output_name,
                   const std::string& setpoint_name,
                   const std::map<std::string, double>& state_weights
                   );

        virtual ~Controller();

        /** \brief Returns the name of the command computed by the controller
         * (suffixed by the controlled force name), as will be written in the datasource
         */
        std::string get_name() const;

    private:
        const std::string output_name;                     //!< Name of the datasource command outputed by the controller
        const std::string setpoint_name;                   //!< Name of the datasource setpoint needed by the controller
        const std::map<std::string, double> state_weights; //!< Weights associated to each state, used to compute the controller's measured input,
                                                           //   with the convention that any missing state has weight 0.
                                                           //   For example "u - 2v" -> { {"u", 1}, {"v", -2} }

        /**
         * @brief Updates the controller output value in the datasource
         *
         * This method will be called by the "ssc::solver::DiscreteSystem::callback" method.
         * @param time Current simulation time (in seconds).
         * @param system The continuous system. Used to retrieve the continuous states.
         */
        void update_discrete_states(const double time, ssc::solver::ContinuousSystem* system);

        /** \brief Gets the value of the controller setpoint input used by `compute_command` from the datasource
         */
        double get_setpoint(const ssc::solver::ContinuousSystem* sys) const;

        /** \brief Gets the value of the controller measured input used by `compute_command` from the system states
         */
        double get_measured_value(const ssc::solver::ContinuousSystem* sys) const;

        /** \brief Computes the command value from the input data
         */
        virtual double compute_command(const double setpoint, const double measured_value, const double t) = 0;

};

#endif /* CONTROLLER_HPP_ */
