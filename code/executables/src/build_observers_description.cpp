#include "build_observers_description.hpp"
#include "YamlOutput.hpp"
#include "parse_output.hpp"
#include "XdynCommandLineArguments.hpp"
#include "Sim.hpp"
#include "stl_io_hdf5.hpp"
#include "h5_tools.hpp"

YamlOutput create_a_wave_observer(const XdynCommandLineArguments& input_data);
YamlOutput create_a_wave_observer(const XdynCommandLineArguments& input_data)
{
    YamlOutput o;
    o.data = {"waves"};
    o.filename = input_data.wave_output;
    o.format = get_format_for_wave_observer(input_data.wave_output);
    return o;
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

void serialize_context_if_necessary(YamlOutput& observer, const Sim& sys, const std::string& yaml_input, const std::string& prog_command);
void serialize_context_if_necessary(YamlOutput& observer, const Sim& sys, const std::string& yaml_input, const std::string& prog_command)
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

void add_observers_from_cli_with_output_filename(const XdynCommandLineArguments& input_data, std::vector<YamlOutput>& out,const std::string& simulator_input,const Sim& sys);
void add_observers_from_cli_with_output_filename(
        const XdynCommandLineArguments& input_data,
        ListOfObservers& out,
        const std::string& simulator_input,
        const Sim& sys)
{
    YamlOutput outputterCli = build_YamlOutput_from_filename(input_data.output_filename);
    outputterCli.full_output = true;
    if(outputterCli.format=="hdf5")
    {
        serialize_context_if_necessary(outputterCli, sys, simulator_input, input_data_serialize(input_data));
    }
    if (not(input_data.wave_output.empty()))
    {
        if ((outputterCli.format=="ws") or (outputterCli.format=="hdf5") or (outputterCli.format=="json"))
        {
            outputterCli.data.push_back("waves");
        }
        else
        {
            out.add_observer(ListOfObservers::parse_observer(create_a_wave_observer(input_data)));
        }
    }
    out.add_observer(ListOfObservers::parse_observer(outputterCli));
}

void add_observers_from_cli(
        const XdynCommandLineArguments& input_data,
        ListOfObservers& out,
        const std::string& simulator_input,
        const Sim& sys)
{
    if (not(input_data.output_filename.empty()))
    {
        add_observers_from_cli_with_output_filename(input_data, out, simulator_input, sys);
    }
    else if (not(input_data.wave_output.empty()))
    {
        out.add_observer(ListOfObservers::parse_observer(create_a_wave_observer(input_data)));
    }
    serialize_context_if_necessary_new(out, sys);
}

std::vector<YamlOutput> build_observers_description(const std::string& yaml)
{
    auto out = parse_output(yaml);
    return out;
}

//ListOfObservers get_observers(const std::string& yaml, const XdynCommandLineArguments& input_data)
//{
//    auto out = parse_output(yaml);
//    add_observers_from_cli(yaml, input_data, out);
//    return ListOfObservers(out);
//}
