/*
 * ConstantUWCurrentModel.cpp
 *
 *  Created on: 21 may 2024
 *      Author: julien.prodhon
 */

// Naval Group Far East - CoE
// Creating files for UW current class

#include "ConstantUWCurrentModel.hpp"

#include <map>
#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI
#include "yaml.h"
#include <ssc/yaml_parser.hpp>

//  CAREFUL NED convention
// REVERIFY WITH THE CONVENTION

ConstantUWCurrentModel::ConstantUWCurrentModel(const Input& input) : velocity(input.velocity), orientation(input.orientation)
{
}

ConstantUWCurrentModel::ConstantUWCurrentModel(int i) : velocity(i), orientation(0)
{
}

ConstantUWCurrentModel::~ConstantUWCurrentModel()
{
}

std::string ConstantUWCurrentModel::model_name() {return "constant UW current";}

ConstantUWCurrentModel::Input ConstantUWCurrentModel::parse(const std::string& yaml_input)
{
    Input ret;
    std::stringstream stream(yaml_input);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    ssc::yaml_parser::parse_uv(node["velocity"], ret.velocity);
    ssc::yaml_parser::parse_uv(node["orientation"], ret.orientation);
    return ret;
}

Eigen::Vector3d ConstantUWCurrentModel::get_UWCurrent(const Eigen::Vector3d& p, const double t, const double wave_height) const
{
    if (p(3) > 2) {return Eigen::Vector3d (velocity*cos(orientation), velocity*sin(orientation), 0*p(0)+0*t*wave_height);}
    else {return Eigen::Vector3d::Zero();}
    
}

ConstantUWCurrentModel::Input::Input() : velocity(), orientation()
{
}