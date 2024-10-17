/*
 * DnvrpUWCurrentModel.hpp
 *
 *  Created on: 13 may 2024
 *      Author: julien.prodhon
 */

// Naval Group Far East - CoE 
// Creating files for UW current class

#ifndef ENVIRONMENT_MODELS_INC_DNVRPUWCurrentMODEL_HPP_
#define ENVIRONMENT_MODELS_INC_DNVRPUWCurrentMODEL_HPP_

#include <string>

#include "UWCurrentModel.hpp"

class DnvrpUWCurrentModel : public UWCurrentModel
{
public:
    struct Input
    {
        Input();
        double orientation;
        double tide_velocity;
        double Vw0;
        double alpha;
        double upper_depth;
    };
    DnvrpUWCurrentModel(const Input& input);     
    DnvrpUWCurrentModel(int); 
    virtual ~DnvrpUWCurrentModel();

    virtual Eigen::Vector3d get_UWCurrent(const Eigen::Vector3d& position, const double t, const double) const override;

    static std::string model_name();
    static Input parse(const std::string&);

private:
    double orientation;
    double tide_velocity;
    double Vw0; // wind
    double alpha; // for tide
    double upper_depth; // depth 0
};

#endif /* ENVIRONMENT_MODELS_INC_DNVRPUWCurrentMODEL_HPP_ */
