#include "GrpcController.hpp"
#include "yaml.h"

GrpcControllerInterface::Input
GrpcController::parse (const std::string &yaml)
{
    std::stringstream stream (yaml);
    std::stringstream ss;
    YAML::Parser parser (stream);
    YAML::Node node;
    parser.GetNextDocument (node);
    GrpcControllerInterface::Input ret;
    try
    {
        node["url"] >> ret.url;
        node["name"] >> ret.name;
        YAML::Emitter out;
        out << node;
        ret.yaml = out.c_str();
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