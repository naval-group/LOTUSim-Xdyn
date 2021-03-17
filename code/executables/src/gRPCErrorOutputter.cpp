#include "gRPCErrorOutputter.hpp"
#include <iostream>
void gRPCErrorOutputter::output() const
{
    std::cerr << get_message();
}