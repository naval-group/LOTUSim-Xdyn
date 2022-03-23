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
#include <cerrno>

template <typename K, typename V> std::unordered_map<V,K> flip(const std::map<K,V>& commands2columns)
{
    std::unordered_map<V,K> ret;
    for (const auto kv:commands2columns)
    {
        ret[kv.second] = kv.first;
    }
    return ret;
}


CSVController::InputFile::InputFile(const Yaml& y)
: yaml(y)
, file(yaml.path)
, headers(get_headers())
, column_name_to_command_name(flip(yaml.commands))
, current_values()
, current_date(0)
, has_more_values(false)
{
    if (yaml.path.empty())
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "An empty file name was supplied to the CSV controller.")
    }
    if (not(file.is_open()))
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "File '" << yaml.path << "' was not found.")
    }
    if (not(file.good()))
    {

       if (file.bad() || file.fail())
        {
            THROW(__PRETTY_FUNCTION__, InvalidInputException, "File '" << yaml.path << "' was found, but xdyn is unable to read it: " <<  std::strerror(errno) << ".")
        }
        if (file.eof())
        {
            THROW(__PRETTY_FUNCTION__, InvalidInputException, "File '" << yaml.path << "' was found, but it looks empty.")
        }
    }
    read_next_line();
}

std::unordered_map<std::string, double> CSVController::InputFile::get_commands() const
{
    return has_more_values ? current_values : std::unordered_map<std::string, double>();
}

bool CSVController::InputFile::eof() const
{
    return not(has_more_values);
}

std::vector<std::string> CSVController::InputFile::get_headers()
{
    std::vector<std::string> result;
    std::string line;
    std::getline(file, line);
    std::stringstream line_stream(line);
    std::string cell;
    // Store all values in the header row, splitting on the separator character specified in the YAML file
    while(std::getline(line_stream, cell, yaml.separator))
    {
        result.push_back(cell);
    }
    return result;
}

void CSVController::InputFile::read_next_line()
{
    std::vector<std::string>   result;
    std::string                line;
    // Get a new line frome the CSV file
    std::getline(file, line);

    std::stringstream line_stream(line);
    std::string buffer;
    int i = 0;
    has_more_values = false;
    // Loop on the values in the line, splitting by the separator character
    while(std::getline(line_stream, buffer, yaml.separator))
    {
        // Only store the values of interest and the current date: skip all other columns
        const auto it = column_name_to_command_name.find(headers[i]);
        if (it != column_name_to_command_name.end())
        {
            const double value = strtod(buffer.c_str(), NULL);
            current_values[it->second] = value;
            has_more_values = true;
        }
        if (headers[i] == yaml.time_column)
        {
            current_date = strtod(buffer.c_str(), NULL);
        }
        i++;
    }
}

double CSVController::InputFile::get_current_date() const
{
    return current_date;
}

CSVController::CSVController(const double tstart, const std::string& yaml_)
    : Controller(tstart, 0)
    , yaml(yaml_)
    , csv(yaml)
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

void CSVController::update_discrete_states(const double ,
                                           ssc::solver::ContinuousSystem* sys)
{
    const auto commands = csv.get_commands();
    for (const auto cv:commands)
    {
        Controller::set_discrete_state(sys, cv.first, cv.second);
    }
    csv.read_next_line();
}

double CSVController::get_date_of_next_update(const double current_time) const
{
    // If there are no more values in the CSV file, return the current_time which is interpreted as
    // "don't call the controller again" by ssc::solver::DiscreteSystem::callback
    if (csv.eof()) return current_time;
    // If there more values, we just return the cached date.
    return csv.get_current_date();
}