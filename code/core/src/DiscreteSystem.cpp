#include "DiscreteSystem.hpp"

ssc::solver::DiscreteSystem::DiscreteSystem(const double tstart_, const double dt_)
    : tstart(tstart_), dt(dt_)
{
}

void ssc::solver::DiscreteSystem::schedule_update(const double t, Scheduler &scheduler)
{
    Scheduler::Callback binded_callback
        = std::bind(&DiscreteSystem::callback, this, std::placeholders::_1, std::placeholders::_2);
    scheduler.schedule_discrete_state_update(t, binded_callback);
}

void ssc::solver::DiscreteSystem::callback(Scheduler &scheduler, ContinuousSystem *system)
{
    const double tcurrent = scheduler.get_time();
    update_discrete_states(tcurrent, system);
    const double tnext = get_date_of_next_update(tcurrent);
    schedule_update(tnext, scheduler);
}

void ssc::solver::DiscreteSystem::initialize(Scheduler &scheduler, ContinuousSystem *system)
{
    callback(scheduler, system);
}

double ssc::solver::DiscreteSystem::get_date_of_next_update(const double current_time) const
{
    const int current_iterator = (int)std::round((current_time - tstart) / dt);
    return tstart + (current_iterator + 1) * dt;
}