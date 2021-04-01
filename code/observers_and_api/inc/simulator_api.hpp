#ifndef SIMULATORAPI_HPP_
#define SIMULATORAPI_HPP_

#include <map>
#include <string>

#include "Scheduler.hpp"
#include "solve.hpp"

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

typedef std::function<void(std::vector<double>&, const double)> ForceStates;

template <typename StepperType> std::vector<Res> simulate(Sim& sys, const YamlSimulatorInput& input, const double tstart, const double tend, const double dt)
{
    EverythingObserver observer;
    ssc::solver::Scheduler scheduler(tstart, tend, dt);
    std::vector<PIDController> controllers = get_pid_controllers(input.controllers, input.commands);
    initialize_controllers(controllers, scheduler, &sys);
    ssc::solver::quicksolve<StepperType>(sys, scheduler, observer);
    auto ret = observer.get();
    return ret;
}

template <typename StepperType, typename ObserverType> void simulate(Sim& sys, const double tstart, const double tend, const double dt, ObserverType& observer)
{
    ssc::solver::Scheduler scheduler(tstart, tend, dt);
    /* This is used for cosimulation, whose inputs do not define controllers. Therefore, `add_controllers_callbacks_to_scheduler` is not called. */
    ssc::solver::quicksolve<StepperType>(sys, scheduler, observer);
}

template <typename StepperType> std::vector<Res> simulate(const std::string& yaml, const double tstart, const double tend, const double dt)
{
    const YamlSimulatorInput input = check_input_yaml(SimulatorYamlParser(yaml).parse());
    Sim sys = get_system(yaml, tstart);
    return simulate<StepperType>(sys, input, tstart, tend, dt);
}

MeshMap make_mesh_map(const YamlSimulatorInput& yaml, const std::string& mesh);

template <typename StepperType> std::vector<Res> simulate(const std::string& yaml, const std::string& mesh, const double tstart, const double tend, const double dt)
{
    const YamlSimulatorInput input = check_input_yaml(SimulatorYamlParser(yaml).parse());
    Sim sys = get_system(yaml, mesh, tstart);
    return simulate<StepperType>(sys, input, tstart, tend, dt);
}

template <typename StepperType> std::vector<Res> simulate(const std::string& yaml, const std::string& mesh, const double tstart, const double tend, const double dt, ssc::data_source::DataSource& commands)
{
    const YamlSimulatorInput input = check_input_yaml(SimulatorYamlParser(yaml).parse());
    Sim sys = get_system(yaml, mesh, tstart, commands);
    return simulate<StepperType>(sys, input, tstart, tend, dt);
}

template <typename StepperType> std::vector<Res> simulate(const std::string& yaml, const std::map<std::string, VectorOfVectorOfPoints>& meshes, const double tstart, const double tend, const double dt)
{
    const YamlSimulatorInput input = check_input_yaml(SimulatorYamlParser(yaml).parse());
    Sim sys = get_system(yaml, meshes, tstart);
    return simulate<StepperType>(sys, input, tstart, tend, dt);
}

template <typename StepperType> std::vector<Res> simulate(const YamlSimulatorInput& input, const double tstart, const double tend, const double dt)
{
    Sim sys = get_system(input, tstart);
    return simulate<StepperType>(sys, input, tstart, tend, dt);
}

template <typename StepperType> std::vector<Res> simulate(const YamlSimulatorInput& input, const std::map<std::string, VectorOfVectorOfPoints>& meshes, const double tstart, const double tend, const double dt)
{
    Sim sys = get_system(input, meshes, tstart);
    return simulate<StepperType>(sys, input, tstart, tend, dt);
}

template <typename StepperType> std::vector<Res> simulate(const YamlSimulatorInput& input, const VectorOfVectorOfPoints& mesh, const double tstart, const double tend, const double dt)
{
    std::map<std::string, VectorOfVectorOfPoints> meshes;
    meshes[input.bodies.front().name] = mesh;
    return simulate<StepperType>(input, meshes, tstart, tend, dt);
}

template <typename StepperType> std::vector<Res> simulate(const YamlSimulatorInput& input, const VectorOfVectorOfPoints& mesh, const double tstart, const double tend, const double dt, ssc::data_source::DataSource& commands)
{
    Sim sys = get_system(input, mesh, tstart, commands);
    ssc::solver::Scheduler scheduler(tstart, tend, dt);
    SimObserver observer;
    std::vector<PIDController> controllers = get_pid_controllers(input.controllers, input.commands);
    initialize_controllers(controllers, scheduler, &sys);
    ssc::solver::quicksolve<StepperType>(sys, scheduler, observer);
    return observer.get();
}

#endif
