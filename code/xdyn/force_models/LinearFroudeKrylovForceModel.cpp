/*
 * LinearFroudeKrylovForceModel.cpp
 *
 *  Created on: July 16, 2021
 *      Author: cady
 */

#include "LinearFroudeKrylovForceModel.hpp"


LinearFroudeKrylovForceModel::LinearFroudeKrylovForceModel(const LinearFroudeKrylovForceModel::Input& data, const std::string& body_name, const EnvironmentAndFrames& env)
    : AbstractRaoForceModel(data, body_name, env)
{}

LinearFroudeKrylovForceModel::LinearFroudeKrylovForceModel(const LinearFroudeKrylovForceModel::Input& data, const std::string& body_name, const EnvironmentAndFrames& env, const std::string& hdb_file_contents)
    : AbstractRaoForceModel(data, body_name, env, hdb_file_contents)
{}

std::string LinearFroudeKrylovForceModel::model_name()
{
    return AbstractRaoForceModel::get_model_name(YamlRAO::TypeOfRao::FROUDE_KRYLOV_RAO);
}

LinearFroudeKrylovForceModel::Input LinearFroudeKrylovForceModel::parse(const std::string& yaml)
{
    return AbstractRaoForceModel::parse(yaml, YamlRAO::TypeOfRao::FROUDE_KRYLOV_RAO);
}
