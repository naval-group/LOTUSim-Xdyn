/*
 * LinearHydrostaticForceModel.hpp
 *
 *  Created on: Aug 12, 2015
 *      Author: cady
 */

#ifndef LINEARHYDROSTATICFORCEMODEL_HPP_
#define LINEARHYDROSTATICFORCEMODEL_HPP_

#include "xdyn/core/ForceModel.hpp"
#include "xdyn/core/EnvironmentAndFrames.hpp"
#include <ssc/kinematics.hpp>
#include <Eigen/Dense>
#include <map>
#include <string>
#include <vector>

class LinearHydrostaticForceModel : public ForceModel
{
    public:
        struct Input
        {
            Input();
            double z_eq;
            double theta_eq;
            double phi_eq;
            std::vector<double> K1;
            std::vector<double> K2;
            std::vector<double> K3;
        };
        LinearHydrostaticForceModel(const Input& input, const std::string& body_name, const EnvironmentAndFrames& env);
        Wrench get_force(const BodyStates& states, const double t, const EnvironmentAndFrames& env, const std::map<std::string,double>& commands) const override;
        static Input parse(const std::string& yaml);
        static std::string model_name();

    private:
        LinearHydrostaticForceModel();
        Eigen::Matrix<double,3,3> K;
        double z_eq;
        double theta_eq;
        double phi_eq;
};

#endif /* LINEARHYDROSTATICFORCEMODEL_HPP_ */
