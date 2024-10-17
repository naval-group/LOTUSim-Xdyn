/*
 * ConstantUWCurrentModel.hpp
 *
 *  Created on: 21 may 2024
 *      Author: julien.prodhon
 */

// Naval Group Far East - CoE 
// Creating files for UW current class

#ifndef ENVIRONMENT_MODELS_INC_CONSTANTUWCurrentMODEL_HPP_
#define ENVIRONMENT_MODELS_INC_CONSTANTUWCurrentMODEL_HPP_

#include <string>
#include <map>

#include "UWCurrentModel.hpp"

class ConstantUWCurrentModel : public UWCurrentModel
{
public:
    struct Input
    {
        Input();
        double velocity;
        double orientation;
    };
    ConstantUWCurrentModel(const Input& input);     
    ConstantUWCurrentModel(int); 
    virtual ~ConstantUWCurrentModel();

    virtual Eigen::Vector3d get_UWCurrent(const Eigen::Vector3d&, const double, const double) const override;

    static std::string model_name();
    static Input parse(const std::string&);

private:
    double velocity;
    double orientation;
};

#endif /* ENVIRONMENT_MODELS_INC_CONSTANTUWCurrentMODEL_HPP_ */
