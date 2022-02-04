#include <algorithm>
#include <cmath>
#include <ssc/yaml_parser.hpp>
#include <ssc/kinematics.hpp>
#include "yaml.h"

#include "external_data_structures_parsers.hpp"
#include "AeroPolarForceModel.hpp"

std::string AeroPolarForceModel::model_name()
{
    return "aerodynamic polar";
}

AeroPolarForceModel::AeroPolarForceModel(const Input input, const std::string body_name_, const EnvironmentAndFrames& env):
        ForceModel(input.name, input.angle_command ? std::vector<std::string>({input.angle_command.get()}) : std::vector<std::string>(), body_name_, env),
        Cl(),
        Cd(),
        reference_area(input.reference_area),
        calculation_point(input.calculation_point_in_body_frame.x, input.calculation_point_in_body_frame.y, input.calculation_point_in_body_frame.z),
        symmetry(),
        angle_command(input.angle_command)
{
    const double min_beta = *std::min_element(input.apparent_wind_angle.begin(),input.apparent_wind_angle.end());
    const double max_beta = *std::max_element(input.apparent_wind_angle.begin(),input.apparent_wind_angle.end());
    const double eps = 0.1*M_PI/180;
    if (input.lift_coefficient.size()!=input.apparent_wind_angle.size() || input.drag_coefficient.size()!=input.apparent_wind_angle.size())
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "Apparent wind angle, lift coefficient and drag coefficient must all have the same size.")
    }
    if(min_beta > eps || max_beta < M_PI-eps)
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "Apparent wind angle must be provided from 0deg to either 180deg (symmetry) or 360deg.")
    }
    if (max_beta < M_PI+eps) // max_beta is close to pi
    {
        symmetry = true;
    }
    else if (max_beta < 2*M_PI-eps) // max_beta is between pi and 2*pi (but not close enough to either)
    {
        std::cerr << "WARNING: In an aerodynamic polar force model '" << name << "', you provided a maximum apparent wind angle between 180deg and 360deg. Symmetry will be assumed and values over 180deg will be ignored." << std::endl;
        symmetry = true;
    }
    else if (max_beta < 2*M_PI+eps) // max_beta is close to 2*pi
    {
        symmetry = false;
    }
    else // max_beta is over 2*pi (but not close enough)
    {
        std::cerr << "WARNING: In an aerodynamic polar force model '" << name << "', you provided a maximum apparent wind higher than 360deg. All values over 360deg will be ignored." << std::endl;
        symmetry = false;
    }
    Cl.reset(new ssc::interpolation::SplineVariableStep(input.apparent_wind_angle, input.lift_coefficient));
    Cd.reset(new ssc::interpolation::SplineVariableStep(input.apparent_wind_angle, input.drag_coefficient));
}

AeroPolarForceModel::Input AeroPolarForceModel::parse(const std::string& yaml)
{
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    Input ret;
    node["name"] >> ret.name;
    ssc::yaml_parser::parse_uv(node["AWA"], ret.apparent_wind_angle);
    node["lift coefficient"] >> ret.lift_coefficient;
    node["drag coefficient"] >> ret.drag_coefficient;
    ssc::yaml_parser::parse_uv(node["reference area"], ret.reference_area);
    node["calculation point in body frame"] >> ret.calculation_point_in_body_frame;
    parse_optional(node, "angle command", ret.angle_command);
    return ret;
}

Wrench AeroPolarForceModel::get_force(const BodyStates& states, const double t, const EnvironmentAndFrames& env, const std::map<std::string,double>& commands) const
{
    using namespace std;
    const Eigen::Vector3d omega(states.p(), states.q(), states.r());
    const Eigen::Vector3d Vg(states.u(), states.v(), states.w());
    const Eigen::Vector3d Vp = Vg - calculation_point.cross(omega);
    const auto rotation = states.get_rot_from_ned_to_body();
    const Eigen::Vector3d application_point_in_NED = states.G.v + rotation*calculation_point;
    const Eigen::Vector3d wind_in_NED = env.wind->get_wind(application_point_in_NED, t);
    const Eigen::Vector3d true_wind_in_body_frame = rotation.transpose()*wind_in_NED;
    const Eigen::Vector3d W = true_wind_in_body_frame - Vp; // Apparent wind in body frame
    const double U = sqrt(W(0)*W(0) + W(1)*W(1)); // Apparent wind speed projected in the body horizontal plane
    double beta = atan2(-W(1), -W(0)); // Incident angle of the flow, in [-pi,pi]
    double alpha = beta; // Angle of attack
    if (angle_command)
    {
        alpha -= commands.at(angle_command.get());
    }
    alpha = remainder(alpha, 2*M_PI); // Putting alpha in [-pi,pi]
    if (alpha<0)
    {
        alpha += 2*M_PI; // Putting alpha in [0,2*pi]
    }
    if (symmetry && alpha>M_PI)
    {
        alpha = 2*M_PI - alpha;
    }
    const double lift = 0.5*Cl->f(alpha)*env.get_rho_air()*pow(U, 2)*reference_area;
    const double drag = 0.5*Cd->f(alpha)*env.get_rho_air()*pow(U, 2)*reference_area;
    Wrench ret(ssc::kinematics::Point(body_name, calculation_point), body_name);
    if (beta>=0.) // Starboard wind
    {
        ret.X() = -drag*cos(beta) + lift*sin(beta);
        ret.Y() = -drag*sin(beta) - lift*cos(beta);
    }
    else // Port wind
    {
        ret.X() = -drag*cos(beta) - lift*sin(beta);
        ret.Y() = -drag*sin(beta) + lift*cos(beta);
    }
    return ret;
}
