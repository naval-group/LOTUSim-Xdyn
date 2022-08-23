#ifndef CSVYAMLHPP
#define CSVYAMLHPP

#include <map>
#include <string>

struct CSVYaml
{
    CSVYaml();
    std::string path; //!< Path to CSV file containing the commands
    std::string time_column; //!< Title of the time column
    char separator; //!< Column separator (either ',' or ';')
    std::map<std::string,std::string> commands; //!< Command name to column name mapping
};

#endif