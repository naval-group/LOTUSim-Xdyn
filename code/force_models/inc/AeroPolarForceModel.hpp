#ifndef FORCE_MODELS_INC_AEROPOLARFORCEMODEL_HPP_
#define FORCE_MODELS_INC_AEROPOLARFORCEMODEL_HPP_

#include <memory>
#include <ssc/interpolation.hpp>
#include <Eigen/Dense>

#include "ForceModel.hpp"

class AeroPolarForceModel : public ForceModel
{
    public:
        struct Input
        {
            std::string name;
            YamlCoordinates calculation_point_in_body_frame;
            std::vector<double> apparent_wind_angle;
            std::vector<double> lift_coefficient;
            std::vector<double> drag_coefficient;
            double reference_area;
        };

        AeroPolarForceModel(const Input input, const std::string body_name, const EnvironmentAndFrames& env);
        AeroPolarForceModel() = delete;
        virtual ~AeroPolarForceModel() = default;

        static std::string model_name();
        static Input parse(const std::string& yaml_input);
        Wrench get_force(const BodyStates& states, const double t, const EnvironmentAndFrames& env, const std::map<std::string,double>& commands) const override;

    private:
        // The interpolators need to be behind pointers because interpolation is non-const
        std::unique_ptr<ssc::interpolation::SplineVariableStep> Cl;
        std::unique_ptr<ssc::interpolation::SplineVariableStep> Cd;
        const double S;
        const Eigen::Vector3d calculation_point;
        bool symmetry;
};

#endif /* FORCE_MODELS_INC_AEROPOLARFORCEMODEL_HPP_ */
