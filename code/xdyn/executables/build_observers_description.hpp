#ifndef UTILITIES_FOR_SIMULATOR_HPP_
#define UTILITIES_FOR_SIMULATOR_HPP_

#include "xdyn/external_data_structures/YamlOutput.hpp"
#include "observers_and_api/ListOfObservers.hpp"
#include <string>
#include <vector>

struct XdynCommandLineArguments;

std::vector<YamlOutput> build_observers_description(const std::string& yaml);

void add_observers_from_cli(const XdynCommandLineArguments& input_data, ListOfObservers& out);
void add_wave_spectra(ObserverPtr& observer, const Sim& sys);
std::string serialize_command(const XdynCommandLineArguments& inputData);

#endif
