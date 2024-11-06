/*
 * AbstractWageningen.cpp
 *
 *  Created on: Jun 28, 2015
 *      Author: cady
 */
#include "AbstractWageningen.hpp"

#include "xdyn/yaml_parser/external_data_structures_parsers.hpp"

#include <ssc/yaml_parser.hpp>

#include "yaml.h"

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

AbstractWageningen::Yaml::Yaml():
    name(),
    position_of_propeller_frame(YamlPosition()),
    wake_coefficient(0.0),
    relative_rotative_efficiency(0.0),
    thrust_deduction_factor(0.0),
    rotating_clockwise(false),
    diameter(0.0)
{}

AbstractWageningen::Yaml AbstractWageningen::parse(const std::string& yaml)
{
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    Yaml ret;
    std::string rot;
    node["rotation"] >> rot;
    ret.rotating_clockwise = (rot == "clockwise");
    node["thrust deduction factor t"]        >> ret.thrust_deduction_factor;
    node["wake coefficient w"]               >> ret.wake_coefficient;
    node["name"]                             >> ret.name;
    node["position of propeller frame"]      >> ret.position_of_propeller_frame;
    node["relative rotative efficiency etaR"]>> ret.relative_rotative_efficiency;
    ssc::yaml_parser::parse_uv(node["diameter"], ret.diameter);
    return ret;
}

double AbstractWageningen::advance_ratio(const BodyStates& states, const EnvironmentAndFrames& env, const std::map<std::string,double>& commands) const
{
    // Naval Group Far East
    Eigen::Vector3d position = {states.x(),states.y(),states.z()};
    Eigen::Vector3d WCurrent = env.get_UWCurrent(position,t);
    const double Va = fabs(states.u()-WCurrent(0));
    // stop
    const double n = commands.at("rpm")/(2*PI);
    return (1-w)*Va/n/D;
}

AbstractWageningen::AbstractWageningen(const Yaml& input, const std::string& body_name_, const EnvironmentAndFrames& env) :
            ForceModel(input.name,{"rpm"},input.position_of_propeller_frame, body_name_, env),
            w(input.wake_coefficient),
            eta_R(input.relative_rotative_efficiency),
            t(input.thrust_deduction_factor),
            kappa(input.rotating_clockwise ? -1 : 1),
            D(input.diameter),
            D4(D*D*D*D),
            D5(D4*D)
{
}

Wrench AbstractWageningen::get_force(const BodyStates& states, const double, const EnvironmentAndFrames& env, const std::map<std::string,double>& commands) const
{
    Wrench tau(ssc::kinematics::Point(name,0,0,0), name);
    const double n2 = commands.at("rpm")*commands.at("rpm")/(4*PI*PI); // In turns per second (Hz)
    const double J = advance_ratio(states, env, commands);
    tau.X() = (1-t)*env.rho*n2*D4*get_Kt(commands, J);
    tau.K() = kappa*eta_R*env.rho*n2*D5*get_Kq(commands, J);
    return tau;
}
