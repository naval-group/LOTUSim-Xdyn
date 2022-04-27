/*
 * SimObserver.cpp
 *
 *  Created on: Jun 17, 2014
 *      Author: cady
 */

#include "SimObserver.hpp"
#include "Sim.hpp"

SimObserver::SimObserver() : res(std::vector<Res>())
{
}

void SimObserver::check_variables_to_serialize_are_available() const
{}

void SimObserver::observe_before_solver_step(const Sim& , const double , const std::vector<std::shared_ptr<ssc::solver::DiscreteSystem> >& )
{
}

void SimObserver::observe_after_solver_step(const Sim& s, const double t, const std::vector<std::shared_ptr<ssc::solver::DiscreteSystem> >& )
{
    res.push_back(Res(t,s.state));
}

void SimObserver::collect_available_serializations(const Sim& , const double , const std::vector<std::shared_ptr<ssc::solver::DiscreteSystem> >& )
{
}

std::vector<Res> SimObserver::get() const
{
    return res;
}

void SimObserver::flush()
{

}