/*
 * ConstantForce.cpp
 *
 *  Created on: Sep 7, 2018
 *      Author: cady
 */


#include "ConstantForceModel.hpp"

#include "Body.hpp"
#include "yaml.h"
#include <ssc/macros.hpp>
#include <ssc/yaml_parser.hpp>

#include <ssc/kinematics.hpp>

std::string ConstantForceModel::model_name() {return "constant force";}

ConstantForceModel::Input::Input() :
        frame(),
        x(),
        y(),
        z(),
        X(),
        Y(),
        Z(),
        K(),
        M(),
        N()
{
}

ConstantForceModel::Input ConstantForceModel::parse(const std::string& yaml)
{
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    ConstantForceModel::Input ret;
    node["frame"] >> ret.frame;
    ssc::yaml_parser::parse_uv(node["x"], ret.x);
    ssc::yaml_parser::parse_uv(node["y"], ret.y);
    ssc::yaml_parser::parse_uv(node["z"], ret.z);
    ssc::yaml_parser::parse_uv(node["X"], ret.X);
    ssc::yaml_parser::parse_uv(node["Y"], ret.Y);
    ssc::yaml_parser::parse_uv(node["Z"], ret.Z);
    ssc::yaml_parser::parse_uv(node["K"], ret.K);
    ssc::yaml_parser::parse_uv(node["M"], ret.M);
    ssc::yaml_parser::parse_uv(node["N"], ret.N);
    return ret;
}

ConstantForceModel::ConstantForceModel(const ConstantForceModel::Input& input, const std::string& body_name_, const EnvironmentAndFrames& env_) :
        ForceModel(model_name(), {}, YamlPosition(YamlCoordinates(input.x, input.y, input.z),YamlAngle(),input.frame), body_name_, env_),
        force(),
        torque()
{
    force << input.X
           , input.Y
           , input.Z;
    torque << input.K
            , input.M
            , input.N;
}

Wrench ConstantForceModel::get_force(const BodyStates&, const double, const EnvironmentAndFrames&, const std::map<std::string,double>&) const
{
    return Wrench(ssc::kinematics::Point(name,0,0,0), name, force, torque);
}

