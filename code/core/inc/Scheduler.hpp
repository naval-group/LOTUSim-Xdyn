/*
 * Scheduler.hpp
 *
 * \date 21 mars 2014
 *      Author: cec
 */

#ifndef SCHEDULER_HPP_
#define SCHEDULER_HPP_

#include <cmath>
#include <functional>
#include <list>
#include <vector>

#include "ContinuousSystem.hpp"
namespace ssc
{
    namespace solver
    {
        class Scheduler
        {
          public:
            typedef std::function<void(Scheduler &scheduler, ContinuousSystem *system)> Callback;
            Scheduler(const double tstart, const double tend_, const double dt)
                : current_time(tstart)
                , scheduled_time_events()
                , discrete_state_updaters()
                , t0(tstart)
                , tend(tend_)
                , tolerance(0.01 * dt)
            {
                if (dt > 0)
                {
                    double t = tstart;
                    size_t i = 0;
                    while (t + dt < tend)
                    {
                        t = tstart + ((double)(++i)) * dt;
                        scheduled_time_events.push_back(t);
                    }
                    scheduled_time_events.push_back(tend);
                    scheduled_time_events.sort();
                }
            }

            virtual ~Scheduler() = default;

            virtual bool has_more_time_events() const { return not(scheduled_time_events.empty()); }

            /**
             * @brief Get the discrete state updaters to run & remove them from list.
             *
             * @return Callbacks to run to update the discrete states (e.g. one updater per
             * controller).
             */
            virtual std::vector<Callback> get_discrete_state_updaters_to_run()
            {
                std::vector<Callback> updaters_to_run;
                std::vector<std::pair<double, Callback> > remaining_updaters;
                remaining_updaters.reserve(discrete_state_updaters.size());
                for (const auto t_updater : discrete_state_updaters)
                {
                    if (t_updater.first <= current_time) // Need to run updater
                    {
                        updaters_to_run.push_back(t_updater.second);
                    }
                    else // Add updater to the list of updaters to keep
                    {
                        remaining_updaters.push_back(t_updater);
                    }
                }
                // Only keep selected updaters, discard the rest
                discrete_state_updaters = remaining_updaters;
                return updaters_to_run;
            }

            virtual void add_time_event(const double t)
            {
                if (t <= tend && is_new_time_event(t))
                {
                    scheduled_time_events.push_back(t);
                    scheduled_time_events.sort();
                }
            }

            virtual void schedule_discrete_state_update(const double t, Callback &updater)
            {
                discrete_state_updaters.push_back(std::make_pair(t, updater));
                add_time_event(t);
            }

            virtual void advance_to_next_time_event()
            {
                if (not(scheduled_time_events.empty()))
                {
                    current_time = scheduled_time_events.front();
                    scheduled_time_events.pop_front();
                }
            }

            virtual double get_time() const { return current_time; }

            virtual double get_step() const
            {
                if (not(scheduled_time_events.empty()))
                {
                    const double next_time = scheduled_time_events.front();
                    return next_time - current_time;
                }
                return 0;
            }

            double get_t0() const { return t0; }

          private:
            Scheduler();
            double current_time;
            std::list<double> scheduled_time_events;
            std::vector<std::pair<double, Callback> > discrete_state_updaters;
            double t0;
            double tend;
            const double tolerance;

            bool is_new_time_event(const double new_t) const
            {
                for (double t : scheduled_time_events)
                {
                    if (std::abs(t - new_t) <= tolerance)
                    {
                        return false;
                    }
                    if (t > new_t)
                    {
                        return true; // scheduled_time_events are sorted, so we're sure abs(t-new_t) > tolerance
                    }
                }
                return false; // new_t > tend
            }
        };
    }
}

#endif /* SCHEDULER_HPP_ */
