/*
 * BasicBuoyancyForceModel.cpp
 *
 *  Created on: Jan 7, 2025
 *      Author: julien.prodhon
 */

#include "BasicBuoyancyForceModel.hpp"
#include "xdyn/core/Body.hpp"
#include "xdyn/core/EnvironmentAndFrames.hpp"
#include <ssc/yaml_parser.hpp>
#include <ssc/exception_handling.hpp>
#include <cmath>
#include "yaml.h"

std::string BasicBuoyancyForceModel::model_name(){return "basic buoyancy";}

BasicBuoyancyForceModel::Input::Input() : V()
{}

BasicBuoyancyForceModel::Input BasicBuoyancyForceModel::parse(const std::string& yaml)
{
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    Input ret;
    ssc::yaml_parser::parse_uv(node["volume"], ret.V);
    return ret;
}

BasicBuoyancyForceModel::BasicBuoyancyForceModel(const Input& input, const std::string& body_name, const EnvironmentAndFrames& env) :
        ForceModel("basic buoyancy", {}, body_name, env),
        input(input),
        g(env.g),
        rho(env.rho),
        V(input.V)
{}

Wrench BasicBuoyancyForceModel::get_force(const BodyStates& states, const double t, const EnvironmentAndFrames& env, const std::map<std::string,double>&) const
{
    double f = 0;
    if (env.w == nullptr) {f = -rho*g*V;}
    else
    {
        std::vector<double> x = states.get_current_state_values(0);
        std::vector<double> sea_height = env.w->get_and_check_wave_height({x[0]},{x[1]},t);
        if (x[2] > sea_height[0]) {f = -rho*g*V;}
    }
    return Wrench(states.G, "NED", Eigen::Vector3d(0, 0, f), Eigen::Vector3d(0,0,0));
}

double BasicBuoyancyForceModel::potential_energy(const BodyStates&, const std::vector<double>& x) const
{
    return rho*g*V*x[2];
}