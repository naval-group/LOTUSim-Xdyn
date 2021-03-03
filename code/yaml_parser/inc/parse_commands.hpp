/*
 * parse_commands.hpp
 *
 *  Created on: Oct 22, 2014
 *      Author: cady
 */

#ifndef PARSE_COMMANDS_HPP_
#define PARSE_COMMANDS_HPP_

#include "YamlCommands.hpp"
#include "yaml.h"

/**  \brief Parses the commands for use by controlled forces.
  *  \snippet simulator_yaml_parser/unit_tests/src/parse_commandsTest.cpp parse_commandsTest example
  */
std::vector<YamlCommands> parse_command_yaml(const std::string& yaml //!< YAML data
                                            );

/**  \brief Parses the inputs for use by the controllers (for controlled forces).
  *  \snippet simulator_yaml_parser/unit_tests/src/parse_inputsTest.cpp parse_inputsTest example
  */
std::vector<YamlCommands> parse_input_yaml(const std::string& yaml //!< YAML data
                                           );

void operator >> (const YAML::Node& node, YamlCommands& c);

#endif /* PARSE_COMMANDS_HPP_ */
