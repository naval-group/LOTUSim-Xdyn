#include "build_observers_description.hpp"
#include "YamlOutput.hpp"
#include "parse_output.hpp"
#include "XdynCommandLineArguments.hpp"
#include "Sim.hpp"
#include "stl_io_hdf5.hpp"
#include "h5_tools.hpp"

YamlOutput create_wave_observer_description(const XdynCommandLineArguments& input_data);
YamlOutput create_wave_observer_description(const XdynCommandLineArguments& input_data)
{
    YamlOutput o;
    o.data = {"waves"};
    o.filename = input_data.wave_output;
    o.format = get_format_for_wave_observer(input_data.wave_output);
    return o;
}

void add_wave_spectra(ObserverPtr& observer, const Sim& sys)
{
    const auto env = sys.get_env();
    const auto w = env.w;
    if (w)
    {
        w->serialize_wave_spectra_before_simulation(observer);
    }
}

std::string serialize_command(const XdynCommandLineArguments& inputData);
std::string serialize_command(const XdynCommandLineArguments& inputData)
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

void send_context_to_observer(const ObserverPtr& observer, const Sim& sys, const std::string& yaml_input, const XdynCommandLineArguments& input_data);
void send_context_to_observer(const ObserverPtr& observer, const Sim& sys, const std::string& yaml_input, const XdynCommandLineArguments& input_data)
{
    auto prog_command = serialize_command(input_data);
    observer->write_before_simulation(prog_command, DataAddressing({"command"}, "CLI command"));
    observer->write_before_simulation(yaml_input, DataAddressing({"yaml","input"}, "YAML input"));
    for (const auto& bodies : sys.get_bodies())
    {
        const auto& states = bodies->get_states();
        const auto& name = states.name;
        const auto& mesh = states.mesh;
        observer->write_before_simulation(mesh, DataAddressing({"meshes",name}, "mesh("+name+")"));
    }
}

void add_main_observer_from_cli(const XdynCommandLineArguments& input_data, std::vector<YamlOutput>& out,const std::string& simulator_input,const Sim& sys);
void add_main_observer_from_cli(
        const XdynCommandLineArguments& input_data,
        ListOfObservers& out,
        const std::string& simulator_input,
        const Sim& sys)
{
    YamlOutput CLI_observer_description = build_YamlOutput_from_filename(input_data.output_filename);
    CLI_observer_description.full_output = true;
    if ((CLI_observer_description.format=="ws") or (CLI_observer_description.format=="hdf5") or (CLI_observer_description.format=="json"))
    {
        CLI_observer_description.data.push_back("waves");
    }
    auto CLI_observer = ListOfObservers::parse_observer(CLI_observer_description);
    send_context_to_observer(CLI_observer, sys, simulator_input, input_data);
    add_wave_spectra(CLI_observer, sys);
    out.add_observer(CLI_observer);
}

void add_observers_from_cli(
        const XdynCommandLineArguments& input_data,
        ListOfObservers& out,
        const std::string& simulator_input,
        const Sim& sys)
{
    if (not(input_data.output_filename.empty()))
    {
        add_main_observer_from_cli(input_data, out, simulator_input, sys);
    }
    if (not(input_data.wave_output.empty()))
    {
        auto wave_observer = ListOfObservers::parse_observer(create_wave_observer_description(input_data));
        add_wave_spectra(wave_observer, sys);
        out.add_observer(wave_observer);
    }
}

std::vector<YamlOutput> build_observers_description(const std::string& yaml)
{
    auto out = parse_output(yaml);
    return out;
}
