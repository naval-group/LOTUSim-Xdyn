#ifndef SIMULATORAPI_HPP_
#define SIMULATORAPI_HPP_

#include <map>
#include <string>

#include <ssc/solver/Scheduler.hpp>
#include <ssc/solver/solve.hpp>

#include "SimulatorYamlParser.hpp"
#include "check_input_yaml.hpp"
#include "EverythingObserver.hpp"
#include "listeners.hpp"
#include "Sim.hpp"
#include "SimulatorBuilder.hpp"
#include "SimObserver.hpp"

struct YamlSimulatorInput;

Sim get_system(const std::string& yaml, const double t0);
Sim get_system(const std::string& yaml, const std::string& mesh, const double t0);
Sim get_system(const std::string& yaml, const std::map<std::string, VectorOfVectorOfPoints>& meshes, const double t0);
Sim get_system(const std::string& yaml, const VectorOfVectorOfPoints& mesh, const double t0);
Sim get_system(const YamlSimulatorInput& input, const double t0);
Sim get_system(const YamlSimulatorInput& input, const std::string& mesh, const double t0);
Sim get_system(const YamlSimulatorInput& input, const VectorOfVectorOfPoints& mesh, const double t0);
Sim get_system(const YamlSimulatorInput& input, const VectorOfVectorOfPoints& mesh, const double t0, ssc::data_source::DataSource& commands);
Sim get_system(const std::string& yaml, const VectorOfVectorOfPoints& mesh, const double t0, ssc::data_source::DataSource& commands);
Sim get_system(const std::string& yaml, const std::string& mesh, const double t0, ssc::data_source::DataSource& commands);
Sim get_system(const YamlSimulatorInput& input, const std::map<std::string, VectorOfVectorOfPoints>& meshes, const double t0);
EnvironmentAndFrames get_environment_for_wave_queries(const std::string& yaml);

typedef std::function<void(std::vector<double>&, const double)> ForceStates;

template <typename StepperType> std::vector<Res> simulate(Sim& sys, const YamlSimulatorInput& input, ssc::solver::Scheduler& scheduler)
{
    EverythingObserver observer;
    const double tstart = scheduler.get_time();
    const auto controllers = get_initialized_controllers(tstart, input.controllers, input.commands, scheduler, sys);
    ssc::solver::quicksolve<StepperType>(sys, scheduler, observer, controllers);
    auto ret = observer.get();
    return ret;
}

template <typename StepperType, typename ObserverType> void simulate(Sim& sys, ssc::solver::Scheduler& scheduler, ObserverType& observer)
{
    ssc::solver::quicksolve<StepperType>(sys, scheduler, observer);
}

template <typename StepperType> std::vector<Res> simulate(const std::string& yaml, ssc::solver::Scheduler& scheduler)
{
    const YamlSimulatorInput input = check_input_yaml(SimulatorYamlParser(yaml).parse());
    const double tstart = scheduler.get_time();
    Sim sys = get_system(yaml, tstart);
    return simulate<StepperType>(sys, input, scheduler);
}

MeshMap make_mesh_map(const YamlSimulatorInput& yaml, const std::string& mesh);

template <typename StepperType> std::vector<Res> simulate(const std::string& yaml, const std::string& mesh, ssc::solver::Scheduler& scheduler)
{
    const YamlSimulatorInput input = check_input_yaml(SimulatorYamlParser(yaml).parse());
    const double tstart = scheduler.get_time();
    Sim sys = get_system(yaml, mesh, tstart);
    return simulate<StepperType>(sys, input, scheduler);
}

template <typename StepperType> std::vector<Res> simulate(const std::string& yaml, const std::string& mesh, ssc::solver::Scheduler& scheduler, ssc::data_source::DataSource& commands)
{
    const YamlSimulatorInput input = check_input_yaml(SimulatorYamlParser(yaml).parse());
    const double tstart = scheduler.get_time();
    Sim sys = get_system(yaml, mesh, tstart, commands);
    return simulate<StepperType>(sys, input, scheduler);
}

template <typename StepperType> std::vector<Res> simulate(const std::string& yaml, const std::map<std::string, VectorOfVectorOfPoints>& meshes, ssc::solver::Scheduler& scheduler)
{
    const YamlSimulatorInput input = check_input_yaml(SimulatorYamlParser(yaml).parse());
    const double tstart = scheduler.get_time();
    Sim sys = get_system(yaml, meshes, tstart);
    return simulate<StepperType>(sys, input, scheduler);
}

template <typename StepperType> std::vector<Res> simulate(const YamlSimulatorInput& input, ssc::solver::Scheduler& scheduler)
{
    const double tstart = scheduler.get_time();
    Sim sys = get_system(input, tstart);
    return simulate<StepperType>(sys, input, scheduler);
}

template <typename StepperType> std::vector<Res> simulate(const YamlSimulatorInput& input, const std::map<std::string, VectorOfVectorOfPoints>& meshes, ssc::solver::Scheduler& scheduler)
{
    const double tstart = scheduler.get_time();
    Sim sys = get_system(input, meshes, tstart);
    return simulate<StepperType>(sys, input, scheduler);
}

template <typename StepperType> std::vector<Res> simulate(const YamlSimulatorInput& input, const VectorOfVectorOfPoints& mesh, ssc::solver::Scheduler& scheduler)
{
    std::map<std::string, VectorOfVectorOfPoints> meshes;
    meshes[input.bodies.front().name] = mesh;
    return simulate<StepperType>(input, meshes, scheduler);
}

template <typename StepperType> std::vector<Res> simulate(const YamlSimulatorInput& input, const VectorOfVectorOfPoints& mesh, ssc::solver::Scheduler& scheduler, ssc::data_source::DataSource& commands)
{
    const double tstart = scheduler.get_time();
    Sim sys = get_system(input, mesh, tstart, commands);
    SimObserver observer;
    const auto controllers = get_initialized_controllers(tstart, input.controllers, input.commands, scheduler, sys);
    ssc::solver::quicksolve<StepperType>(sys, scheduler, observer, controllers);
    return observer.get();
}

#endif
