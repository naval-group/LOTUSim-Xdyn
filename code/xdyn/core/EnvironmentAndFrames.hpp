/*
 * EnvironmentAndFrames.hpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#ifndef ENVIRONMENTANDFRAMES_HPP_
#define ENVIRONMENTANDFRAMES_HPP_

#include "xdyn/core/Body.hpp"
#include "xdyn/core/StateMacros.hpp"
#include "xdyn/core/SurfaceElevationInterface.hpp"
#include "environment_models/WindModel.hpp"
#include "external_data_structures/YamlRotation.hpp"
#include <boost/optional.hpp>
#include <ssc/kinematics.hpp>

class Observer;

struct EnvironmentAndFrames
{
    EnvironmentAndFrames ();
    void feed (Observer &observer, double t,
               const std::vector<BodyPtr> &bodies,
               const StateType &state) const;
    SurfaceElevationPtr w;
    WindModelPtr wind;
    ssc::kinematics::KinematicsPtr k;
    double rho;
    double nu;
    double g;
    YamlRotation rot;

    void set_rho_air (const double value);
    double get_rho_air () const;

    private:
        boost::optional<double> rho_air;
};

#endif /* ENVIRONMENTANDFRAMES_HPP_ */
