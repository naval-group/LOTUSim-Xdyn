/*
 * ConstantUWCurrentModel.cpp
 *
 *  Created on: 21 may 2024
 *      Author: julien.prodhon
 */

#include "ConstantUWCurrentModel.hpp" 
#include <cmath>
#include <sstream>
#include "yaml.h"
#include <ssc/yaml_parser.hpp>

ConstantUWCurrentModel::ConstantUWCurrentModel(const Input& input) : velocity(input.velocity), orientation(input.orientation)
{
}

ConstantUWCurrentModel::ConstantUWCurrentModel(int) : velocity(0), orientation(0)
{
}

ConstantUWCurrentModel::~ConstantUWCurrentModel()
{
}

std::string ConstantUWCurrentModel::model_name() {return "constant UW current";}

ConstantUWCurrentModel::Input ConstantUWCurrentModel::parse(const std::string& yaml_input) {
    Input result;
    std::stringstream stream(yaml_input);
    YAML::Parser parser(stream);
    YAML::Node node;
    if (!parser.GetNextDocument(node)) {
        throw std::runtime_error("Failed to parse UW constant current YAML input");
    }
    ssc::yaml_parser::parse_uv(node["velocity"], result.velocity);
    ssc::yaml_parser::parse_uv(node["orientation"], result.orientation);
    return result;
}

Eigen::Vector3d ConstantUWCurrentModel::get_UWCurrent(const Eigen::Vector3d& position, const double /*time*/, const double wave_height) const 
{   
    if (position.z() > wave_height) {
        return Eigen::Vector3d(velocity * std::cos(orientation),velocity * std::sin(orientation),0.0);
    }
    return Eigen::Vector3d::Zero();
}

ConstantUWCurrentModel::Input::Input() : velocity(), orientation()
{
}