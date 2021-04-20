#include "GrpcController.hpp"

GrpcController::Input::Input () : url (), name (), yaml () {}

GrpcController::Input
GrpcController::parse (const std::string &)
{
    return GrpcController::Input ();
}