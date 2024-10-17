/*
 * DefaultUWCurrentModel.cpp
 *
 *  Created on: 13 may 2024
 *      Author: julien.prodhon
 */

// Naval Group Far East - CoE
// Creating files for UW current class

#include "DefaultUWCurrentModel.hpp"

DefaultUWCurrentModel::DefaultUWCurrentModel(int)
{
}

DefaultUWCurrentModel::~DefaultUWCurrentModel()
{
}

std::string DefaultUWCurrentModel::model_name() {return "no UW current";}

int DefaultUWCurrentModel::parse(const std::string&)
{
    return 0;
}

Eigen::Vector3d DefaultUWCurrentModel::get_UWCurrent(const Eigen::Vector3d&, const double, const double) const
{
    return Eigen::Vector3d::Zero();
}
