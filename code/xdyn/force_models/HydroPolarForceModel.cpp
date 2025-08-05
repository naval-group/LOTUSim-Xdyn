#include "HydroPolarForceModel.hpp"
#include "xdyn/yaml_parser/external_data_structures_parsers.hpp"
#include "yaml.h"
#include <ssc/yaml_parser.hpp>
#include <ssc/kinematics.hpp>
#include <algorithm>
#include <cmath>

HydroPolarForceModel::Input::Input():
    name(),
    internal_frame(),
    angle_of_attack(),
    lift_coefficient(),
    drag_coefficient(),
    moment_coefficient(),
    reference_area(0.0),
    chord_length(),
    use_waves_velocity(false),
    angle_command()
{}

std::string HydroPolarForceModel::model_name()
{
    return "hydrodynamic polar";
}

HydroPolarForceModel::HydroPolarForceModel(const Input& input, const std::string& body_name_, const EnvironmentAndFrames& env):
        ForceModel(input.name, input.angle_command ? std::vector<std::string>({input.angle_command.get()}) : std::vector<std::string>(), input.internal_frame, body_name_, env),
        Cl(),
        Cd(),
        Cm(),
        reference_area(input.reference_area),
        chord_length(input.chord_length),
        symmetry(),
        use_waves_velocity(input.use_waves_velocity),
        angle_command(input.angle_command),
        relative_velocity(new double(0)),
        angle_of_attack(new double(0))
        // relative_velocity & angle_of_attack need to be stored for outputting, but because ForceModel::get_force(...) is const, it can't modify normal variables. That's why they're hidden behind pointers, to break constness
{
    if (input.lift_coefficient.size()==0)
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "An empty vector was provided for the angle of attack, which must be provided from either -180° or 0deg (symmetry) to 180deg.")
    }
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
    ret.lift_coefficient = extract_vector_of_doubles(node, "lift coefficient");
    ret.drag_coefficient = extract_vector_of_doubles(node, "drag coefficient");
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
    parse_optional(node, "angle command", ret.angle_command);
    return ret;
}

Wrench HydroPolarForceModel::get_force(const BodyStates& states, const double t, const EnvironmentAndFrames& env, const std::map<std::string,double>& commands) const
{
    // NED, bodyNED (local body NED) and body frames are defined here (https://sirehna.github.io/xdyn/#rep%C3%A8res)
    Wrench force(ssc::kinematics::Point(name,0,0,0), name);
    const ssc::kinematics::RotationMatrix Rot_NED_to_body = states.get_rot_from_ned_to_body();
    const ssc::kinematics::RotationMatrix Rot_body_to_NED = Rot_NED_to_body.transpose();
    const ssc::kinematics::RotationMatrix Rot_NED_to_name = Rot_NED_to_body * env.k->get(name, body_name).get_rot();
    // The following variables follow the naming Variable_Object_Frame
    // P stands for position (x,y,z), V for the velocity (u,v,w)
    const Eigen::Vector3d P_body_NED(states.x(), states.y(), states.z());
    const Eigen::Vector3d V_body_NED(states.u(), states.v(), states.w());
    // Omega is the angular velocities (p,q,r) with the 3-2-1 euler convention
    const Eigen::Vector3d Omega_body(states.p(), states.q(), states.r());
    const Eigen::Vector3d P_name_body = env.k->get(body_name, name).get_point().v;
    const Eigen::Vector3d P_name_NED = P_body_NED + Rot_body_to_NED * P_name_body;
    const Eigen::Vector3d V_name_NED = V_body_NED + Rot_body_to_NED * Omega_body.cross(P_name_body);
    // Velocity of water flow in name frame
    Eigen::Vector3d V_water_name = Rot_NED_to_name * V_name_NED; 
    double water_surface_height = 0.;
    if (env.w.use_count())
    {
        const std::vector<double> wave_height = env.w->get_and_check_wave_height({P_name_NED(0)}, {P_name_NED(1)}, t);
        water_surface_height = wave_height.at(0);
        if (use_waves_velocity)
        {
            const ssc::kinematics::PointMatrix wave_velocity_NED = env.w->get_and_check_orbital_velocity(env.g, {P_name_NED(0)}, {P_name_NED(1)}, {P_name_NED(2)}, t, wave_height);
            const Eigen::Vector3d V_waves_NED(wave_velocity_NED.m(0,0), wave_velocity_NED.m(1,0), wave_velocity_NED.m(2,0)); 
            V_water_name -= Rot_NED_to_name * V_waves_NED; // Adding orbital velocities effect
        }
    }
    // Naval Group Far East
    Eigen::Vector3d V_Current_NED = env.get_UWCurrent(P_name_NED,t);
    V_water_name -= Rot_NED_to_name * V_Current_NED;
    // stop
    if (P_name_NED(2) < water_surface_height)
    {
        std::cerr << "WARNING: In hydrodynamic polar force model '" << name << "', the calculation point isn't in the water (z = " << P_name_NED(2) << "). In consequence, no force is being applied by this model." << std::endl;
        return force;
    }
    // Incident angle of the flow, in [-pi,pi]
    const double beta = - atan2(V_water_name(1), V_water_name(0)); 
    // Apparent flow velocity projected in the (x,y) plane of the internal frame
    const double U = std::hypot(V_water_name(0), V_water_name(1)); 
    // Angle of attack in [-pi,pi]
    const double alpha = remainder( (angle_command) ? beta + commands.at(angle_command.get()) : beta , 2*M_PI); 
    const double alpha_sign = (alpha >= 0) ? +1 : -1;
    const double alpha_prime = symmetry ? abs(alpha) : alpha;
    const double lift = 0.5 * Cl->f(alpha_prime) * env.rho * U * U * reference_area;
    const double drag = 0.5 * Cd->f(alpha_prime) * env.rho * U * U * reference_area;
    force.X() = - drag * cos(beta) + alpha_sign * lift * sin(beta);
    force.Y() = drag * sin(beta) + alpha_sign * lift * cos(beta);
    if (Cm)
    {
        const double normalized_cubic_length = chord_length.is_initialized() ? reference_area * chord_length.get() : pow(reference_area, 1.5);
        const double moment = 0.5 * Cm->f(alpha_prime) * env.rho * U * U * normalized_cubic_length;
        force.N() = alpha_sign * moment;
    }
    *angle_of_attack = alpha;
    *relative_velocity = U;
    return force;
}

void HydroPolarForceModel::extra_observations(Observer& observer) const
{
    observer.write_before_solver_step(*angle_of_attack, DataAddressing({"efforts",body_name,name,"alpha"},std::string("alpha(")+name+","+body_name+")"));
    observer.write_before_solver_step(*relative_velocity, DataAddressing({"efforts",body_name,name,"U"},std::string("U(")+name+","+body_name+")"));
}
