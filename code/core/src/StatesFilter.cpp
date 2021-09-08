#include "yaml.h"
#include "InvalidInputException.hpp"
#include "InternalErrorException.hpp"
#include "StatesFilter.hpp"

void StatesFilter::build(const std::string& yaml)
{
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    std::string type_of_filter;
    node["type of filter"] >> type_of_filter;
    if (type_of_filter != "moving average")
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "Unknown filter: known state filters are: 'moving average'.");
    }
}