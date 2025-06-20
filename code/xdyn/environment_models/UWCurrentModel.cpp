/*
 * UWCurrentModel.cpp
 *
 *  Created on: 13 may 2024
 *      Author: julien.prodhon
 */

// Naval Group Far East - CoE
// Creating files for UW current class


#include "UWCurrentModel.hpp"


UWCurrentModel::UWCurrentModel() {}

UWCurrentModel::~UWCurrentModel() {}


// std::string DnvrpUWCurrentModel::model_name() {return "dnvrp current";}

// DnvrpUWCurrentModel::Input DnvrpUWCurrentModel::parse(const std::string& yaml_input)
// {
//     Input ret;
//     std::stringstream stream(yaml_input);
//     YAML::Parser parser(stream);
//     YAML::Node node;
//     parser.GetNextDocument(node);
//     ssc::yaml_parser::parse_uv(node["orientation"], ret.orientation);
//     ssc::yaml_parser::parse_uv(node["tide velocity"], ret.tide_velocity);
//     ssc::yaml_parser::parse_uv(node["Vw0"], ret.Vw0);
//     node["alpha"] >> ret.alpha;
//     ssc::yaml_parser::parse_uv(node["upper depth"], ret.upper_depth);
//     return ret;
// }

// Eigen::Vector3d DnvrpUWCurrentModel::get_UWCurrent(const Eigen::Vector3d& p, const double, const double wave_height) const
// {
//     double z = p(2);
//     // % Tidal component V_t(z)
//     double Vt;
//     Vt = pow(tide_velocity * ((wave_height+z)/wave_height), alpha);
//     // % Wind-driven component V_w(z)
//     double Vw;
//     if (z < upper_depth) {Vw = Vw0 * ((upper_depth+z)/upper_depth);}
//     else {Vw = 0;}

//     double u = Vt + Vw;

//     return Eigen::Vector3d (u*cos(orientation), u*sin(orientation), 0);
// }

// std::string IsscUWCurrentModel::model_name() {return "issc current";}

// IsscUWCurrentModel::Input IsscUWCurrentModel::parse(const std::string& yaml_input)
// {
//     Input ret;
//     std::stringstream stream(yaml_input);
//     YAML::Parser parser(stream);
//     YAML::Node node;
//     parser.GetNextDocument(node);
//     ssc::yaml_parser::parse_uv(node["orientation"], ret.orientation);
//     ssc::yaml_parser::parse_uv(node["mean velocity"], ret.mean_velocity);
//     ssc::yaml_parser::parse_uv(node["upper depth"], ret.upper_depth);
//     ssc::yaml_parser::parse_uv(node["tide velocity"], ret.tide_velocity);
//     ssc::yaml_parser::parse_uv(node["U10"], ret.U10);
//     return ret;
// }

// Eigen::Vector3d IsscUWCurrentModel::get_UWCurrent(const Eigen::Vector3d& p, const double, const double wave_height) const
// {
//     double z = p(2);
//     // % Tidal component
//     double v_tide;
//     if (z < wave_height - 10) {v_tide = tide_velocity;}
//     else {v_tide = tide_velocity * log10(1 + (9*z)/(wave_height - 10));}
//     // Local wave component
//     double v_loc;
//     if (z < upper_depth) {v_loc = 0.02 * U10 * ((upper_depth-z)/upper_depth);}
//     else {v_loc = 0;}
//     double v = v_tide + v_loc + mean_velocity;
//     return Eigen::Vector3d (v*cos(orientation), v*sin(orientation), 0);
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

