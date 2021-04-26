#ifndef FORCE_MODELS_INC_HYDROPOLARFORCEMODEL_HPP_
#define FORCE_MODELS_INC_HYDROPOLARFORCEMODEL_HPP_

#include <memory>
#include <ssc/interpolation.hpp>
#include <Eigen/Dense>
#include <boost/optional.hpp>

#include "ForceModel.hpp"

class HydroPolarForceModel : public ForceModel
{
    public:
        struct Input
        {
            std::string name;
            YamlPosition internal_frame;
            std::vector<double> angle_of_attack;
            std::vector<double> lift_coefficient;
            std::vector<double> drag_coefficient;
            boost::optional<std::vector<double>> moment_coefficient;
            double reference_area;
            boost::optional<double> chord_length;
            bool use_waves_velocity;
        };

        HydroPolarForceModel(const Input input, const std::string body_name, const EnvironmentAndFrames& env);
        HydroPolarForceModel() = delete;
        virtual ~HydroPolarForceModel() = default;

        static std::string model_name();
        static Input parse(const std::string& yaml_input);
        Wrench get_force(const BodyStates& states, const double t, const EnvironmentAndFrames& env, const std::map<std::string,double>& commands) const override;

    private:
        // The interpolators need to be behind pointers because interpolation is non-const
        std::unique_ptr<ssc::interpolation::SplineVariableStep> Cl; //<! Lift coefficient as a function of the apparent flow angle beta
        std::unique_ptr<ssc::interpolation::SplineVariableStep> Cd; //!< Drag coefficient as a function of the apparent flow angle beta
        std::unique_ptr<ssc::interpolation::SplineVariableStep> Cm; //!< Moment coefficient as a function of the apparent flow angle beta (optional)
        const double reference_area; //!< Reference area (in square metres) of the wing, for lift and drag normalization
        boost::optional<double> chord_length; //!< Chord length (in m), used (optionally) for moment normalization
        bool symmetry; //!< If true, then lift and drag coefficients from -180° to 0° angle of attack are the same as the coefficients from 0° to 180° (they are symmetric with respect to the foil's x0 axis in the (x0,y0) plane). Otherwise, the coefficients are assumed to have been given for angle of attack from -180° to 180°.
        bool use_waves_velocity; //!< If true, the waves orbital velocity is added to the flow velocity acting on the foil
};

#endif /* FORCE_MODELS_INC_HYDROPOLARFORCEMODEL_HPP_ */
