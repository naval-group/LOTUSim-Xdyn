#include "py_xdyn_exe.hpp"
#include "py_pybind_additions.hpp"

#include "xdyn/executables/ErrorReporter.hpp"
#include "xdyn/executables/XdynCommandLineArguments.hpp"
#include "xdyn/executables/parse_XdynCommandLineArguments.hpp"
#include "xdyn/executables/build_observers_description.hpp"
#include "observers_and_api/simulator_api.hpp"
#include "ssc/ssc/text_file_reader.hpp"
#include "ssc/ssc/solver/solve.hpp"
#include "ssc/ssc/solver/steppers.hpp"
#include "ssc/ssc/solver/Scheduler.hpp"

#include <exception>
#include <functional>
#include <iostream>
#include <vector>

namespace py = pybind11;


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

bool run(const XdynCommandLineArguments& input_data, ErrorReporter& error_outputter);
bool run(const XdynCommandLineArguments& input_data, ErrorReporter& error_outputter)
{
    if (not(input_data.empty())) run_simulation(input_data, error_outputter);
    if (error_outputter.contains_errors())
    {
        return false;
    }
    return true;
}

void py_add_module_xdyn_exe(py::module& m);
void py_add_module_xdyn_exe(py::module& m)
{
    py::enum_<ErrorReporter::Status>(m, "ErrorReporterStatus", py::arithmetic())
        .value("OK", ErrorReporter::Status::OK)
        .value("INVALID_INPUT", ErrorReporter::Status::INVALID_INPUT)
        .value("SIMULATION_ERROR", ErrorReporter::Status::SIMULATION_ERROR)
        .value("INTERNAL_ERROR", ErrorReporter::Status::INTERNAL_ERROR)
        .value("NETWORK_ERROR", ErrorReporter::Status::NETWORK_ERROR)
        ;

    py::class_<ErrorReporter>(m, "ErrorReporter")
        .def(py::init<>())
        .def("invalid_request", &ErrorReporter::invalid_request, py::arg("function"), py::arg("line"))
        .def("empty_history", &ErrorReporter::empty_history)
        .def("invalid_state_size", &ErrorReporter::invalid_state_size, py::arg("state_name"), py::arg("state_size"), py::arg("t_size"))
        .def("simulation_error", &ErrorReporter::simulation_error, py::arg("error_message"))
        .def("invalid_command_line", &ErrorReporter::invalid_command_line, py::arg("error_message"))
        .def("invalid_input", &ErrorReporter::invalid_input, py::arg("error_message"))
        .def("internal_error", &ErrorReporter::internal_error, py::arg("error_message"))
        .def("network_error", &ErrorReporter::network_error, py::arg("error_message"))
        .def("run_and_report_errors_with_yaml_dump", &ErrorReporter::run_and_report_errors_with_yaml_dump, py::arg("f"), py::arg("yaml_dump"))
        .def("run_and_report_errors_without_yaml_dump", &ErrorReporter::run_and_report_errors_without_yaml_dump, py::arg("f"))
        .def("contains_errors", &ErrorReporter::contains_errors)
        .def("get_message", &ErrorReporter::get_message)
        .def("get_status", &ErrorReporter::get_status)
        .def("reset", &ErrorReporter::reset)
        ;

    py::class_<XdynCommandLineArguments>(m, "XdynCommandLineArguments")
        .def(py::init<>())
        .def_readwrite("yaml_filenames",&XdynCommandLineArguments::yaml_filenames, "List of YAML filenames to use")
        .def_readwrite("solver", &XdynCommandLineArguments::solver, "Solver name")
        .def_readwrite("wave_output", &XdynCommandLineArguments::wave_output, "Wave output")
        .def_readwrite("initial_timestep"  , &XdynCommandLineArguments::initial_timestep, "Initial timestep")
        .def_readwrite("tstart", &XdynCommandLineArguments::tstart, "Start time")
        .def_readwrite("tend", &XdynCommandLineArguments::tend, "End time")
        .def_readwrite("catch_exceptions", &XdynCommandLineArguments::catch_exceptions)
        .def("empty", &XdynCommandLineArguments::empty)
        ;

    m.def("run", &run,
        py::arg("input_data"),
        py::arg("error_outputter"),
        "Run a XDyn simulation from Python");
}
