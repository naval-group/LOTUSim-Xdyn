/*
 * solve.hpp
 *
 * \date 21 mars 2014
 *      Author: cec
 */

#ifndef SOLVE_HPP_
#define SOLVE_HPP_

#include "EventHandler.hpp"
#include "Scheduler.hpp"

#include <boost/numeric/odeint/stepper/controlled_step_result.hpp>

namespace ssc
{
    namespace solver
    {
        template <typename StepperType,
                  typename SystemType,
                  typename ObserverType>
        void
        solve_for_constant_step(SystemType& sys, ObserverType& observer, StepperType& stepper, Scheduler& scheduler, EventHandler& event_handler)
        {
            const double tstart = scheduler.get_time();
            observer.observe(sys,tstart);
            while(scheduler.has_more_time_events())
            {
                const double dt = scheduler.get_step();
                const double t = scheduler.get_time();
                stepper.do_step(sys, sys.state, t, dt);
                if (event_handler.detected_state_events())
                {
                    event_handler.locate_event();
                    event_handler.run_event_actions();
                }
                const auto discrete_state_updaters = scheduler.get_discrete_state_updaters_to_run();
                for (const auto state_updater:discrete_state_updaters)
                {
                    state_updater(t, sys.state);
                }
                scheduler.advance_to_next_time_event();
                observer.observe(sys, scheduler.get_time());
            }
        }

        template <typename StepperType,
                  typename SystemType,
                  typename ObserverType>
        void
        solve_for_adaptive_step(SystemType& sys, ObserverType& observer, StepperType& stepper, Scheduler& scheduler, EventHandler& event_handler)
        {
            const double tstart = scheduler.get_time();
            observer.observe(sys,tstart);
            while(scheduler.has_more_time_events())
            {
                const double t = scheduler.get_time();
                const double dt = scheduler.get_step();
                const boost::numeric::odeint::controlled_step_result res = stepper.try_step(sys, sys.state, t, dt);
                if (res == boost::numeric::odeint::success)
                {
                    if (event_handler.detected_state_events())
                    {
                        event_handler.locate_event();
                        event_handler.run_event_actions();
                    }
                    const auto discrete_state_updaters = scheduler.get_discrete_state_updaters_to_run();
                    for (const auto state_updater:discrete_state_updaters)
                    {
                        state_updater(t, sys.state);
                    }
                    scheduler.advance_to_next_time_event();
                    observer.observe(sys, t);
                }
                else
                {
                    scheduler.add_time_event(t + dt/2);
                    scheduler.advance_to_next_time_event();
                }
            }
        }

        template <typename StepperType,
                  typename SystemType,
                  typename ObserverType>
        void quicksolve(SystemType& sys, Scheduler& scheduler, ObserverType& observer)
        {
            StepperType stepper;
            EventHandler event_handler;
            solve_for_constant_step<StepperType,SystemType,ObserverType>(sys,observer,stepper,scheduler,event_handler);
        }
    }
}

#endif /* SOLVE_HPP_ */
