#include "GrpcController.hpp"
#include "yaml.h"

GrpcController::Input::Input () : url (), name (), yaml () {}

GrpcController::Input
GrpcController::parse (const std::string &yaml)
{
    std::stringstream stream (yaml);
    std::stringstream ss;
    YAML::Parser parser (stream);
    YAML::Node node;
    parser.GetNextDocument (node);
    GrpcController::Input ret;
    try
    {
        node["url"] >> ret.url;
    }
    catch (YAML::Exception &e)
    {
        THROW (__PRETTY_FUNCTION__, InvalidInputException,
               "Unable to parse YAML data for a gRPC controller:"
                   << std::endl
                   << e.what () << std::endl
                   << "The offending YAML block was:" << std::endl
                   << yaml);
    }
    return ret;
}