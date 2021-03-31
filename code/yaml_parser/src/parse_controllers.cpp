/*
 * parse_controllers.cpp
 *
 *  Created on: Fev 10, 2021
 *      Author: lincker
 */

#include "parse_controllers.hpp"
#include "InvalidInputException.hpp"

#include <ssc/yaml_parser.hpp>

void operator >> (const YAML::Node& node, YamlController& c)
{
    node["name"] >> c.name;
    node["output"] >> c.output;
    node["type"] >> c.type;
    node["dt"] >> c.dt;
    node["setpoint"] >> c.setpoint;

    for(YAML::Iterator it=node["state_weights"].begin();it!=node["state_weights"].end();++it)
    {
        std::string key = "";
        it.first() >> key;
        try
        {
            double value;
            node["state_weights"][key] >> value;
            c.state_weights[key] = value;
        }
        catch(const YAML::Exception& e)
        {
            THROW(__PRETTY_FUNCTION__, InvalidInputException, "Something is wrong with the YAML, more specifically in the 'state_weights' section. When parsing the '" << key << "' values: " << e.msg);
        }
    }

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
