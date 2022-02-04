#include <Eigen/Dense>
#include "Body.hpp"
#include <ssc/yaml_parser.hpp>
#include <ssc/kinematics.hpp>
#include <cmath>

#include "yaml.h"
#include "EnvironmentAndFrames.hpp"
#include "YamlPosition.hpp"
#include "BodyStates.hpp"

#include "MMGManeuveringForceModel.hpp"

std::string MMGManeuveringForceModel::model_name() {return "MMG maneuvering";}

MMGManeuveringForceModel::MMGManeuveringForceModel(const Input& input_, const std::string& body_name, const EnvironmentAndFrames& env):
        ForceModel(MMGManeuveringForceModel::model_name(), {}, YamlPosition(input_.application_point, YamlAngle(), body_name), body_name, env),
        input(input_)
{}

MMGManeuveringForceModel::Input MMGManeuveringForceModel::parse(const std::string& yaml)
{
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    Input ret;

    node["calculation point in body frame"] >> ret.application_point;
    ssc::yaml_parser::parse_uv(node["Lpp"], ret.Lpp);
    ssc::yaml_parser::parse_uv(node["T"], ret.T);
    node["Xvv"] >> ret.Xvv;
    node["Xrr"] >> ret.Xrr;
    node["Xvr"] >> ret.Xvr;
    node["Xvvvv"] >> ret.Xvvvv;
    node["Yv"] >> ret.Yv;
    node["Yr"] >> ret.Yr;
    node["Yvvv"] >> ret.Yvvv;
    node["Yrvv"] >> ret.Yrvv;
    node["Yvrr"] >> ret.Yvrr;
    node["Yrrr"] >> ret.Yrrr;
    node["Nv"] >> ret.Nv;
    node["Nr"] >> ret.Nr;
    node["Nvvv"] >> ret.Nvvv;
    node["Nrvv"] >> ret.Nrvv;
    node["Nvrr"] >> ret.Nvrr;
    node["Nrrr"] >> ret.Nrrr;

    return ret;
}

Wrench MMGManeuveringForceModel::get_force(const BodyStates& states, const double t, const EnvironmentAndFrames& env, const std::map<std::string,double>& commands) const
{
    ssc::kinematics::Vector6d tau = ssc::kinematics::Vector6d::Zero();
    const double v = states.v();
    const double r = states.r();
    const double xG = states.G.v(0) - env.k->get(body_name, name).get_point().v(0); // The point in the transform is always P in body frame as per the input
    const double vm = v - xG*r;
    const double U = hypot(states.u(), vm);
    if (U!=0)
    {
        const double u_ = states.u()/U;
        const double vm_ = vm/U;
        const double r_ = r*input.Lpp/U;
        const double X = input.Xvv*vm_*vm_ + input.Xrr*r_*r_ + input.Xvr*vm_*r_ + input.Xvvvv*vm_*vm_*vm_*vm_;
        const double Y = input.Yv*vm_ + input.Yr*r_ + input.Yvvv*vm_*vm_*vm_ + input.Yvrr*vm_*r_*r_  + input.Yrrr*r_*r_*r_ + input.Yrvv*r_*vm_*vm_;
        const double N = input.Nv*vm_ + input.Nr*r_ + input.Nvvv*vm_*vm_*vm_ + input.Nvrr*vm_*r_*r_  + input.Nrrr*r_*r_*r_ + input.Nrvv*r_*vm_*vm_;
        tau(0) = 0.5*env.rho*pow(U,2)*input.Lpp*input.T*X;
        tau(1) = 0.5*env.rho*pow(U,2)*input.Lpp*input.T*Y;
        tau(5) = 0.5*env.rho*pow(U,2)*input.Lpp*input.Lpp*input.T*N;
    }
    return Wrench(ssc::kinematics::Point(name, 0, 0, 0), body_name, tau);
}
