/*
 * DnvrpUWCurrentModel.cpp
 *
 *  Created on: 13 may 2024
 *      Author: julien.prodhon
 */

// Naval Group Far East - CoE
// Creating files for UW current class

#include "DnvrpUWCurrentModel.hpp"

#include "yaml.h"
#include <ssc/yaml_parser.hpp>

DnvrpUWCurrentModel::DnvrpUWCurrentModel(int) : orientation(0), tide_velocity(0), Vw0(1), alpha(2), upper_depth(3)
{
}

DnvrpUWCurrentModel::DnvrpUWCurrentModel(const Input& input) : orientation(input.orientation), tide_velocity(input.tide_velocity), Vw0(input.Vw0), alpha(input.alpha), upper_depth(input.upper_depth)
{
}

DnvrpUWCurrentModel::~DnvrpUWCurrentModel()
{
}

std::string DnvrpUWCurrentModel::model_name() {return "dnvrp current";}

DnvrpUWCurrentModel::Input DnvrpUWCurrentModel::parse(const std::string& yaml_input)
{
    Input ret;
    std::stringstream stream(yaml_input);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    ssc::yaml_parser::parse_uv(node["orientation"], ret.orientation);
    ssc::yaml_parser::parse_uv(node["tide velocity"], ret.tide_velocity);
    ssc::yaml_parser::parse_uv(node["Vw0"], ret.Vw0);
    node["alpha"] >> ret.alpha;
    ssc::yaml_parser::parse_uv(node["upper depth"], ret.upper_depth);
    return ret;
}

Eigen::Vector3d DnvrpUWCurrentModel::get_UWCurrent(const Eigen::Vector3d& p, const double, const double wave_height) const
{
    double z = p(2);
    // % Tidal component V_t(z)
    double Vt;
    Vt = pow(tide_velocity * ((wave_height+z)/wave_height), alpha);
    // % Wind-driven component V_w(z)
    double Vw;
    if (z < upper_depth) {Vw = Vw0 * ((upper_depth+z)/upper_depth);}
    else {Vw = 0;}

    double u = Vt + Vw;

    return Eigen::Vector3d (u*cos(orientation), u*sin(orientation), 0);
}

DnvrpUWCurrentModel::Input::Input() : orientation(), tide_velocity(), Vw0(), alpha(), upper_depth()
{
}