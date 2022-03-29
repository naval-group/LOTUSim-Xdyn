#include "CSVController.hpp"
#include "CSVLineByLineReader.hpp"
#include "InvalidInputException.hpp"
#include <limits> // For float min , float max
#include <map>
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

std::unordered_map<std::string, double> zero_commands(const std::map<std::string,std::string>& commands2columns);
std::unordered_map<std::string, double> zero_commands(const std::map<std::string,std::string>& commands2columns)
{
    std::unordered_map<std::string, double> ret;
    for (const auto kv:commands2columns)
    {
        ret[kv.first] = 0;
    }
    return ret;
}

CSVLineByLineReader::CSVLineByLineReader(const CSVYaml& y)
: yaml(y)
, file(yaml.path)
, headers(get_headers())
, column_name_to_command_name(flip(yaml.commands))
, current(yaml.commands)
, next(yaml.commands)
, has_more_values(false)
, date2position()
, position_of_first_line(0)
, zero()
, max_date_so_far(std::numeric_limits<double>::min())
{
    if (yaml.path.empty())
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "Cannot open CSV file because an empty file name was supplied.")
    }
    if (not(file.is_open()))
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "CSV file '" << yaml.path << "' was not found.")
    }
    if (not(file.good()))
    {

       if (file.bad() || file.fail())
        {
            THROW(__PRETTY_FUNCTION__, InvalidInputException, "CSV file '" << yaml.path << "' was found, but xdyn is unable to read it: " <<  std::strerror(errno) << ".")
        }
        if (file.eof())
        {
            THROW(__PRETTY_FUNCTION__, InvalidInputException, "CSV file '" << yaml.path << "' was found, but it looks empty.")
        }
    }
}

template <typename K, typename V> std::ostream& operator<<(std::ostream& os, const std::unordered_map<K,V>& m)
{
    os << "{";
    for (const auto kv:m)
    {
        os << kv.first << ": " << kv.second << "  ";
    }

    return os << "}";
}


/**
 * @brief Sets the position in the stream buffer so 'date' is between current_date and next_date
 * 
 * @param date
 */
void CSVLineByLineReader::set_read_position(const double date)
{
    if (current.date == std::numeric_limits<double>::max() && date2position.empty())
    {
        current = read_next_line();
        next = read_next_line();
    }
    while (date >= next.date && !read_next_line().values.empty()) {
    }
    if (date2position.empty()) return;
    // Initialize the position of the first line if necessary
    if (!position_of_first_line)
    {
        position_of_first_line = date2position.begin()->second;
    }
    if (date2position.empty() || date < date2position.begin()->first)
    {
        current = zero;
        file.clear();
        file.seekg(position_of_first_line, file.beg);
        next = read_next_line();
        return;
    }
    
    long int previous_pos = position_of_first_line;
    double previous_date = std::numeric_limits<double>::max();
    current = DateValues(yaml.commands);
    for (const auto dateposition:date2position)
    {
        if (date >= previous_date && dateposition.first > date)
        {
            file.clear();
            file.seekg(previous_pos, file.beg);
            current = read_next_line();
            next = read_next_line();
            return;
        }
        previous_date = dateposition.first;
        previous_pos = dateposition.second;
    }
    file.clear();
    file.seekg(previous_pos, file.beg);
    current = read_next_line();
    next = read_next_line();
}

std::unordered_map<std::string, double> CSVLineByLineReader::get_values(const double t)
{
    set_read_position(t);
    return current.values;
}

bool CSVLineByLineReader::eof() const
{
    return not(has_more_values);
}

std::vector<std::string> CSVLineByLineReader::get_headers()
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

CSVLineByLineReader::DateValues::DateValues()
: date(std::numeric_limits<double>::max())
, values()
{
}

CSVLineByLineReader::DateValues::DateValues(const std::map<std::string,std::string>& commands2columns)
: date(std::numeric_limits<double>::max())
, values(zero_commands(commands2columns))
{
}

CSVLineByLineReader::DateValues CSVLineByLineReader::read_next_line()
{
    std::vector<std::string>   result;
    std::string                line;
    const long int current_position = file.tellg();
    // Get a new line from the CSV file
    std::getline(file, line);

    std::stringstream line_stream(line);
    std::string buffer;
    int i = 0;
    has_more_values = false;
    DateValues ret;
    // Loop on the values in the line, splitting by the separator character
    while(std::getline(line_stream, buffer, yaml.separator))
    {
        // Only store the values of interest and the current date: skip all other columns
        const auto it = column_name_to_command_name.find(headers[i]);
        if (it != column_name_to_command_name.end())
        {
            const double value = strtod(buffer.c_str(), NULL);
            ret.values[it->second] = value;
            has_more_values = true;
        }
        if (headers[i] == yaml.time_column)
        {
            ret.date = strtod(buffer.c_str(), NULL);
            if (date2position.find(ret.date)==date2position.end())
            {
                if (max_date_so_far>=ret.date)
                {
                    std::stringstream ss;
                    ss << "Values in time column '" << yaml.time_column << "' in CSV file '"
                       << yaml.path
                       << "' should be increasing: got "
                       << max_date_so_far
                       << " followed by "
                       << ret.date;
                    THROW(__PRETTY_FUNCTION__, InvalidInputException, ss.str());
                }
                date2position[ret.date] = current_position;
                max_date_so_far = ret.date;
            }
        }
        i++;
    }
    return ret;
}

double CSVLineByLineReader::get_next_date() const
{
    return next.date;
}