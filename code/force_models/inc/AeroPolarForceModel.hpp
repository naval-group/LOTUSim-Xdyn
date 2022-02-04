#ifndef FORCE_MODELS_INC_AEROPOLARFORCEMODEL_HPP_
#define FORCE_MODELS_INC_AEROPOLARFORCEMODEL_HPP_

#include <memory>
#include <ssc/interpolation.hpp>
#include <Eigen/Dense>
#include <boost/optional.hpp>

#include "ForceModel.hpp"

class AeroPolarForceModel : public ForceModel
{
    public:
        struct Input
        {
            Input() = default;
            std::string name;
            YamlCoordinates calculation_point_in_body_frame;
            std::vector<double> apparent_wind_angle;
            std::vector<double> lift_coefficient;
            std::vector<double> drag_coefficient;
            double reference_area;
            boost::optional<std::string> angle_command;
        };

        AeroPolarForceModel(const Input input, const std::string body_name, const EnvironmentAndFrames& env);
        AeroPolarForceModel() = delete;
        virtual ~AeroPolarForceModel() = default;

        static std::string model_name();
        static Input parse(const std::string& yaml_input);
        Wrench get_force(const BodyStates& states, const double t, const EnvironmentAndFrames& env, const std::map<std::string,double>& commands) const override;

    private:
        // The interpolators need to be behind pointers because interpolation is non-const
        std::unique_ptr<ssc::interpolation::SplineVariableStep> Cl; //<! Lift coefficient as a function of the apparent wind angle AWA
        std::unique_ptr<ssc::interpolation::SplineVariableStep> Cd; //!< Drag coefficient as a function of the apparent wind angle AWA
        const double reference_area; //!< Reference area (in square metres) of the wing, for lift and drag normalization
        const Eigen::Vector3d calculation_point;
        bool symmetry; //!< If true, then lift and drag coefficients from 180° to 360° AWA are the same as the coefficients from 180° to 0° (they are symmetric with respect to the wing's x0 axis in the (x0,y0) plane). Otherwise, the coefficients are assumed to have been given for AWA from 0° to 360°.
        boost::optional<std::string> angle_command; //!< If specified, used as an offset to the angle of attack (equivalent to a rotation of the local frame).
};

#endif /* FORCE_MODELS_INC_AEROPOLARFORCEMODEL_HPP_ */
