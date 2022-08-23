/*
 * LinearHydrostaticForceModel.cpp
 *
 *  Created on: Aug 12, 2015
 *      Author: cady
 */

#include "LinearHydrostaticForceModel.hpp"
#include <ssc/exception_handling.hpp>
#include <ssc/yaml_parser.hpp>

std::string LinearHydrostaticForceModel::model_name() {return "linear hydrostatics";}

LinearHydrostaticForceModel::Input LinearHydrostaticForceModel::parse(const std::string& yaml)
{
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    LinearHydrostaticForceModel::Input ret;
    ssc::yaml_parser::parse_uv(node["z eq"], ret.z_eq);
    ssc::yaml_parser::parse_uv(node["phi eq"], ret.phi_eq);
    ssc::yaml_parser::parse_uv(node["theta eq"], ret.theta_eq);
    node["K row 1"] >> ret.K1;
    node["K row 2"] >> ret.K2;
    node["K row 3"] >> ret.K3;
    return ret;
}

LinearHydrostaticForceModel::LinearHydrostaticForceModel(const Input& input, const std::string& body_name_, const EnvironmentAndFrames& env) :
        ForceModel(model_name(), {}, body_name_, env),
        K(),
        z_eq(input.z_eq),
        theta_eq(input.theta_eq),
        phi_eq(input.phi_eq)
{
    K(0,0) = input.K1.at(0);
    K(0,1) = input.K1.at(1);
    K(0,2) = input.K1.at(2);
    K(1,0) = input.K2.at(0);
    K(1,1) = input.K2.at(1);
    K(1,2) = input.K2.at(2);
    K(2,0) = input.K3.at(0);
    K(2,1) = input.K3.at(1);
    K(2,2) = input.K3.at(2);
}

Wrench LinearHydrostaticForceModel::get_force(const BodyStates& states, const double t, const EnvironmentAndFrames& env, const std::map<std::string,double>&) const
{
    const auto angles = states.get_angles(env.rot);
    const Eigen::Vector3d v(states.z() - z_eq, angles.phi - phi_eq, angles.theta - theta_eq);
    const Eigen::Vector3d F = -K*v;

    return Wrench(states.G, body_name, Eigen::Vector3d(0, 0, F(0)), Eigen::Vector3d(F(1),F(2),0));
}
