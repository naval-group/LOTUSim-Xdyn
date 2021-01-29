/*
 * GravityForceModel.cpp
 *
 *  Created on: Jun 17, 2014
 *      Author: cady
 */

#include "EnvironmentAndFrames.hpp"
#include "GravityForceModel.hpp"
#include "SurfaceElevationInterface.hpp"
#include "Body.hpp"

#include <ssc/kinematics.hpp>

std::string GravityForceModel::model_name(){return "gravity";}

GravityForceModel::GravityForceModel(const std::string& body_name_, const EnvironmentAndFrames& env) :
        ControllableForceModel("gravity", {}, body_name_, env),
        g(env.g),
        k(env.k)
{}

Wrench GravityForceModel::get_force(const BodyStates& states, const double, const EnvironmentAndFrames&, const std::map<std::string,double>&) const
{
    const double m = states.solid_body_inertia->operator()(2,2);
    return Wrench(states.G, "NED", Eigen::Vector3d(0, 0, m * g), Eigen::Vector3d(0,0,0));
}

double GravityForceModel::potential_energy(const BodyStates& states, const std::vector<double>& x) const
{
    const double m = states.solid_body_inertia->operator()(2,2);
    return -m*g*x[2];
}
