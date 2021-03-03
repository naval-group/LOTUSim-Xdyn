/*
 * parse_controllers.hpp
 *
 *  Created on: Fev 10, 2021
 *      Author: lincker
 */

#ifndef PARSE_CONTROLLERS_HPP_
#define PARSE_CONTROLLERS_HPP_

#include "YamlController.hpp"
#include "yaml.h"

/**  \brief Parses the controllers for use by controlled forces.
  *  \snippet simulator_yaml_parser/unit_tests/src/parse_controllersTest.cpp parse_controllersTest example
  */
std::vector<YamlController> parse_controller_yaml(const std::string& yaml //!< YAML data
                                                  );



void operator >> (const YAML::Node& node, YamlController& c);

#endif /* PARSE_CONTROLLERS_HPP_ */
