/*
 * DampingForceModel.cpp
 *
 *  Created on: Oct 17, 2014
 *      Author: cady
 */

#include "DampingForceModel.hpp"
#include "xdyn/core/Body.hpp"
#include <Eigen/Dense>


DampingForceModel::DampingForceModel(const std::string& name_, const std::string& body_name_, const EnvironmentAndFrames& env, const Eigen::Matrix<double,6,6>& D_) :
        ForceModel(name_, {}, body_name_, env),
        D(D_)
{
}

Wrench DampingForceModel::get_force(const BodyStates& states, const double t, const EnvironmentAndFrames& env, const std::map<std::string,double>& /*commands*/) const
{
    // Naval Group Far East
    Eigen::Vector3d position = {states.x(),states.y(),states.z()};
    Eigen::Vector3d WCurrent = env.get_UWCurrent(position,t);
    // stop
    Eigen::Matrix<double, 6, 1> W;
    W <<states.u()-WCurrent(0),
        states.v()-WCurrent(1),
        states.w()-WCurrent(2),
        states.p(),
        states.q(),
        states.r();
    return Wrench(states.hydrodynamic_forces_calculation_point, body_name, get_force_and_torque(D, W));
}
