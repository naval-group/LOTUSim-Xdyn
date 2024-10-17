/*
 * EkmanUWCurrentModel.hpp
 *
 *  Created on: 21 may 2024
 *      Author: julien.prodhon
 */

// Naval Group Far East - CoE
// Creating files for UW current class

#ifndef ENVIRONMENT_MODELS_INC_EkmanUWCurrentModel_HPP_
#define ENVIRONMENT_MODELS_INC_EkmanUWCurrentModel_HPP_

#include <string>

#include "UWCurrentModel.hpp"

class EkmanUWCurrentModel : public UWCurrentModel
{
public:

    struct Input
    {
        Input();
        Seabed seabed;
        double rho;
        double dTop;
        double dBottom;
        double velocity;
        double phi;
        double omega;
        Eigen::Vector2d windTau;
    };

    EkmanUWCurrentModel(const Input& input); // Constructor argument is a dummy in order to be able to call DefaultUWCurrentModel(DefaultUWCurrentModel::parse(...)) from parser

    EkmanUWCurrentModel(int); 
    virtual ~EkmanUWCurrentModel();

    virtual Eigen::Vector3d get_UWCurrent(const Eigen::Vector3d&, const double t, const double wave_height) const override;

    virtual Eigen::Vector3d TopLayer(const Eigen::Vector3d&, const Eigen::Vector3d&) const;
    virtual Eigen::Vector3d MiddleLayer() const;
    virtual Eigen::Vector3d BottomLayer(const Eigen::Vector3d&, const Eigen::Vector3d&) const;

    static std::string model_name();
    static Input parse(const std::string&);

private:
    Seabed seabed;
    double rho;
    double dTop;
    double dBottom;
    double velocity;
    double phi;
    double omega;
    Eigen::Vector2d windTau;
};

#endif /* ENVIRONMENT_MODELS_INC_EkmanUWCurrentModel_HPP_ */
