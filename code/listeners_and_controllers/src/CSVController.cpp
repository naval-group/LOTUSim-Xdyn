/*
 * CSVController.cpp
 *
 *  Created on: Mar, 16 2022
 *      Author: cady
 */

#include "InvalidInputException.hpp"
#include "CSVController.hpp"
#include "yaml.h"
#include <numeric>

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

CSVController::CSVController(const double tstart //!< Date of beginning of simulation (usually 0): this is needed by the parent class, but the date of the first call can only be known after reading the first line of the CSV file (or if shift_time_column is true)
, const std::string& yaml_)
    : Controller(tstart, 0)
    , yaml(yaml_)
    , csv(toCSVYaml(yaml))
    , tstart(std::numeric_limits<double>::min())
    , got_tstart(false)
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

void CSVController::initialize_tstart_on_first_call(const double t)
{
    // Only set tstart once
    if (not(got_tstart))
    {
        tstart = t;
        got_tstart = true;
    }
}

double CSVController::shift_time_if_necessary(const double time) const
{
    if (yaml.shift_time_column)
    {
        // Shift each time instant so simulation's tstart matches the date of the first line of the
        // CSV file
        return time - tstart + csv.get_initial_date();
    }
    return time;
}

void CSVController::update_discrete_states(const double time, //!< Current simulation time
                                           ssc::solver::ContinuousSystem* sys)
{
    initialize_tstart_on_first_call(time);
    const double t = shift_time_if_necessary(time);
    const auto commands = csv.get_values(t);
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
    const double tnext = csv.get_next_date();
    return yaml.shift_time_column ? tnext - csv.get_initial_date() + tstart: tnext;
}