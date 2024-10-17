/*
 * IsscUWCurrentModel.cpp
 *
 *  Created on: 13 may 2024
 *      Author: julien.prodhon
 */

// Naval Group Far East - CoE
// Creating files for UW current class

#include "IsscUWCurrentModel.hpp"

#include "yaml.h"
#include <ssc/yaml_parser.hpp>

IsscUWCurrentModel::IsscUWCurrentModel(int) : orientation(0), mean_velocity(0), upper_depth(1), tide_velocity(2), U10(0)
{
}

IsscUWCurrentModel::IsscUWCurrentModel(const Input& input) : orientation(input.orientation), mean_velocity(input.mean_velocity), upper_depth(input.upper_depth), tide_velocity(input.tide_velocity), U10(input.U10)
{
}

IsscUWCurrentModel::~IsscUWCurrentModel()
{
}

std::string IsscUWCurrentModel::model_name() {return "issc current";}

IsscUWCurrentModel::Input IsscUWCurrentModel::parse(const std::string& yaml_input)
{
    Input ret;
    std::stringstream stream(yaml_input);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    ssc::yaml_parser::parse_uv(node["orientation"], ret.orientation);
    ssc::yaml_parser::parse_uv(node["mean velocity"], ret.mean_velocity);
    ssc::yaml_parser::parse_uv(node["upper depth"], ret.upper_depth);
    ssc::yaml_parser::parse_uv(node["tide velocity"], ret.tide_velocity);
    ssc::yaml_parser::parse_uv(node["U10"], ret.U10);
    return ret;
}

Eigen::Vector3d IsscUWCurrentModel::get_UWCurrent(const Eigen::Vector3d& p, const double, const double wave_height) const
{
    double z = p(2);
    // % Tidal component
    double v_tide;
    if (z < wave_height - 10) {v_tide = tide_velocity;}
    else {v_tide = tide_velocity * log10(1 + (9*z)/(wave_height - 10));}
    // Local wave component
    double v_loc;
    if (z < upper_depth) {v_loc = 0.02 * U10 * ((upper_depth-z)/upper_depth);}
    else {v_loc = 0;}
    double v = v_tide + v_loc + mean_velocity;
    return Eigen::Vector3d (v*cos(orientation), v*sin(orientation), 0);
}

IsscUWCurrentModel::Input::Input() : orientation(), mean_velocity(), upper_depth(), tide_velocity(), U10()
{
}