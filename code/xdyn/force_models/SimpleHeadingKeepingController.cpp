/*
 * SimpleHeadingKeepingController.cpp
 *
 *  Created on: Jan 15, 2015
 *      Author: cady
 */
#include "SimpleHeadingKeepingController.hpp"

#include "xdyn/core/BodyStates.hpp"
#include "xdyn/yaml_parser/external_data_structures_parsers.hpp"
#include <ssc/yaml_parser.hpp>

#include "yaml.h"

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

std::string SimpleHeadingKeepingController::model_name() {return "simple heading controller";}

SimpleHeadingKeepingController::Yaml::Yaml() : name(), ksi(0), Tp(0)
{
}

SimpleHeadingKeepingController::Yaml SimpleHeadingKeepingController::parse(const std::string& yaml)
{
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    Yaml ret;
    node["name"] >> ret.name;
    node["ksi"] >> ret.ksi;
    ::ssc::yaml_parser::parse_uv(node["Tp"], ret.Tp);
    return ret;
}

SimpleHeadingKeepingController::SimpleHeadingKeepingController(const Yaml& input, const std::string& body_name_, const EnvironmentAndFrames& env) :
        ForceModel(input.name, {"psi_co"}, YamlPosition(YamlCoordinates(),YamlAngle(), body_name_), body_name_, env),
        ksi(input.ksi),
        omega0(2*PI/input.Tp),
        rotation_convention("angle", {"z","y'","x''"})
{
}

Wrench SimpleHeadingKeepingController::get_force(const BodyStates& states, const double, const EnvironmentAndFrames& /*env*/, const std::map<std::string,double>& commands) const
{
    Wrench ret(ssc::kinematics::Point(body_name,0,0,0), body_name);

    const auto angles = states.get_angles(rotation_convention);
    const double delta_psi = commands.at("psi_co") - angles.psi;
    const double sigma_zz = states.total_inertia(2,2);
    const double K_psi = sigma_zz*omega0*omega0;
    const double K_r = 2*ksi*omega0*sigma_zz;
    ret.N() = K_psi*delta_psi - K_r*states.r();
    return ret;
}
