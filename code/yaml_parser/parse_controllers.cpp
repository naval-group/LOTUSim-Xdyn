/*
 * parse_controllers.cpp
 *
 *  Created on: Fev 10, 2021
 *      Author: lincker
 */

#include "parse_controllers.hpp"
#include "InvalidInputException.hpp"
#include "external_data_structures_parsers.hpp"
#include <ssc/yaml_parser.hpp>

      void
      operator>>(const YAML::Node& node, YamlController& c)
{
    node["type"] >> c.type;
    node["name"] >> c.name;
    c.dt = 0;
    try_to_parse(node, "dt", c.dt);
    YAML::Emitter out;
    out << node;
    c.rest_of_the_yaml = out.c_str();
}

std::vector<YamlController> parse_controller_yaml(const std::string& yaml)
{
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    std::vector<YamlController> ret;
    node["controllers"] >> ret;
    return ret;
}
