#include <algorithm>
#include <cmath>
#include <ssc/yaml_parser.hpp>
#include <ssc/kinematics.hpp>
#include "yaml.h"

#include "external_data_structures_parsers.hpp"
#include "HydroPolarForceModel.hpp"

std::string HydroPolarForceModel::model_name()
{
    return "hydrodynamic polar";
}

HydroPolarForceModel::HydroPolarForceModel(const Input input, const std::string body_name_, const EnvironmentAndFrames& env):
        ForceModel(input.name, {}, input.internal_frame, body_name_, env),
        Cl(),
        Cd(),
        Cm(),
        reference_area(input.reference_area),
        chord_length(input.chord_length),
        symmetry(),
        use_waves_velocity(input.use_waves_velocity),
        relative_velocity(new double(0)),
        angle_of_attack(new double(0))
{
    const double min_alpha = *std::min_element(input.angle_of_attack.begin(),input.angle_of_attack.end());
    const double max_alpha = *std::max_element(input.angle_of_attack.begin(),input.angle_of_attack.end());
    const double eps = 0.1*M_PI/180;
    if (input.lift_coefficient.size()!=input.angle_of_attack.size() || input.drag_coefficient.size()!=input.angle_of_attack.size())
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "Angle of attack, lift coefficient and drag coefficient must all have the same size.")
    }
    if(min_alpha > eps || max_alpha < M_PI-eps)
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "Angle of attack must be provided from either -180° or 0deg (symmetry) to 180deg.")
    }
    if (max_alpha > M_PI+eps)
    {
        std::cerr << "WARNING: In hydrodynamic polar force model '" << name << "', you provided a maximum angle of attack higher than 180deg. All values over 180deg will be ignored." << std::endl;
    }
    if (min_alpha > -eps) // min_alpha is close to 0
    {
        symmetry = true;
    }
    else if (min_alpha > -M_PI+eps) // min_alpha is between -pi and 0 (but not close enough to either)
    {
        std::cerr << "WARNING: In hydrodynamic polar force model '" << name << "', you provided a minimum angle of attack between -180deg and 0deg. Symmetry will be assumed and values under 0deg will be ignored." << std::endl;
        symmetry = true;
    }
    else if (min_alpha > -M_PI-eps) // min_alpha is close to -pi
    {
        symmetry = false;
    }
    else // min_alpha is under -pi (but not close enough)
    {
        std::cerr << "WARNING: In hydrodynamic polar force model '" << name << "', you provided a minimum angle of attack lower than -180deg. All values under -180deg will be ignored." << std::endl;
        symmetry = false;
    }
    Cl.reset(new ssc::interpolation::SplineVariableStep(input.angle_of_attack, input.lift_coefficient));
    Cd.reset(new ssc::interpolation::SplineVariableStep(input.angle_of_attack, input.drag_coefficient));
    if (input.moment_coefficient.is_initialized())
    {
        if (input.moment_coefficient.get().size()!=input.angle_of_attack.size())
        {
            THROW(__PRETTY_FUNCTION__, InvalidInputException, "Angle of attack and moment coefficient must have the same size.")
        }
        Cm.reset(new ssc::interpolation::SplineVariableStep(input.angle_of_attack, input.moment_coefficient.get()));
    }
    if (use_waves_velocity && env.w.use_count()==0)
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "In order to take into account the orbital velocity of waves, a wave model must be defined in the 'environment models' section.")
    }
}

HydroPolarForceModel::Input HydroPolarForceModel::parse(const std::string& yaml)
{
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    Input ret;
    node["name"] >> ret.name;
    ssc::yaml_parser::parse_uv(node["angle of attack"], ret.angle_of_attack);
    node["lift coefficient"] >> ret.lift_coefficient;
    node["drag coefficient"] >> ret.drag_coefficient;
    parse_optional(node, "moment coefficient", ret.moment_coefficient);
    ssc::yaml_parser::parse_uv(node["reference area"], ret.reference_area);
    if (node.FindValue("chord length"))
    {
        double cord_length; // Intermediate value is necessary to call ssc::yaml_parser::parse_uv
        ssc::yaml_parser::parse_uv(node["chord length"], cord_length);
        ret.chord_length = cord_length;
    }
    node["position of calculation frame"] >> ret.internal_frame;
    node["take waves orbital velocity into account"] >> ret.use_waves_velocity;
    return ret;
}

