#include <iostream>

#include "ConsoleErrorOutputter.hpp"

void ConsoleErrorOutputter::output() const
{
    std::cerr << get_message();
}