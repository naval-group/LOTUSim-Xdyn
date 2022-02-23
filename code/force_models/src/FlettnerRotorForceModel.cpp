#include <string>
#include <cmath>
#include <Eigen/Dense>
#include <ssc/yaml_parser.hpp>
#include <ssc/kinematics.hpp>
#include <ssc/interpolation.hpp>

#include "ForceModel.hpp"
#include "BodyStates.hpp"
#include "yaml.h"
#include "external_data_structures_parsers.hpp"

#include "FlettnerRotorForceModel.hpp"

#define PI M_PI

std::string FlettnerRotorForceModel::model_name() {return "Flettner rotor";}

FlettnerRotorForceModel::FlettnerRotorForceModel(const Input& input, const std::string& body_name, const EnvironmentAndFrames& env):
        ForceModel(input.name, {"rpm"}, body_name, env),
        calculation_point(input.calculation_point_in_body_frame.x, input.calculation_point_in_body_frame.y, input.calculation_point_in_body_frame.z),
        radius(input.diameter/2.),
        reference_area(input.length*input.diameter),
        Cl(new ssc::interpolation::SplineVariableStep(input.spin_ratio, input.lift_coefficient, true)),
        Cd(new ssc::interpolation::SplineVariableStep(input.spin_ratio, input.drag_coefficient, true)),
        sr_bounds(*std::min_element(input.spin_ratio.begin(), input.spin_ratio.end()), *std::max_element(input.spin_ratio.begin(), input.spin_ratio.end())),
        spin_ratio(new double(0))
{
}

FlettnerRotorForceModel::Input FlettnerRotorForceModel::parse(const std::string& yaml)
{
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    Input ret;

    node["name"] >> ret.name;
    ssc::yaml_parser::parse_uv(node["length"], ret.length);
    ssc::yaml_parser::parse_uv(node["diameter"], ret.diameter);
    node["spin ratio"] >> ret.spin_ratio;
    node["lift coefficient"] >> ret.lift_coefficient;
    node["drag coefficient"] >> ret.drag_coefficient;
    node["rotor position in body frame"] >> ret.calculation_point_in_body_frame;

    return ret;
}

Wrench FlettnerRotorForceModel::get_force(const BodyStates& states, const double t, const EnvironmentAndFrames& env, const std::map<std::string,double>& commands) const
{
    const Eigen::Vector3d omega(states.p(), states.q(), states.r());
    const Eigen::Vector3d Vo(states.u(), states.v(), states.w());
    const Eigen::Vector3d Vp_body = Vo - calculation_point.cross(omega); // Velocity of point P of body relative to NED, expressed in body frame
    const auto rotation = states.get_rot_from_ned_to_body();
    const Eigen::Vector3d P_NED = Eigen::Vector3d(states.x(), states.y(), states.z()) + rotation*calculation_point; // Coordinates of point P in NED frame
    const Eigen::Vector3d wind_in_NED = env.wind->get_wind(P_NED, t);
    const Eigen::Vector3d true_wind_in_body_frame = rotation.transpose()*wind_in_NED;
    const Eigen::Vector3d W = true_wind_in_body_frame - Vp_body; // Apparent wind in body frame
    const double beta = atan2(-W(1), -W(0));
    const double U = sqrt(W(0)*W(0) + W(1)*W(1)); // Apparent wind speed projected in the internal horizontal plane

    const double rpm = commands.at("rpm");
    *spin_ratio = (rpm!=0. && U>0.) ? abs(rpm*radius/U) : 0.;
    double sr_interp = *spin_ratio;
    if (sr_interp < sr_bounds.first)
    {
        std::cerr << "Warning: spin ratio in Flettner rotor model '" << name << "' subceeds minimum tabulated value. Saturating at SR=" << sr_bounds.first << std::endl;
        sr_interp = sr_bounds.first;
    }
    else if (sr_interp > sr_bounds.second)
    {
        std::cerr << "Warning: spin ratio in Flettner rotor model '" << name << "' exceeds maximum tabulated value. Saturating at SR=" << sr_bounds.second << std::endl;
        sr_interp = sr_bounds.second;
    }
    const double rho = env.get_rho_air();
    const double lift = (rpm>=0 ? -0.5*Cl->f(sr_interp)*rho*U*U*reference_area : 0.5*Cl->f(sr_interp)*rho*U*U*reference_area);
    const double drag = 0.5*Cd->f(sr_interp)*rho*U*U*reference_area;

    Wrench ret(ssc::kinematics::Point(body_name, calculation_point), body_name);
    ret.X() = -sin(beta)*lift - cos(beta)*drag;
    ret.Y() = cos(beta)*lift - sin(beta)*drag;
    return ret;
}

void FlettnerRotorForceModel::extra_observations(Observer& observer) const
{
    observer.write(*spin_ratio, DataAddressing({"efforts",body_name,name,"SR"},std::string("SR(")+name+","+body_name+")"));
}
