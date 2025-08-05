/*
 * EkmanUWCurrentModel.hpp
 *
 *  Created on: 5 august 2025
 *      Author: julien.prodhon
 */

#ifndef ENVIRONMENT_MODELS_INC_EKMANUWCURRENTMODEL_HPP_
#define ENVIRONMENT_MODELS_INC_EKMANUWCURRENTMODEL_HPP_

#include "UWCurrentModel.hpp"
#include "Seabed.hpp"
#include <string>

/**
 * @brief Ekman spiral model for underwater current simulation
 * 
 * Implements a three-layer current model:
 * - Top layer: Wind-driven Ekman spiral
 * - Middle layer: Uniform background current
 * - Bottom layer: Seabed-influenced current
 */

class EkmanUWCurrentModel : public UWCurrentModel
{
public:
    struct Input
    {
        Input();
        Input(Seabed);
        Seabed seabed;              ///< Seabed bathymetry
        double top_ekman_depth;     ///< Top layer depth (m)
        double bottom_ekman_depth;  ///< Bottom layer depth (m)
        double middle_velocity;     ///< middle current velocity (m/s)
        double middle_orientation;  ///< middle current direction (rad)
        double f_and_sqrt_rho;
        double wind_angle;          ///< wind angle (rad)
        double wind_stress;         ///< Wind stress vector (N/m²)
    };
    EkmanUWCurrentModel(const Input& input); 
    EkmanUWCurrentModel(int); // dummy in order to call DefaultUWCurrentModel(DefaultUWCurrentModel::parse(...)) from parser
    virtual ~EkmanUWCurrentModel();
    virtual Eigen::Vector3d get_UWCurrent(const Eigen::Vector3d& position, 
                                         const double time, 
                                         const double wave_height) const override;
    static std::string model_name();
    static Input parse(const std::string&);

private:
    // Model parameters
    Seabed seabed;                  ///< Seabed bathymetry
    double top_ekman_depth;         ///< Top layer depth (m)
    double bottom_ekman_depth;      ///< Bottom layer depth (m)
    double middle_velocity;         ///< middle current velocity (m/s)
    double middle_orientation;      ///< middle current direction (rad)
    double f_and_sqrt_rho;
    double wind_angle;              ///< wind angle (rad)
    double wind_stress;             ///< Wind stress vector (N/m²)
    // Layer-specific current calculations
    virtual Eigen::Vector3d getTopLayerCurrent(const Eigen::Vector3d&, const Eigen::Vector3d&, const double) const;
    virtual Eigen::Vector3d getMiddleLayerCurrent() const;
    virtual Eigen::Vector3d getBottomLayerCurrent(const Eigen::Vector3d&, const Eigen::Vector3d&, const double) const;
};

#endif /* EKMANUWCURRENTMODEL_HPP_ */
