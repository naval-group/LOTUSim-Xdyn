/*
 * solver.hpp
 *
 *  Created on: Aug 25, 2020
 *      Author: cady
 */

#ifndef CORE_INC_SOLVER_HPP_
#define CORE_INC_SOLVER_HPP_

/* This file was created because forced states don't work properly in xdyn:
 * forced state values are indeed written to the observers & fed to the force
 * models, but unforced states are integrated by the solver & when the states
 * are no longer forced the system behaves as if no states had ever been forced.
 * Instead of fixing this in the SSC, which requires pushing to GitHub, updating
 * the Docker base images, etc. we created this file, which almost duplicates
 * what is done in the SSC's solve.hpp but accounts for forced values.
 */

#include <Scheduler.hpp>
#include <EventHandler.hpp>
#include <steppers.hpp>
#include "Sim.hpp"

namespace ssc
{
    namespace solver
    {

        template <typename StepperType, typename ObserverType, typename StateForcer>
        void quicksolve(Sim& sys, const double tstart, const double tend, double dt, ObserverType& observer, StateForcer& force_states)
        {
            StepperType stepper;
            ssc::solver::Scheduler scheduler(tstart, tend, dt);
            ssc::solver::EventHandler event_handler;
            sys.initialize_system_outputs_before_first_observation();
            observer.observe(sys,tstart);
            while(scheduler.has_more_time_events())
            {
                const double t = scheduler.get_time();
                stepper.do_step(sys, sys.state, t, dt);
                force_states(sys.state, t);
                if (event_handler.detected_state_events())
                {
                    event_handler.locate_event();
                    event_handler.run_event_actions();
                }
                const auto discrete_state_updaters = scheduler.get_discrete_state_updaters_to_run();
                for (const auto state_updater : discrete_state_updaters)
                {
                    state_updater(t, sys.state);
                }
                scheduler.advance_to_next_time_event();
                observer.observe(sys, scheduler.get_time());
            }
        }
    }
}
#endif /* CORE_INC_SOLVER_HPP_ */
