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
                   const std::string& input_name,
                   const std::map<std::string, double>& states
                   );

        virtual ~Controller();

        /** \brief Returns the name of the command computed by the controller
         * (suffixed by the controlled force name), as will be written in the datasource
         */
        std::string get_name() const;

    private:
        const std::string output_name;              //!< Name of the datasource command outputed by the controller
        const std::string input_name;               //!< Name of the datasource input needed by the controller (`setpoint` for the controller algorithm)
        const std::map<std::string, double> states; //!< Name and coefficients of the measured states needed by the controller
                                                    //   (to compute the `measured_value` input of the controller algorithm)
                                                    //   for example: "u - 2v" -> { {"u", 1}, {"v", -2} }

        /**
         * @brief Updates the controller output value in the datasource
         *
         * This method will be called by the "ssc::solver::DiscreteSystem::callback" method.
         * @param time Current simulation time (in seconds).
         * @param system The continuous system. Used to retrieve the continuous states.
         */
        void update_discrete_states(const double time, ssc::solver::ContinuousSystem* system);

        /** \brief Gets the value of the controller input used by `compute_command` from the datasource
         */
        double get_setpoint(const Sim* sys) const;

        /** \brief Gets the value of the controller measured input used by `compute_command` from the system states
         */
        double get_measured_value(const Sim* sys) const;

        /** \brief Gets the system states value corresponding to the given name
         */
        double get_state_value_from_name(const Sim* sys, const std::string name) const;

};

#endif /* CONTROLLER_HPP_ */
