/*
 * DiffractionForceModel.cpp
 *
 *  Created on: Oct 24, 2014
 *      Author: cady
 */

#include "DiffractionForceModel.hpp"




DiffractionForceModel::DiffractionForceModel(const DiffractionForceModel::Input& data, const std::string& body_name, const EnvironmentAndFrames& env)
    : AbstractRaoForceModel(data, body_name, env)
{}

DiffractionForceModel::DiffractionForceModel(const DiffractionForceModel::Input& data, const std::string& body_name, const EnvironmentAndFrames& env, const std::string& hdb_file_contents)
    : AbstractRaoForceModel(data, body_name, env, hdb_file_contents)
{}

std::string DiffractionForceModel::model_name()
{
    return AbstractRaoForceModel::get_model_name(YamlRAO::TypeOfRao::DIFFRACTION_RAO);
}

DiffractionForceModel::Input DiffractionForceModel::parse(const std::string& yaml)
{
    return AbstractRaoForceModel::parse(yaml, YamlRAO::TypeOfRao::DIFFRACTION_RAO);
}
