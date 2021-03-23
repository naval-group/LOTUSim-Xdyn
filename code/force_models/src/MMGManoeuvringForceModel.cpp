#include <Eigen/Dense>
#include "Body.hpp"
#include <ssc/yaml_parser.hpp>
#include <ssc/kinematics.hpp>
#include <cmath>

#include "yaml.h"
#include "EnvironmentAndFrames.hpp"
#include "YamlPosition.hpp"
#include "BodyStates.hpp"

#include "MMGManoeuvringForceModel.hpp"

std::string MMGManoeuvringForceModel::model_name() {return "MMG manoeuvring";}

MMGManoeuvringForceModel::Input::Input():
		application_point(),
		Lpp(),
		T(),
		Xvv(),
		Xrr(),
		Xvr(),
		Xvvvv(),
		Yv(),
		Yr(),
		Yvvv(),
		Yrvv(),
		Yvrr(),
		Yrrr(),
		Nv(),
		Nr(),
		Nvvv(),
		Nrvv(),
		Nvrr(),
		Nrrr()
{}

MMGManoeuvringForceModel::MMGManoeuvringForceModel(const Input& input, const std::string& body_name, const EnvironmentAndFrames& env):
		ForceModel(MMGManoeuvringForceModel::model_name(), {}, YamlPosition(input.application_point, YamlAngle(), body_name), body_name, env),
		Lpp(input.Lpp),
		T(input.T),
		Xvv(input.Xvv),
		Xrr(input.Xrr),
		Xvr(input.Xvr),
		Xvvvv(input.Xvvvv),
		Yv(input.Yv),
		Yr(input.Yr),
		Yvvv(input.Yvvv),
		Yrvv(input.Yrvv),
		Yvrr(input.Yvrr),
		Yrrr(input.Yrrr),
		Nv(input.Nv),
		Nr(input.Nr),
		Nvvv(input.Nvvv),
		Nrvv(input.Nrvv),
		Nvrr(input.Nvrr),
		Nrrr(input.Nrrr)
{}

MMGManoeuvringForceModel::Input MMGManoeuvringForceModel::parse(const std::string& yaml)
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

Wrench MMGManoeuvringForceModel::get_force(const BodyStates& states, const double t, const EnvironmentAndFrames& env, const std::map<std::string,double>& commands) const
{
    ssc::kinematics::Vector6d tau = ssc::kinematics::Vector6d::Zero();
    double v = states.v();
    double r = states.r();
    double xG = states.G.v(0) - env.k->get(body_name, name).get_point().v(0); // The point in the transform is always P in body frame as per the input
    double vm = v - xG*r;
    double U = sqrt(pow(states.u(),2)+pow(vm,2));
    if (U!=0)
    {
        double u_ = states.u()/U;
        double vm_ = vm/U;
        double r_ = r*Lpp/U;
        double X = Xvv*vm_*vm_ + Xrr*r_*r_ + Xvr*vm_*r_ + Xvvvv*vm_*vm_*vm_*vm_;
        double Y = Yv*vm_ + Yr*r_ + Yvvv*vm_*vm_*vm_ + Yvrr*vm_*r_*r_  + Yrrr*r_*r_*r_ + Yrvv*r_*vm_*vm_;
        double N = Nv*vm_ + Nr*r_ + Nvvv*vm_*vm_*vm_ + Nvrr*vm_*r_*r_  + Nrrr*r_*r_*r_ + Nrvv*r_*vm_*vm_;
        tau(0) = 0.5*env.rho*pow(U,2)*Lpp*T*X;
        tau(1) = 0.5*env.rho*pow(U,2)*Lpp*T*Y;
        tau(5) = 0.5*env.rho*pow(U,2)*Lpp*Lpp*T*N;
    }
    return Wrench(ssc::kinematics::Point(name, 0, 0, 0), body_name, tau);
}
