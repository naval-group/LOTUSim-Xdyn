#include "build_observers_description.hpp"
#include "YamlOutput.hpp"
#include "parse_output.hpp"
#include "XdynCommandLineArguments.hpp"

YamlOutput create_a_wave_observer(
        const XdynCommandLineArguments& input_data);

void add_observers_from_cli_with_output_filename(
        const XdynCommandLineArguments& input_data,
        std::vector<YamlOutput>& out);

YamlOutput create_a_wave_observer(const XdynCommandLineArguments& input_data)
{
    YamlOutput o;
    o.data = {"waves"};
    o.filename = input_data.wave_output;
    o.format = get_format_for_wave_observer(input_data.wave_output);
    return o;
}

void add_observers_from_cli_with_output_filename(
        const XdynCommandLineArguments& input_data,
        ListOfObservers& out)
{
    YamlOutput outputterCli = build_YamlOutput_from_filename(input_data.output_filename);
    outputterCli.full_output = true;
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
        ListOfObservers& out)
{
    if (not(input_data.output_filename.empty()))
    {
        add_observers_from_cli_with_output_filename(input_data, out);
    }
    else if (not(input_data.wave_output.empty()))
    {
        out.add_observer(ListOfObservers::parse_observer(create_a_wave_observer(input_data)));
    }
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
