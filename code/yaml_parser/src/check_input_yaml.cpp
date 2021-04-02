/*
 * check_input_yaml.cpp
 *
 *  Created on: May 23, 2014
 *      Author: cady
 */
#include <algorithm>
#include <sstream>

#include "check_input_yaml.hpp"
#include "InvalidInputException.hpp"

void check_rotations(const YamlRotation& input);
void throw_if_any_errors_were_detected(const std::string& caller, const std::stringstream& ss);
bool is_an_axis(const std::string& name);

void throw_if_any_errors_were_detected(const std::string& caller, const std::stringstream& ss)
{
    const std::string errors = ss.str();
    if (not(errors.empty()))
    {
        THROW(caller, InvalidInputException, errors);
    }
}

bool is_an_axis(const std::string& name)
{
    if ((name == "x") or (name == "x'") or (name == "x''"))
    {
        return true;
    }
    if ((name == "y") or (name == "y'") or (name == "y''"))
    {
        return true;
    }
    if ((name == "z") or (name == "z'") or (name == "z''"))
    {
        return true;
    }
    return false;
}

void check_rotations(const YamlRotation& input)
{
    std::stringstream errors;
    if (input.convention.size() != 3)
    {
        errors << "'rotations/conventions' should be a list of three axes: got " << input.convention.size() << " elements." << std::endl;
    }
    if (not(is_an_axis(input.convention.at(0))))
    {
        errors << "First element of 'rotations/conventions' must be an axis name: got '" << input.convention.at(0) << "'" << std::endl;
    }
    if (not(is_an_axis(input.convention.at(1))))
    {
        errors << "Second element of 'rotations/conventions' must be an axis name: got '" << input.convention.at(1) << "'" << std::endl;
    }
    if (not(is_an_axis(input.convention.at(2))))
    {
        errors << "Third element of 'rotations/conventions' must be an axis name: got '" << input.convention.at(2) << "'" << std::endl;
    }    
    if ((input.order_by != "angle") and (input.order_by != "axis"))
    {
        errors << "'rotations/order by' must be either 'angle' or 'axis': got '" << input.order_by << "'" << std::endl;
    }        
    throw_if_any_errors_were_detected(__PRETTY_FUNCTION__, errors);
}

void check_controller_output_is_not_defined_in_a_command(const std::string& controller_command_name,
                                                         const std::vector<YamlTimeSeries>& commands_input)
{
    std::stringstream errors;

    for (const YamlTimeSeries commands : commands_input)
    {
        for (const auto& command_name_values : commands.values)
        {
            const std::string command_complete_name = commands.name + "(" + command_name_values.first + ")";
            if (controller_command_name == command_complete_name)
            {
                errors << "The controllers output '" << controller_command_name << "' is already defined in the 'commands' section." << std::endl;
            }
        }
    }
    throw_if_any_errors_were_detected(__PRETTY_FUNCTION__, errors);
}

void check_command_names(const std::vector<YamlTimeSeries>& commands_input)
{
    std::stringstream errors;

    for (const YamlTimeSeries commands : commands_input)
    {
        if (commands.name == "")
        {
            errors << "Missing 'name' field in the 'commands' section: all commands should be defined for a force model." << std::endl;
        }
    }
    throw_if_any_errors_were_detected(__PRETTY_FUNCTION__, errors);
}

void check_state_name(const std::string& state_name)
{
    std::stringstream errors;

    const std::vector<std::string> valid_states = { "x", "y", "z",
                                                    "u", "v", "w",
                                                    "p", "q", "r",
                                                    "qr", "qi", "qj", "qk",
                                                    "phi", "theta", "psi" };

    if (std::find(valid_states.begin(), valid_states.end(), state_name) == valid_states.end()) {
        errors << "'" << state_name << "' is not a valid state. The valid states are: 'x', 'y', 'z', 'u', 'v', 'w', 'p', 'q', 'r', 'qr', 'qi', 'qj', 'qk', 'phi', 'theta' and 'psi'." << std::endl;
    }
    throw_if_any_errors_were_detected(__PRETTY_FUNCTION__, errors);
}

YamlSimulatorInput check_input_yaml(const YamlSimulatorInput& input)
{
    check_rotations(input.rotations);
    return input;
}
