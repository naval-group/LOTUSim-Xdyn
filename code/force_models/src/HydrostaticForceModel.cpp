/*
 * DampingForceModel.cpp
 *
 *  Created on: Oct 17, 2014
 *      Author: cady
 */

#include <Eigen/Dense>

#include <ssc/kinematics.hpp>

#include "Body.hpp"
#include "calculate_gz.hpp"
#include "HydrostaticForceModel.hpp"
#include "Observer.hpp"
#include "QuadraticDampingForceModel.hpp"

std::string HydrostaticForceModel::model_name(){return "hydrostatic";}

HydrostaticForceModel::HydrostaticForceModel(const std::string& body_name_, const EnvironmentAndFrames& env) :
        ForceModel(model_name(), {}, body_name_, env),
        centre_of_buoyancy(new Eigen::Vector3d())
{
}

bool HydrostaticForceModel::is_a_surface_force_model() const
{
    return true;
}

Wrench HydrostaticForceModel::get_force(const BodyStates& states, const double t, const EnvironmentAndFrames& env, const std::map<std::string,double>& commands) const
{
    auto C = states.intersector->center_of_mass_immersed();

    if (C.all_facets_are_in_same_plane) C.volume = 0;

    for (size_t i = 0 ; i < 3 ; ++i) centre_of_buoyancy->operator()(i) = C.G(i);

    // Coordinates of the center of buoyancy in the BODY frame
    const ssc::kinematics::Point B(states.name, C.G);

    ssc::kinematics::Vector6d w;

    w << 0,
         0,
         -env.rho*env.g*C.volume,
         0,
         0,
         0;
    return Wrench(B, "NED", w);
}

ssc::kinematics::Point HydrostaticForceModel::get_centre_of_buoyancy() const
{
    return ssc::kinematics::Point(get_body_name(), centre_of_buoyancy->operator()(0),
                                                   centre_of_buoyancy->operator()(1),
                                                   centre_of_buoyancy->operator()(2));
}

void HydrostaticForceModel::extra_observations(Observer& observer) const
{
    observer.write(centre_of_buoyancy->operator()(0),DataAddressing(std::vector<std::string>{"efforts",get_body_name(),get_name(),"Bx"},std::string("Bx")));
    observer.write(centre_of_buoyancy->operator()(1),DataAddressing(std::vector<std::string>{"efforts",get_body_name(),get_name(),"By"},std::string("By")));
    observer.write(centre_of_buoyancy->operator()(2),DataAddressing(std::vector<std::string>{"efforts",get_body_name(),get_name(),"Bz"},std::string("Bz")));
    //const double gz = calculate_gz(*this, env);
    //observer.write(gz,DataAddressing(std::vector<std::string>{"efforts",get_body_name(),get_name(),get_body_name(),"GZ"},std::string("GZ(")+get_name()+","+get_body_name()+")"));
}
