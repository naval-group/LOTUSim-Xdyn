/*
 * DefaultUWCurrentModel.hpp
 *
 *  Created on: 13 may 2024
 *      Author: julien.prodhon
 */

// Naval Group Far East - CoE
// Creating files for UW current class

#ifndef ENVIRONMENT_MODELS_INC_DEFAULTUWCurrentMODEL_HPP_
#define ENVIRONMENT_MODELS_INC_DEFAULTUWCurrentMODEL_HPP_

#include <string>

#include "UWCurrentModel.hpp"

class DefaultUWCurrentModel : public UWCurrentModel
{
public:
    DefaultUWCurrentModel(int); // Constructor argument is a dummy in order to be able to call DefaultUWCurrentModel(DefaultUWCurrentModel::parse(...)) from parser
    virtual ~DefaultUWCurrentModel();

    virtual Eigen::Vector3d get_UWCurrent(const Eigen::Vector3d& position, const double t, const double) const override;

    static std::string model_name();
    static int parse(const std::string&);
};

#endif /* ENVIRONMENT_MODELS_INC_DEFAULTUWCurrentMODEL_HPP_ */
