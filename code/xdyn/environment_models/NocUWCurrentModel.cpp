// /*
//  * NocUWCurrentModel.cpp
//  *
//  *  Created on: 13 may 2024
//  *      Author: julien.prodhon
//  */

// // Naval Group Far East - CoE
// // Creating files for UW current class

// #include "NocUWCurrentModel.hpp"

// #include "yaml.h"
// #include <ssc/yaml_parser.hpp>

// NocUWCurrentModel::NocUWCurrentModel(int) : orientation(0), mean_velocity(0), upper_depth(1), lower_depth(2)
// {
// }

// NocUWCurrentModel::NocUWCurrentModel(const Input& input) : orientation(input.orientation), mean_velocity(input.mean_velocity), upper_depth(input.upper_depth), lower_depth(input.lower_depth)
// {
// }

// NocUWCurrentModel::~NocUWCurrentModel()
// {
// }

// std::string NocUWCurrentModel::model_name() {return "noc current";}

// NocUWCurrentModel::Input NocUWCurrentModel::parse(const std::string& yaml_input)
// {
//     Input ret;
//     std::stringstream stream(yaml_input);
//     YAML::Parser parser(stream);
//     YAML::Node node;
//     parser.GetNextDocument(node);
//     ssc::yaml_parser::parse_uv(node["orientation"], ret.orientation);
//     ssc::yaml_parser::parse_uv(node["mean velocity"], ret.mean_velocity);
//     ssc::yaml_parser::parse_uv(node["upper depth"], ret.upper_depth);
//     ssc::yaml_parser::parse_uv(node["lower depth"], ret.lower_depth);
//     return ret;
// }

// Eigen::Vector3d NocUWCurrentModel::get_UWCurrent(const Eigen::Vector3d& p, const double, const double wave_height) const
// {
//     double z = p(2);
//     double u;
//     // upper_depth / 2
//     if (z > wave_height && upper_depth <= z)
//     {
//         u = mean_velocity * log(upper_depth / lower_depth) / (log(upper_depth / lower_depth) - upper_depth / wave_height);
//         return Eigen::Vector3d (u*cos(orientation), u*sin(orientation), 0);
//     }
//     else if (z > upper_depth && lower_depth <= z)
//     {
//         u = mean_velocity * log(z / lower_depth) / (log(upper_depth / lower_depth) - upper_depth / wave_height);
//         return Eigen::Vector3d (u*cos(orientation), u*sin(orientation), 0);
//     }
//     else
//     {
//         return Eigen::Vector3d::Zero();
//     }
// }

// NocUWCurrentModel::Input::Input() : orientation(), mean_velocity(), upper_depth(), lower_depth()
// {
// }