/*
 * check_input_yaml.hpp
 *
 *  Created on: 15 avr. 2014
 *      Author: cady
 */

#ifndef CHECK_INPUT_YAML_HPP_
#define CHECK_INPUT_YAML_HPP_

#include "YamlSimulatorInput.hpp"

YamlSimulatorInput check_input_yaml(const YamlSimulatorInput& input);
void check_controller_output_is_not_defined_in_a_command(const std::string& controller_command_name,
                                                         const std::vector<YamlTimeSeries>& commands_input);
void check_command_names(const std::vector<YamlTimeSeries>& commands_input);
void check_state_name(const std::string& state_name);
void check_for_duplicated_controller_names(const std::vector<YamlController>& controller_yaml);

#endif /* CHECK_INPUT_YAML_HPP_ */
