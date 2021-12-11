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
    o.data = {"waves", "spectra"};
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

void add_observer_from_cli_dash_o_option(const XdynCommandLineArguments& input_data, ListOfObservers& out);
void add_observer_from_cli_dash_o_option(
        const XdynCommandLineArguments& input_data,
        ListOfObservers& out)
{
    YamlOutput description = build_YamlOutput_from_filename(input_data.output_filename);
    description.full_output = true;
    description.data.push_back("waves");
    description.data.push_back("matlab scripts");
    description.data.push_back("python scripts");
    description.data.push_back("spectra");
    description.data.push_back("mesh");
    description.data.push_back("yaml");
    description.data.push_back("command line");
    auto observer = ListOfObservers::parse_observer(description);
    out.add_observer(observer);
}

void add_observers_from_cli(
        const XdynCommandLineArguments& input_data,
        ListOfObservers& out)
{
    if (not(input_data.output_filename.empty()))
    {
        add_observer_from_cli_dash_o_option(input_data, out);
    }
    if (not(input_data.wave_output.empty()))
    {
        auto wave_observer = ListOfObservers::parse_observer(create_wave_observer_description(input_data));
        out.add_observer(wave_observer);
    }
}

std::vector<YamlOutput> build_observers_description(const std::string& yaml)
{
    auto out = parse_output(yaml);
    return out;
}
