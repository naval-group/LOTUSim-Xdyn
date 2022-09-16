/*
 * xdyn.cpp
 *
 *  Created on: 17 avr. 2014
 *      Author: cady
 */

#include <exception>
#include <functional>
#include <iostream>
#include <google/protobuf/stubs/common.h>
#include <ssc/text_file_reader.hpp>

#include "build_observers_description.hpp"
#include "parse_XdynCommandLineArguments.hpp"
#include "observers_and_api/simulator_api.hpp"
#include "XdynCommandLineArguments.hpp"

#include <ssc/solver/solve.hpp>
#include <ssc/solver/steppers.hpp>
#include <ssc/solver/Scheduler.hpp>
#include "ErrorReporter.hpp"

void solve(const std::string& solver_name, Sim& sys, ssc::solver::Scheduler& scheduler, ListOfObservers& observer, const std::vector<ssc::solver::DiscreteSystemPtr>& controllers);
void solve(const std::string& solver_name, Sim& sys, ssc::solver::Scheduler& scheduler, ListOfObservers& observer, const std::vector<ssc::solver::DiscreteSystemPtr>& controllers)
{
    if (solver_name=="euler")
    {
        ssc::solver::quicksolve<ssc::solver::EulerStepper>(sys, scheduler, observer, controllers);
    }
    else if (solver_name=="rk4")
    {
        ssc::solver::quicksolve<ssc::solver::RK4Stepper>(sys, scheduler, observer, controllers);
    }
    else if (solver_name=="rkck")
    {
        ssc::solver::quicksolve<ssc::solver::RKCK>(sys, scheduler, observer, controllers);
    }
    else
    {
        ssc::solver::quicksolve<ssc::solver::EulerStepper>(sys, scheduler, observer, controllers);
    }
}

void write_before_simulation(ListOfObservers& observers, const Sim& sys, const XdynCommandLineArguments& input_data, const std::string& yaml_input);
void write_before_simulation(ListOfObservers& observers, const Sim& sys, const XdynCommandLineArguments& input_data, const std::string& yaml_input)
{
    const auto obs = observers.get();
    for (auto o:obs)
    {
        add_wave_spectra(o, sys);
        o->write_command_line_before_simulation(serialize_command(input_data));
        o->write_matlab_script_before_simulation();
        o->write_python_script_before_simulation();
        for (const auto& bodies : sys.get_bodies())
        {
            const auto& states = bodies->get_states();
            o->write_before_simulation(states.mesh, DataAddressing({"meshes",states.name}, "mesh("+states.name+")"));
        }
        o->write_yaml_before_simulation(yaml_input);
    };
}

void run_simulation(const XdynCommandLineArguments& input_data, ErrorReporter& error_outputter);
void run_simulation(const XdynCommandLineArguments& input_data, ErrorReporter& error_outputter)
{
    std::string yaml_input;
    error_outputter.run_and_report_errors_without_yaml_dump([&yaml_input,&input_data](){yaml_input = ssc::text_file_reader::TextFileReader(input_data.yaml_filenames).get_contents();});
    const auto f = [input_data, yaml_input](){
    {
        const auto input = SimulatorYamlParser(yaml_input).parse();
        auto sys = get_system(input, input_data.tstart);
        ssc::solver::Scheduler scheduler(input_data.tstart, input_data.tend, input_data.initial_timestep);
        const auto controllers = get_initialized_controllers(input_data.tstart, input.controllers, input.commands, scheduler, sys);
        auto observers_description = build_observers_description(yaml_input);
        ListOfObservers observers(observers_description);
        add_observers_from_cli(input_data, observers);
        write_before_simulation(observers, sys, input_data, yaml_input);
        solve(input_data.solver, sys, scheduler, observers, controllers);
    }};
    if (input_data.catch_exceptions)
    {
        error_outputter.run_and_report_errors_with_yaml_dump(f, yaml_input);
    }
    else
    {
        f();
    }
}

int run(const XdynCommandLineArguments& input_data, ErrorReporter& error_outputter);
int run(const XdynCommandLineArguments& input_data, ErrorReporter& error_outputter)
{
    if (not(input_data.empty())) run_simulation(input_data, error_outputter);
    if (error_outputter.contains_errors())
    {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int main(int argc, char** argv)
{
    XdynCommandLineArguments input_data;
    int error = 0;
    ErrorReporter error_outputter;
    try
    {
        if (argc==1) return fill_input_or_display_help(argv[0], input_data);
        error = parse_command_line_for_xdyn(argc, argv, input_data);
    }
    catch(boost::program_options::error& e)
    {
        error_outputter.invalid_command_line(e.what());
        return EXIT_FAILURE;
    }
    if (error)
    {
        return error;
    }
    if (input_data.catch_exceptions)
    {
        try
        {
            return run(input_data, error_outputter);
        }
        catch (const std::exception& e)
        {
            error_outputter.internal_error(e.what());
            if (error_outputter.contains_errors())
            {
                std::cerr << error_outputter.get_message() << std::endl;
            }
            return EXIT_FAILURE;
        }
    }
    const auto ret = run(input_data, error_outputter);
    google::protobuf::ShutdownProtobufLibrary();
    return ret;
}
