/*
 * simulator.cpp
 *
 *  Created on: 17 avr. 2014
 *      Author: cady
 */

#include <exception>
#include <functional>
#include <iostream>
#include <google/protobuf/stubs/common.h>
#include "yaml-cpp/exceptions.h"

#include <ssc/check_ssc_version.hpp>
#include <ssc/text_file_reader.hpp>

#include <ssc/exception_handling.hpp>

#include "build_observers_description.hpp"
#include "ConnexionError.hpp"
#include "InternalErrorException.hpp"
#include "listeners.hpp"
#include "MeshException.hpp"
#include "NumericalErrorException.hpp"
#include "parse_XdynCommandLineArguments.hpp"
#include "simulator_api.hpp"
#include "SurfaceElevationInterface.hpp"
#include "XdynCommandLineArguments.hpp"

#include <solve.hpp>
#include <steppers.hpp>
#include <Scheduler.hpp>
#include "ErrorReporter.hpp"

CHECK_SSC_VERSION(8,0)

void solve(const std::string& solver_name, Sim& sys, ssc::solver::Scheduler& scheduler, ListOfObservers& observer);
void solve(const std::string& solver_name, Sim& sys, ssc::solver::Scheduler& scheduler, ListOfObservers& observer)
{
    if (solver_name=="euler")
    {
        ssc::solver::quicksolve<ssc::solver::EulerStepper>(sys, scheduler, observer);
    }
    else if (solver_name=="rk4")
    {
        ssc::solver::quicksolve<ssc::solver::RK4Stepper>(sys, scheduler, observer);
    }
    else if (solver_name=="rkck")
    {
        ssc::solver::quicksolve<ssc::solver::RKCK>(sys, scheduler, observer);
    }
    else
    {
        ssc::solver::quicksolve<ssc::solver::EulerStepper>(sys, scheduler, observer);
    }
}

void serialize_context_if_necessary_new(ListOfObservers& observers, const Sim& sys);
void serialize_context_if_necessary_new(ListOfObservers& observers, const Sim& sys)
{
    const auto env = sys.get_env();
    const auto w = env.w;
    if (w)
    {
        for (auto observer:observers.get())
        {
            w->serialize_wave_spectra_before_simulation(observer);
        }
    }
}

#include "stl_io_hdf5.hpp"
#include "h5_tools.hpp"
void serialize_context_if_necessary(std::vector<YamlOutput>& observers, const Sim& sys, const std::string& yaml_input, const std::string& prog_command);
void serialize_context_if_necessary(std::vector<YamlOutput>& observers, const Sim& sys, const std::string& yaml_input, const std::string& prog_command)
{
    for (const auto observer:observers)
    {
        if(observer.format=="hdf5")
        {
            if (not(prog_command.empty()))
            {
                H5_Tools::write(observer.filename, "/inputs/command", prog_command);
            }
            if (not(yaml_input.empty()))
            {
                H5_Tools::write(observer.filename, "/inputs/yaml/input", yaml_input);
            }

            for (const auto& bodies : sys.get_bodies())
            {
                const auto& states = bodies->get_states();
                const auto& name = states.name;
                const auto& mesh = states.mesh;
                if (mesh->nb_of_static_nodes>0)
                {
                    writeMeshToHdf5File(observer.filename,
                                        "/inputs/meshes/"+name,
                                        mesh->nodes,
                                        mesh->facets);
                }
            }
        }
    }
}

std::string input_data_serialize(const XdynCommandLineArguments& inputData);
std::string input_data_serialize(const XdynCommandLineArguments& inputData)
{
    std::stringstream s;
    s << "xdyn ";
    if (not inputData.yaml_filenames.empty()) s << "-y ";
    for (const auto& f:inputData.yaml_filenames)
    {
        s << f << " ";
    }
    s << " --tstart " << inputData.tstart<<" ";
    s << " --tend " << inputData.tend<<" ";
    s << " --dt " << inputData.initial_timestep<<" ";
    s << " --solver "<<inputData.solver;
    if (not(inputData.output_filename.empty()))
    {
        s << " -o " << inputData.output_filename;
    }
    if (not(inputData.wave_output.empty()))
    {
        s << " -w " << inputData.wave_output;
    }
    return s.str();
}

void run_simulation(const XdynCommandLineArguments& input_data, ErrorReporter& error_outputter);
void run_simulation(const XdynCommandLineArguments& input_data, ErrorReporter& error_outputter)
{
    const auto f = [input_data](){
    {
        const auto yaml_input = ssc::text_file_reader::TextFileReader(input_data.yaml_filenames).get_contents();
        const auto input = SimulatorYamlParser(yaml_input).parse();

        auto sys = get_system(input, input_data.tstart);

        ssc::solver::Scheduler scheduler(input_data.tstart, input_data.tend, input_data.initial_timestep);
        std::vector<PIDController> controllers = get_pid_controllers(input.controllers, input.commands);
        initialize_controllers(controllers, scheduler, &sys);

        auto observers_description = build_observers_description(yaml_input, input_data);
        ListOfObservers observers(observers_description);

        serialize_context_if_necessary(observers_description, sys, yaml_input, input_data_serialize(input_data));
        serialize_context_if_necessary_new(observers, sys);

        solve(input_data.solver, sys, scheduler, observers);
    }};
    if (input_data.catch_exceptions)
    {
        error_outputter.run_and_report_errors(f);
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
        return -1;
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
            return -1;
        }
    }
    const auto ret = run(input_data, error_outputter);
    google::protobuf::ShutdownProtobufLibrary();
    return ret;
}
