/*
 * ContinuousSystem.hpp
 *
 *  Created on: Feb 17, 2021
 *      Author: cady
 */

#ifndef SOLVER_CONTINUOUSSYSTEM_HPP_
#define SOLVER_CONTINUOUSSYSTEM_HPP_

#include <string>
#include <vector>

namespace ssc
{
    namespace solver
    {
        class ContinuousSystem
        {
          public:
            ContinuousSystem()
                : state()
            {
            }
            virtual ~ContinuousSystem() = default;
            virtual void operator()(const std::vector<double> &x, std::vector<double> &dx_dt,
                                    const double t)
                = 0;
            /**
             * @brief Sets the state vector to predefined values.
             *
             * By default, doesn't do anything: it's up to each system to decide how to handle this.
             * We guarantee that this function is called after a (successful) integration step.
             *
             * @param x Original states vector (can be modified)
             * @param t Current simulation time (in seconds)
             */
            virtual void force_states(std::vector<double> &, const double) const {}
            std::vector<double> state;

            /**
             * @brief Give a chance to the system to initialize its outputs before observation.
             *
             * Otherwise the initial state of the system will not be observed properly. This is to
             * be used after construction and when all necessary commands are set, but before the
             * very first observation. We made it pure virtual so that you must explicitly say
             * what you want for initialization (and not blame the solver if you don't and don't get
             * the observations you were expecting).
             * Cf.
             *
https://gitlab.com/sirehna_naval_group/xdyn/-/commit/b3e624960e941d73a8b3823e9cbf306a7679a5be
             *
             */
            virtual void initialize_system_outputs_before_first_observation() = 0;

            /** \brief Sets the value of a discrete state (output of a DiscreteSystem) in the ContinuousSystem.
             */
            virtual void set_discrete_state(const std::string &discrete_state_name, const double value) = 0;

            /** \brief Gets the value of an input from the ContinuousSystem, needed by a DiscreteSystem.
             */
            virtual double get_input_value(const std::string &name) const = 0;

            /** \brief Gets the value of state from the ContinuousSystem, needed by a DiscreteSystem.
             */
            virtual double get_state_value(const std::string &name) const = 0;
        };
    }
}

#endif /* SOLVER_CONTINUOUSSYSTEM_HPP_ */
