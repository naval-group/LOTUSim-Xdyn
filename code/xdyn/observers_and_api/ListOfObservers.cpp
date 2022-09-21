/*
 * ListOfObservers.cpp
 *
 *  Created on: Jan 12, 2015
 *      Author: cady
 */

#include "ListOfObservers.hpp"
#include "CsvObserver.hpp"
#include "TsvObserver.hpp"
#include "JsonObserver.hpp"
#include "MapObserver.hpp"
#include "Hdf5Observer.hpp"
#include "WebSocketObserver.hpp"
#include "xdyn/external_data_structures/YamlOutput.hpp"

ObserverPtr ListOfObservers::parse_observer(const YamlOutput& output)
{
    ObserverPtr obs;
    if (output.full_output)
    {
        if (output.format == "csv")  obs.reset(new CsvObserver(output.filename));
        if (output.format == "h5")   obs.reset(new Hdf5Observer(output.filename));
        if (output.format == "hdf5") obs.reset(new Hdf5Observer(output.filename));
        if (output.format == "tsv")  obs.reset(new TsvObserver(output.filename));
        if (output.format == "map")  obs.reset(new MapObserver());
        if (output.format == "json") obs.reset(new JsonObserver(output.filename));
        if (output.format == "ws")   obs.reset(new WebSocketObserver(output.address,output.port));
    }
    else
    {
        if (output.format == "csv")  obs.reset(new CsvObserver(output.filename,output.data));
        if (output.format == "h5")   obs.reset(new Hdf5Observer(output.filename,output.data));
        if (output.format == "hdf5") obs.reset(new Hdf5Observer(output.filename,output.data));
        if (output.format == "tsv")  obs.reset(new TsvObserver(output.filename,output.data));
        if (output.format == "map")  obs.reset(new MapObserver(output.data));
        if (output.format == "json") obs.reset(new JsonObserver(output.filename,output.data));
        if (output.format == "ws")   obs.reset(new WebSocketObserver(output.address,output.port,output.data));
    }
    return obs;
}

ListOfObservers::ListOfObservers(const std::vector<YamlOutput>& yaml) : observers()
{
    for (auto output:yaml)
    {
        observers.push_back(parse_observer(output));
    }
}

ListOfObservers::ListOfObservers(const std::vector<ObserverPtr>& observers_) : observers(observers_)
{
}

void ListOfObservers::check_variables_to_serialize_are_available() const
{
    for (auto observer:observers)
    {
        observer->check_variables_to_serialize_are_available();
    }
}

void ListOfObservers::observe_before_solver_step(const Sim& sys, const double t, const std::vector<std::shared_ptr<ssc::solver::DiscreteSystem> >& discrete_systems)
{
    for (auto observer:observers)
    {
        observer->observe_before_solver_step(sys,t, discrete_systems);
    }
}

void ListOfObservers::observe_after_solver_step(const Sim& sys, const double t, const std::vector<std::shared_ptr<ssc::solver::DiscreteSystem> >& discrete_systems)
{
    for (auto observer:observers)
    {
        observer->observe_after_solver_step(sys,t, discrete_systems);
    }
}

void ListOfObservers::collect_available_serializations(const Sim& sys, const double t, const std::vector<std::shared_ptr<ssc::solver::DiscreteSystem> >& discrete_systems)
{
    for (auto observer:observers)
    {
        observer->collect_available_serializations(sys,t, discrete_systems);
    }
}

void ListOfObservers::flush()
{
    for (auto observer:observers)
    {
        observer->flush();
    }
}

std::vector<ObserverPtr> ListOfObservers::get() const
{
    return observers;
}

bool ListOfObservers::empty() const
{
    return observers.empty();
}

void ListOfObservers::add_observer(const ObserverPtr& observer)
{
    observers.push_back(observer);
}
