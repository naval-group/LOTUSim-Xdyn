/*
 * LinearDampingForceModel.cpp
 *
 *  Created on: Oct 17, 2014
 *      Author: cady
 */

#include "LinearDampingForceModel.hpp"
#include "xdyn/core/Body.hpp"
#include "xdyn/core/yaml2eigen.hpp"
#include "xdyn/yaml_parser/external_data_structures_parsers.hpp"
#include <Eigen/Dense>
#include "yaml.h"

std::string LinearDampingForceModel::model_name() {return "linear damping";}

LinearDampingForceModel::LinearDampingForceModel(const Eigen::Matrix<double,6,6>& D_, const std::string& body_name_, const EnvironmentAndFrames& env) :
        DampingForceModel("linear damping", body_name_, env, D_)
{
}

Eigen::Matrix<double, 6, 1> LinearDampingForceModel::get_force_and_torque(const Eigen::Matrix<double,6,6>& D, const Eigen::Matrix<double, 6, 1>& W) const
{
    return (Eigen::Matrix<double, 6, 1>)(-D * W);
}

LinearDampingForceModel::Input LinearDampingForceModel::parse(const std::string& yaml)
{
    Eigen::Matrix<double,6,6> ret;
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    YamlDynamics6x6Matrix M;
    try
    {
        parse_YamlDynamics6x6Matrix(node["damping matrix at the center of gravity projected in the body frame"], M, false);
    }
    catch(const InvalidInputException& e)
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "In node 'damping matrix at the center of gravity projected in the body frame': " << e.get_message());
    }
    return make_matrix6x6(M);
}