Wrench HydroPolarForceModel::get_force(const BodyStates& states, const double t, const EnvironmentAndFrames& env, const std::map<std::string,double>& commands) const
{
    using namespace std;
    const Eigen::Vector3d omega(states.p(), states.q(), states.r());
    const Eigen::Vector3d Vg(states.u(), states.v(), states.w());
    const auto T = env.k->get(name, body_name);
    const Eigen::Vector3d P_body = T.get_point().v; // Coordinates of point P in body frame
    const Eigen::Vector3d Vp_body = Vg - P_body.cross(omega); // Velocity of point P of body relative to NED, expressed in body frame
    Eigen::Vector3d Vp = T.get_rot()*Vp_body; // Velocity of P in fluid, expressed in internal frame
    const auto rotation = states.get_rot_from_ned_to_body();
    const Eigen::Vector3d P_NED = states.G.v + rotation*P_body; // Coordinates of point P in NED frame
    double water_height = 0.;
    if (env.w.use_count())
    {
        const auto wave_height = env.w->get_and_check_wave_height({P_NED(0)}, {P_NED(1)}, t);
        if (use_waves_velocity)
        {
            const auto wave_velocity_matrix = env.w->get_and_check_orbital_velocity(env.g, {P_NED(0)}, {P_NED(1)}, {P_NED(2)}, t, wave_height);
            const Eigen::Vector3d Vw_NED(wave_velocity_matrix.m(0,0), wave_velocity_matrix.m(1,0), wave_velocity_matrix.m(2,0)); // Velocity of wave flow
            Vp -= env.k->get(name, wave_velocity_matrix.get_frame()).get_rot()*Vw_NED;
        }
        water_height = wave_height.at(0);
    }
    if (P_NED(2) > water_height)
    {
        std::cerr << "WARNING: In hydrodynamic polar force model '" << name << "', the calculation point seems to be outside of the water (z = " << P_NED(2) << ")." << std::endl;
    }
    const double alpha = -atan2(Vp(1), Vp(0));
    const double U = sqrt(pow(Vp(0), 2) + pow(Vp(1), 2)); // Apparent flow velocity projected in the (x,y) plane of the internal frame
    double alpha_prime;
    if (symmetry && alpha<M_PI/2)
    {
        alpha_prime = -alpha;
    }
    else
    {
        alpha_prime = alpha;
    }
    const double lift = 0.5*Cl->f(alpha_prime)*env.rho*pow(U, 2)*reference_area;
    const double drag = 0.5*Cd->f(alpha_prime)*env.rho*pow(U, 2)*reference_area;
    Wrench ret(ssc::kinematics::Point(name,0,0,0), name);
    if (alpha>=0)
    {
        ret.X() = -drag*cos(alpha) + lift*sin(alpha);
        ret.Y() =  drag*sin(alpha) + lift*cos(alpha);
    }
    else
    {
        ret.X() = -drag*cos(alpha) - lift*sin(alpha);
        ret.Y() =  drag*sin(alpha) - lift*cos(alpha);
    }
    if (Cm)
    {
        double normalization_cubic_length;
        if (chord_length.is_initialized())
        {
            normalization_cubic_length = reference_area*chord_length.get();
        }
        else
        {
            normalization_cubic_length = pow(reference_area, 1.5);
        }
        const double moment = 0.5*Cm->f(alpha_prime)*env.rho*pow(U, 2)*normalization_cubic_length;
        if (alpha>=0)
        {
            ret.N() = moment;
        }
        else
        {
            ret.N() = -moment;
        }
    }
    *angle_of_attack = alpha;
    *relative_velocity = U;
    return ret;
}

void HydroPolarForceModel::extra_observations(Observer& observer) const
{
    observer.write(*angle_of_attack, DataAddressing({"efforts",body_name,name,"alpha"},std::string("alpha(")+name+","+body_name+")"));
    observer.write(*relative_velocity, DataAddressing({"efforts",body_name,name,"U"},std::string("U(")+name+","+body_name+")"));
}
