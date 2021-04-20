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

    node["url"] >> ret.url;
    return ret;
}