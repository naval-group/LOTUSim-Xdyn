/*
 * parse_time_series.cpp
 *
 *  Created on: Oct 22, 2014
 *      Author: cady
 */

#include "parse_time_series.hpp"
#include "InvalidInputException.hpp"

#include <ssc/yaml_parser.hpp>

void operator >> (const YAML::Node& node, YamlTimeSeries& c)
{
    c.name = "";
    try
    {
        node["name"] >> c.name;
    }
    catch(std::exception& ) // Nothing to do: 'name' section is not mandatory (should be empty for controller setpoints)
    {
    }

    node["t"] >> c.t;
    for(YAML::Iterator it=node.begin();it!=node.end();++it)
    {
        std::string key = "";
        it.first() >> key;
        if ((key != "name") and (key != "t"))
        {
            try
            {
                std::vector<double> values;
                ssc::yaml_parser::parse_uv(node[key], values);
                c.commands[key] = values;
            }
            catch(const YAML::Exception& e)
            {
                THROW(__PRETTY_FUNCTION__, InvalidInputException, "Something is wrong with the YAML, more specifically in the 'commands' or 'setpoints' section. When parsing the '" << key << "' values: " << e.msg);
            }
        }
    }
}

std::vector<YamlTimeSeries> parse_command_yaml(const std::string& yaml)
{
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    std::vector<YamlTimeSeries> ret;
    node["commands"] >> ret;
    return ret;
}

std::vector<YamlTimeSeries> parse_setpoint_yaml(const std::string& yaml)
{
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    std::vector<YamlTimeSeries> ret;
    node["setpoints"] >> ret;
    return ret;
}
