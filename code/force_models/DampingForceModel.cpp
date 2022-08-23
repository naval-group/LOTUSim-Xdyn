/*
 * DampingForceModel.cpp
 *
 *  Created on: Oct 17, 2014
 *      Author: cady
 */

#include "DampingForceModel.hpp"

#include <Eigen/Dense>
#include "Body.hpp"


DampingForceModel::DampingForceModel(const std::string& name_, const std::string& body_name_, const EnvironmentAndFrames& env, const Eigen::Matrix<double,6,6>& D_) :
        ForceModel(name_, {}, body_name_, env),
        D(D_)
{
}

Wrench DampingForceModel::get_force(const BodyStates& states, const double /*t*/, const EnvironmentAndFrames& /*env*/, const std::map<std::string,double>& /*commands*/) const
{
    Eigen::Matrix<double, 6, 1> W;
    W <<states.u(),
        states.v(),
        states.w(),
        states.p(),
        states.q(),
        states.r();
    return Wrench(states.hydrodynamic_forces_calculation_point, body_name, get_force_and_torque(D, W));
}
