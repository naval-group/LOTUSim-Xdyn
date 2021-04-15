/*
 * solve.hpp
 *
 * \date 21 mars 2014
 *      Author: cec
 */

#ifndef SOLVE_HPP_
#define SOLVE_HPP_

#include "DiscreteSystem.hpp"
#include "EventHandler.hpp"
#include "Scheduler.hpp"

#include "ssc/macros/tr1_macros.hpp"
#include <boost/numeric/odeint/stepper/controlled_step_result.hpp>

#include TR1INC(memory)

namespace ssc
{
    namespace solver
    {
        template <typename StepperType, typename SystemType, typename ObserverType>
        void solve_for_constant_step(SystemType &sys, ObserverType &observer, StepperType &stepper,
                                     Scheduler &scheduler, EventHandler &event_handler)
        {
            const double tstart = scheduler.get_time();
            sys.initialize_system_outputs_before_first_observation();
            observer.observe(sys, tstart);
            while (scheduler.has_more_time_events())
            {
                const double dt = scheduler.get_step();
                const double t = scheduler.get_time();
                stepper.do_step(sys, sys.state, t, dt);
                sys.force_states(sys.state, t);
                if (event_handler.detected_state_events())
                {
                    event_handler.locate_event();
                    event_handler.run_event_actions();
                }
                const auto discrete_state_updaters = scheduler.get_discrete_state_updaters_to_run();
                for (const auto state_updater : discrete_state_updaters)
                {
                    state_updater(scheduler, &sys);
                }
                scheduler.advance_to_next_time_event();
                observer.observe(sys, scheduler.get_time());
            }
        }

        template <typename StepperType, typename SystemType, typename ObserverType>
        void solve_for_adaptive_step(SystemType &sys, ObserverType &observer, StepperType &stepper,
                                     Scheduler &scheduler, EventHandler &event_handler)
        {
            const double tstart = scheduler.get_time();
            sys.initialize_system_outputs_before_first_observation();
            observer.observe(sys, tstart);
            while (scheduler.has_more_time_events())
            {
                const double t = scheduler.get_time();
                const double dt = scheduler.get_step();
                const boost::numeric::odeint::controlled_step_result res
                    = stepper.try_step(sys, sys.state, t, dt);
                if (res == boost::numeric::odeint::success)
                {
                    sys.force_states(sys.state, t);
                    if (event_handler.detected_state_events())
                    {
                        event_handler.locate_event();
                        event_handler.run_event_actions();
                    }
                    const auto discrete_state_updaters
                        = scheduler.get_discrete_state_updaters_to_run();
                    for (const auto state_updater : discrete_state_updaters)
                    {
                        state_updater(scheduler, &sys);
                    }
                    scheduler.advance_to_next_time_event();
                    observer.observe(sys, t);
                }
                else
                {
                    scheduler.add_time_event(t + dt / 2);
                    scheduler.advance_to_next_time_event();
                }
            }
        }
        template <typename StepperType, typename SystemType, typename ObserverType>
        void quicksolve(SystemType &sys, Scheduler &scheduler, ObserverType &observer)
        {
            StepperType stepper;
            EventHandler event_handler;
            solve_for_constant_step<StepperType, SystemType>(sys, observer, stepper, scheduler,
                                                             event_handler);
        }
        typedef TR1(shared_ptr)<DiscreteSystem> DiscreteSystemPtr;

        template <typename StepperType, typename SystemType, typename ObserverType>
        void quicksolve(SystemType &sys, std::vector<DiscreteSystemPtr> &discrete_systems,
                        Scheduler &scheduler, ObserverType &observer)
        {
            StepperType stepper;
            EventHandler event_handler;
            for (const auto &discrete_system : discrete_systems)
            {
                discrete_system->schedule_update(scheduler.get_t0(), scheduler);
            }
            solve_for_constant_step<StepperType, SystemType, ObserverType>(sys, observer, stepper,
                                                                           scheduler, event_handler);
        }

        template <typename StepperType, typename SystemType, typename ObserverType>
        void quicksolve(SystemType &sys, DiscreteSystem &discrete_system, Scheduler &scheduler,
                        ObserverType &observer)
        {
            quicksolve<StepperType, SystemType, ObserverType>(
                sys,
                std::vector<DiscreteSystemPtr>(1,
                                               std::make_shared<DiscreteSystem>(&discrete_system)),
                scheduler,
                observer);
        }

    }
}

#endif /* SOLVE_HPP_ */
