#include <ssc/kinematics.hpp>

#include "yaml.h"
#include "external_data_structures_parsers.hpp"

#include "LinearStiffnessForceModel.hpp"

std::string LinearStiffnessForceModel::model_name() {return "linear stiffness";}

LinearStiffnessForceModel::LinearStiffnessForceModel(const Input input, const std::string& body_name, const EnvironmentAndFrames& env) :
        ForceModel(input.name, {}, body_name, env),
        K(input.K)
{
}

Wrench LinearStiffnessForceModel::get_force(const BodyStates& states, const double, const EnvironmentAndFrames&, const std::map<std::string,double>&) const
{
    Eigen::Matrix<double, 6, 1> X;
    ssc::kinematics::EulerAngles angles = states.get_angles();
    X <<states.x(),
        states.y(),
        states.z(),
        angles.phi,
        angles.theta,
        angles.psi;
    return Wrench(ssc::kinematics::Point(body_name,0,0,0), body_name, -K*X);
}

LinearStiffnessForceModel::Input LinearStiffnessForceModel::parse(const std::string& yaml)
{
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    Input ret;
    node["name"] >> ret.name;
    YamlDynamics6x6Matrix M;
    try
    {
        parse_YamlDynamics6x6Matrix(node["stiffness matrix"], M, false);
    }
    catch(const InvalidInputException& e)
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "In node 'damping matrix at the center of gravity projected in the body frame': " << e.get_message());
    }
    for (size_t j = 0 ; j < 6 ; ++j) ret.K(0,(int)j) = M.row_1[j];
    for (size_t j = 0 ; j < 6 ; ++j) ret.K(1,(int)j) = M.row_2[j];
    for (size_t j = 0 ; j < 6 ; ++j) ret.K(2,(int)j) = M.row_3[j];
    for (size_t j = 0 ; j < 6 ; ++j) ret.K(3,(int)j) = M.row_4[j];
    for (size_t j = 0 ; j < 6 ; ++j) ret.K(4,(int)j) = M.row_5[j];
    for (size_t j = 0 ; j < 6 ; ++j) ret.K(5,(int)j) = M.row_6[j];
    return ret;
}
