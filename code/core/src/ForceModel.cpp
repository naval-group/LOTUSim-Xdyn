/*
 * ForceModel.cpp
 *
 *  Created on: Oct 22, 2014
 *      Author: cady
 */

#include "ForceModel.hpp"

#include "InvalidInputException.hpp"
#include "InternalErrorException.hpp"
#include "Observer.hpp"
#include "yaml2eigen.hpp"

#include <ssc/data_source.hpp>

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

ForceModel::ForceModel(const std::string& name_, const std::vector<std::string>& commands_, const YamlPosition& internal_frame, const std::string& body_name_, const EnvironmentAndFrames& env_) :
    commands(commands_),
    name(name_),
    body_name(body_name_),
    has_internal_frame(true),
    known_reference_frame(internal_frame.frame),
    latest_force_in_body_frame(ssc::kinematics::Point(body_name))
{
    env_.k->add(make_transform(internal_frame, name, env_.rot));
}

ForceModel::ForceModel(const std::string& name_, const std::vector<std::string>& commands_, const std::string& body_name_, const EnvironmentAndFrames&) :
    commands(commands_),
    name(name_),
    body_name(body_name_),
    has_internal_frame(false),
    known_reference_frame(),
    latest_force_in_body_frame(ssc::kinematics::Point(body_name))
{
}

ForceModel::~ForceModel()
{
}

std::string ForceModel::get_name() const
{
    return name;
}

std::map<std::string,double> ForceModel::get_commands(ssc::data_source::DataSource& command_listener, const double t) const
{
    std::map<std::string,double> ret;
    for (auto that_command = commands.begin() ; that_command != commands.end() ; ++that_command)
    {
        ret[*that_command] = get_command(*that_command, command_listener, t);
    }
    auto m = command_listener.get_all<double>();
    ret.insert(m.begin(),m.end());
    return ret;
}

ssc::kinematics::Wrench ForceModel::operator()(const BodyStates& states, const double t, const EnvironmentAndFrames& env, ssc::data_source::DataSource& command_listener)
{
    auto F = get_force(states, t, env, get_commands(command_listener,t));
    can_find_internal_frame(env.k);
    F.change_point_and_frame(states.G, body_name, env.k);
    latest_force_in_body_frame = ssc::kinematics::Wrench(states.G, F.to_vector());
    return latest_force_in_body_frame;
}

ssc::kinematics::Wrench ForceModel::operator()(const BodyStates& states, const double t, const EnvironmentAndFrames& env)
{
    if (not(commands.empty()))
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "This force model needs commands (namely " << commands << ") but you didn't supply any to ForceModel::operator(). Please note there are two versions of ForceModel::operator(): one with a DataSource (which must contain the force commands) and another without. In this case you need the version with the DataSource.");
    }
    ssc::data_source::DataSource ds;
    return operator()(states, t, env, ds);
}

double ForceModel::get_command(const std::string& command_name, ssc::data_source::DataSource& command_listener, const double t) const
{
    double ret = 0;
    try
    {
        command_listener.check_in(__PRETTY_FUNCTION__);
        command_listener.set("t", t);
        ret = command_listener.get<double>(name + "(" + command_name + ")");
        command_listener.check_out();
    }
    catch (const ssc::data_source::DataSourceException& e)
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException,
                "Unable to retrieve command '" << command_name << "' for '" << name << "': " << e.get_message()
                << " Check that the YAML file containing the commands was supplied to the simulator & that the command exists in that file."
                );
    }
    return ret;
}

void ForceModel::can_find_internal_frame(const ssc::kinematics::KinematicsPtr& k) const
{
    if (has_internal_frame)
    {
        bool reference_frame_exists = false;
        try
        {
            k->get(body_name, known_reference_frame);
            reference_frame_exists = true;
        }
        catch (const ssc::kinematics::KinematicsException& e)
        {
        }
        try
        {
            k->get("NED", known_reference_frame);
            reference_frame_exists = true;
        }
        catch (const ssc::kinematics::KinematicsException& e)
        {
        }
        if (not(reference_frame_exists))
        {
            THROW(__PRETTY_FUNCTION__, InvalidInputException, "When computing force model '" << name << "' we were unable to find frame '" << known_reference_frame << "' used to express the reference frame in which the forces are expressed. Use 'NED' or '" << body_name << "' in the 'frame' section perhaps?");
        }
        try
        {
            k->get(body_name, name);
        }
        catch (const ssc::kinematics::KinematicsException& e)
        {
            THROW(__PRETTY_FUNCTION__, InternalErrorException, "When computing force model '" << name << "' we were unable to find frame '" << name << "' in which the forces are expressed. If the force model uses an internal frame, it must be provided to the ForceModel constructor.");
        }
    }
}

