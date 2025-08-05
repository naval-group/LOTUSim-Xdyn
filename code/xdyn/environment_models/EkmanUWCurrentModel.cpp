/*
 * EkmanUWCurrentModel.cpp
 *
 *  Created on: 5 august 2025
 *      Author: julien.prodhon
 */

#include "EkmanUWCurrentModel.hpp"
#include "Seabed.hpp"
#include <cmath>
#include <ssc/yaml_parser.hpp>
#include "xdyn/yaml_parser/external_data_structures_parsers.hpp"

// Constructor with input parameters
EkmanUWCurrentModel::EkmanUWCurrentModel(const Input& input) 
    : seabed(input.seabed), 
      top_ekman_depth(input.top_ekman_depth), 
      bottom_ekman_depth(input.bottom_ekman_depth), 
      middle_velocity(input.middle_velocity), 
      middle_orientation(input.middle_orientation), 
      f_and_sqrt_rho(input.f_and_sqrt_rho),
      wind_angle(input.wind_angle), 
      wind_stress(input.wind_stress)
{}

// Default constructor
EkmanUWCurrentModel::EkmanUWCurrentModel(int /* dummy_param */) 
    : seabed(0),
      top_ekman_depth(), 
      bottom_ekman_depth(), 
      middle_velocity(), 
      middle_orientation(), 
      f_and_sqrt_rho(),
      wind_angle(), 
      wind_stress()
{}

EkmanUWCurrentModel::~EkmanUWCurrentModel() {}

std::string EkmanUWCurrentModel::model_name() {return "ekman current";}

EkmanUWCurrentModel::Input EkmanUWCurrentModel::parse(const std::string& yaml_input)
{
    Input input;
    // Yaml import
    std::stringstream stream(yaml_input);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    // optional constants parsing
    double rho = 1026.0; // kg/m³ / Default water density
    try_to_parse(node, "rho", rho);
    double omega = 7.2921e-5; // rad/s / Default Earth rotation rate
    try_to_parse(node, "omega", omega);
    double rho_air =  1.225; // kg/m³ / Default air density
    try_to_parse(node, "rho_air", rho_air);
    // Seabed parsing
    if (node.FindValue("seabed file")) {
        std::string seabed_file;
        node["seabed file"] >> seabed_file;
        input.seabed = Seabed(seabed_file);
    }
    else if (node.FindValue("seabed depth")) {
        double seabed_depth ;
        node["seabed depth"] >> seabed_depth;
        input.seabed = Seabed(seabed_depth);
    }
    else {
        throw std::runtime_error("Please provide a seabed file or a fixed depth for Ekman model");
    }
    // Wind parsing
    double U10;
    ssc::yaml_parser::parse_uv(node["wind orientation"], input.wind_angle);
    ssc::yaml_parser::parse_uv(node["U10"], U10);
    double drag_coefficient = (U10 < 20.2) ? 0.79e-3 + 0.08e-3 * U10 : 0.002423; // Threshold at 20.2 m/s
    input.wind_stress = drag_coefficient * rho_air * U10 * U10;
    // Middle current parsing
    ssc::yaml_parser::parse_uv(node["current velocity"], input.middle_velocity);
    ssc::yaml_parser::parse_uv(node["current orientation"], input.middle_orientation);
    // Ekman layers parsing
    double latitude;
    ssc::yaml_parser::parse_uv(node["latitude"], latitude);
    ssc::yaml_parser::parse_uv(node["top layer thickness"], input.top_ekman_depth);
    ssc::yaml_parser::parse_uv(node["bottom layer thickness"], input.bottom_ekman_depth);
    input.f_and_sqrt_rho = 2 * omega * sin(latitude) * sqrt(rho);
    return input;
}

Eigen::Vector3d EkmanUWCurrentModel::get_UWCurrent(const Eigen::Vector3d& position, const double, const double wave_height) const
{
    double z = position(2);
    double seabed_height = seabed.get_height(position(0), position(1));
    Eigen::Vector3d middle_current = getMiddleLayerCurrent();
    if (z > wave_height && z < (wave_height + 2 * top_ekman_depth)) // Ekman depth effect is inferior to 0.2% below 2 ekman depths
    {
        return getTopLayerCurrent(position, middle_current, wave_height); // Wind-driven Ekman spiral
    }
    else if (z >= (wave_height + 2 * top_ekman_depth) && z <= (seabed_height - 2 * bottom_ekman_depth)) 
    {
        return middle_current; // Uniform background current
    }
    else if (z > (seabed_height - 2 * bottom_ekman_depth) && z < seabed_height) // Ekman depth effect is inferior to 0.2% above 2 ekman depths
    {
        return getBottomLayerCurrent(position, middle_current, seabed_height); // Seabed-influenced current
    }
    else 
    {
        return Eigen::Vector3d::Zero();  // Outside valid depth range
    }
}

Eigen::Vector3d EkmanUWCurrentModel::getTopLayerCurrent(const Eigen::Vector3d& position, 
                                                        const Eigen::Vector3d& middle_current,
                                                        const double // wave_height
                                                        ) const
{
    double z = position(2); 
    double sgn_f = (f_and_sqrt_rho >= 0) ? 1.0 : -1.0;
    double V0 = sqrt(2) * M_PI * wind_stress / ( top_ekman_depth * f_and_sqrt_rho );
    double decay_factor = top_ekman_depth / M_PI;
    // Horizontal velocities
    double u = middle_current(0) + sgn_f * V0 * exp( - z / decay_factor ) * cos(M_PI / 4 - z / decay_factor - sgn_f * wind_angle);
    double v = middle_current(1) + V0 * exp( - z / decay_factor ) * sin(M_PI / 4 - z / decay_factor + sgn_f * wind_angle);
    // Vertical velocity (currently not supported, has to be incorporated with orbital velocities)
    double w = 0.0;
    return Eigen::Vector3d(u, v, w);
}

Eigen::Vector3d EkmanUWCurrentModel::getMiddleLayerCurrent() const
{
    return Eigen::Vector3d(middle_velocity * cos(middle_orientation), middle_velocity * sin(middle_orientation), 0);
}

Eigen::Vector3d EkmanUWCurrentModel::getBottomLayerCurrent(const Eigen::Vector3d& position, 
                                                           const Eigen::Vector3d& middle_current,
                                                           const double seabed_height) const
{
    double depth_factor = M_PI * (seabed_height - position(2)) / bottom_ekman_depth;
    // Horizontal velocities
    double u = middle_current(0) * ( 1 - exp(-depth_factor) * cos(depth_factor) ) - middle_current(1) * exp(-depth_factor) * sin(depth_factor);
    double v = middle_current(0) * exp(-depth_factor) * sin(depth_factor) + middle_current(1) * ( 1 - exp(-depth_factor) * cos(depth_factor) );
    // Vertical velocity (currently not supported)
    double w = 0.0;
    return Eigen::Vector3d(u, v, w);
}

EkmanUWCurrentModel::Input::Input() :
    seabed(0), 
    top_ekman_depth(), 
    bottom_ekman_depth(), 
    middle_velocity(), 
    middle_orientation(), 
    f_and_sqrt_rho(),
    wind_angle(),
    wind_stress()
{}

EkmanUWCurrentModel::Input::Input(Seabed seabed_input) : 
    seabed(seabed_input), 
    top_ekman_depth(), 
    bottom_ekman_depth(), 
    middle_velocity(), 
    middle_orientation(), 
    f_and_sqrt_rho(),
    wind_angle(),
    wind_stress()
{}