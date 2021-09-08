#include "yaml.h"
#include "History.hpp"
#include "InvalidInputException.hpp"
#include "InternalErrorException.hpp"
#include "StatesFilter.hpp"

std::shared_ptr<StatesFilter> StatesFilter::build(const std::string& yaml)
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
    return std::shared_ptr<StatesFilter>(new StatesFilter());
}

StatesFilter::StatesFilter() {}

double StatesFilter::get_value(const History& h) const
{
    return h();
}