void ForceModel::feed(Observer& observer, ssc::kinematics::KinematicsPtr& k) const
{
    // G is the point in which 'latest_force_in_body_frame' is expressed (sum of forces)
    // O is the origin of the NED frame
    // P is the origin of the ForceModel's internal frame

    const Wrench tau_in_body_frame_at_G(latest_force_in_body_frame.get_point(), latest_force_in_body_frame.get_frame(), latest_force_in_body_frame.to_vector());
    const Wrench tau_in_ned_frame_at_G = tau_in_body_frame_at_G.change_frame("NED", k);

    if (has_internal_frame)
    {
        can_find_internal_frame(k);
        const Wrench tau_in_internal_frame_at_P = tau_in_body_frame_at_G.change_point_and_frame(ssc::kinematics::Point(name,0,0,0), name, k);
        observer.write(tau_in_internal_frame_at_P.X(),DataAddressing({"efforts",body_name,name,name,"Fx"},std::string("Fx(")+name+","+body_name+","+name+")"));
        observer.write(tau_in_internal_frame_at_P.Y(),DataAddressing({"efforts",body_name,name,name,"Fy"},std::string("Fy(")+name+","+body_name+","+name+")"));
        observer.write(tau_in_internal_frame_at_P.Z(),DataAddressing({"efforts",body_name,name,name,"Fz"},std::string("Fz(")+name+","+body_name+","+name+")"));
        observer.write(tau_in_internal_frame_at_P.K(),DataAddressing({"efforts",body_name,name,name,"Mx"},std::string("Mx(")+name+","+body_name+","+name+")"));
        observer.write(tau_in_internal_frame_at_P.M(),DataAddressing({"efforts",body_name,name,name,"My"},std::string("My(")+name+","+body_name+","+name+")"));
        observer.write(tau_in_internal_frame_at_P.N(),DataAddressing({"efforts",body_name,name,name,"Mz"},std::string("Mz(")+name+","+body_name+","+name+")"));
    }

    observer.write(tau_in_body_frame_at_G.X(),DataAddressing({"efforts",body_name,name,body_name,"Fx"},std::string("Fx(")+name+","+body_name+","+body_name+")"));
    observer.write(tau_in_body_frame_at_G.Y(),DataAddressing({"efforts",body_name,name,body_name,"Fy"},std::string("Fy(")+name+","+body_name+","+body_name+")"));
    observer.write(tau_in_body_frame_at_G.Z(),DataAddressing({"efforts",body_name,name,body_name,"Fz"},std::string("Fz(")+name+","+body_name+","+body_name+")"));
    observer.write(tau_in_body_frame_at_G.K(),DataAddressing({"efforts",body_name,name,body_name,"Mx"},std::string("Mx(")+name+","+body_name+","+body_name+")"));
    observer.write(tau_in_body_frame_at_G.M(),DataAddressing({"efforts",body_name,name,body_name,"My"},std::string("My(")+name+","+body_name+","+body_name+")"));
    observer.write(tau_in_body_frame_at_G.N(),DataAddressing({"efforts",body_name,name,body_name,"Mz"},std::string("Mz(")+name+","+body_name+","+body_name+")"));

    observer.write(tau_in_ned_frame_at_G.X(),DataAddressing({"efforts",body_name,name,"NED","Fx"},std::string("Fx(")+name+","+body_name+",NED)"));
    observer.write(tau_in_ned_frame_at_G.Y(),DataAddressing({"efforts",body_name,name,"NED","Fy"},std::string("Fy(")+name+","+body_name+",NED)"));
    observer.write(tau_in_ned_frame_at_G.Z(),DataAddressing({"efforts",body_name,name,"NED","Fz"},std::string("Fz(")+name+","+body_name+",NED)"));
    observer.write(tau_in_ned_frame_at_G.K(),DataAddressing({"efforts",body_name,name,"NED","Mx"},std::string("Mx(")+name+","+body_name+",NED)"));
    observer.write(tau_in_ned_frame_at_G.M(),DataAddressing({"efforts",body_name,name,"NED","My"},std::string("My(")+name+","+body_name+",NED)"));
    observer.write(tau_in_ned_frame_at_G.N(),DataAddressing({"efforts",body_name,name,"NED","Mz"},std::string("Mz(")+name+","+body_name+",NED)"));

    extra_observations(observer);
}

double ForceModel::get_Tmax() const
{
    return 0.;
}

std::string ForceModel::get_body_name() const
{
    return body_name;
}


bool ForceModel::is_a_surface_force_model() const
{
    return false;
}

void ForceModel::extra_observations(Observer&) const
{
}
