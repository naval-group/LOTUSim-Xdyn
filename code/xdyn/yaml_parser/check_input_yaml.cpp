/*
 * check_input_yaml.cpp
 *
 *  Created on: May 23, 2014
 *      Author: cady
 */
#include "check_input_yaml.hpp"
#include "xdyn/exceptions/InvalidInputException.hpp"
#include <algorithm>
#include <map>
#include <unordered_set>
#include <sstream>

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

    for (const YamlTimeSeries& commands : commands_input)
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

    for (const YamlTimeSeries& commands : commands_input)
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
    check_for_duplicated_controller_names(input.controllers);
    check_for_mesh_declaration_if_needed_by_force_models(input.bodies);
    check_for_redundant_models(input.bodies);
    return input;
}

void check_for_duplicated_controller_names(const std::vector<YamlController>& controller_yaml)
{
    std::unordered_set<std::string> names;
    for (const auto& controller:controller_yaml)
    {
        if (names.count(controller.name))
        {
            THROW(__PRETTY_FUNCTION__, InvalidInputException, "Controller name '" << controller.name << "' is defined twice in the YAML: each controller should have a unique name.");
        }
        names.insert(controller.name);
    }
}


void _raise_invalid_input_exception(const std::string& model_name, const std::string& model_name_requesting_mesh);
void _raise_invalid_input_exception(const std::string& model_name, const std::string& model_name_requesting_mesh)
{
    if (model_name == model_name_requesting_mesh)
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException,
            "Attempting to evaluate forces with model '" << model_name_requesting_mesh << \
            "' that requires a mesh. Please, declare a mesh to use this model.");
    }
}

void check_for_mesh_declaration_if_needed_by_force_models(const std::vector<YamlBody>& bodies)
{
    for (const auto& body: bodies)
    {
        if (body.mesh.empty())
        {
            for (const auto& external_force: body.external_forces)
            {
                _raise_invalid_input_exception(external_force.model, "non-linear hydrostatic (fast)");
                _raise_invalid_input_exception(external_force.model, "non-linear hydrostatic (exact)");
            }
        }
    }
}

void check_for_redundant_models(const std::vector<YamlBody>& bodies)
{
    const std::unordered_set<std::string> hydrostatic_models =
        {"linear hydrostatics",
         "non-linear hydrostatic (fast)",
         "non-linear hydrostatic (exact)"};
    for (const auto& body: bodies)
    {
        std::map<std::string, size_t> counter;
        for (const auto& external_force: body.external_forces)
        {
            auto search = hydrostatic_models.find(external_force.model);
            if (search != hydrostatic_models.end())
            {
                // Found
                if (counter.find(external_force.model)!= counter.end())
                {
                    // Already existing
                    counter[external_force.model]++;
                }
                else
                {
                    // Add a new entry in counter map with value 1
                    counter.insert(std::pair<std::string, size_t>{external_force.model, 1});
                }
            }
        }
        if (counter.size()>1)
        {
            std::stringstream ss;
            for (const auto& model: counter){ss << model.first <<" "<<std::endl;}
            THROW(__PRETTY_FUNCTION__, InvalidInputException,
                "Several hydrostatic models have been declared" + ss.str());
        }
        for (const auto& model: counter)
        {
            if (model.second > 1)
            {
                THROW(__PRETTY_FUNCTION__, InvalidInputException,
                    "Hydrostatic model \'"<< model.first << "\' has been declared " << std::to_string(model.second));
            }
        }
    }
}
