#include "InvalidInputException.hpp"
#include "InternalErrorException.hpp"
#include "StatesFilter.hpp"

void StatesFilter::build(const std::string& )
{
    THROW(__PRETTY_FUNCTION__, InvalidInputException, "Unknown filter: known state filters are: 'moving average'.");
}