/*
 * CSVController.cpp
 *
 *  Created on: Mar, 16 2021
 *      Author: cady
 */

#include "InvalidInputException.hpp"
#include "CSVController.hpp"
#include "check_input_yaml.hpp"
#include "yaml.h"

CSVController::CSVController(const double tstart, const std::string& yaml_)
    : Controller(tstart, 0)
    , yaml(yaml_)
{
}

CSVController::Yaml::Yaml(const std::string& yaml)
    : path()
    , time_column()
    , separator(',')
    , shift_time_column(false)
    , commands()
{
    std::stringstream stream(yaml);
    std::stringstream ss;
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    node["path"] >> path;
    node["time column name"] >> time_column;
    std::string sep;
    node["separator"] >> sep;
    if (sep != "comma" && sep != "semicolon")
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "Invalid separator: got '" << separator << "' but only 'comma' and 'semicolon' are allowed.");
    }
    separator = sep == "comma" ? ',' : ';';
    std::string shift;
    node["shift time column to match tstart"] >> shift;
    if (shift != "true" && shift != "false")
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException,
              "Invalid value for 'shift time column to match tstart': got '"
                  << shift << "' but only 'true' and 'false' are allowed.");
    }
    shift_time_column = shift == "true";
    node["commands"] >> commands;
}

std::vector<std::string> CSVController::get_command_names() const
{
    std::vector<std::string> ret(yaml.commands.size());
    int i = 0;
    for (const auto kv:yaml.commands)
    {
        ret[i++] = kv.first;
    }
    return ret;
}

std::string CSVController::get_name() const
{
    return "";
}

void CSVController::update_discrete_states(const double ,
                                           ssc::solver::ContinuousSystem* )
{
    
}

double CSVController::get_measured_value(const ssc::solver::ContinuousSystem* ) const
{
    return 0;
}

double CSVController::compute_command(const double , const double ,
                                      const double )
{
    return 0;
}

double CSVController::get_date_of_next_update(const double current_time) const
{
    return current_time;
}