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

CSVController::Yaml::Yaml(const std::string& yaml)
    : path()
    , time_column()
    , separator(',')
    , shift_time_column()
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


CSVYaml toCSVYaml(const CSVController::Yaml& yaml);
CSVYaml toCSVYaml(const CSVController::Yaml& yaml)
{
    CSVYaml ret;
    ret.commands = yaml.commands;
    ret.path = yaml.path;
    ret.separator = yaml.separator;
    ret.time_column = yaml.time_column;
    return ret;
}

CSVController::CSVController(const double tstart, const std::string& yaml_)
    : Controller(tstart, 0)
    , yaml(yaml_)
    , csv(toCSVYaml(yaml))
{
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

void CSVController::update_discrete_states(const double time,
                                           ssc::solver::ContinuousSystem* sys)
{
    const auto commands = csv.get_values(time);
    for (const auto cv:commands)
    {
        Controller::set_discrete_state(sys, cv.first, cv.second);
    }
}

double CSVController::get_date_of_next_update(const double current_time) const
{
    // If there are no more values in the CSV file, return the current_time which is interpreted as
    // "don't call the controller again" by ssc::solver::DiscreteSystem::callback
    if (csv.eof()) return current_time;
    // If there are more values, we just return the date of the next line.
    return csv.get_next_date();
}