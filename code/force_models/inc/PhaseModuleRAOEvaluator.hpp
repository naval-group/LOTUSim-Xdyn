#ifndef _PHASEMODULERAOEVALUATORHPP_
#define _PHASEMODULERAOEVALUATORHPP_

#include <ssc/kinematics.hpp>
#include "DiffractionInterpolator.hpp"

struct YamlDiffraction;
class BodyStates;
class EnvironmentAndFrames;
class HydroDBParser;
class PhaseModuleRAOEvaluator
{
    public:

        PhaseModuleRAOEvaluator(
            const YamlDiffraction& data,
            const EnvironmentAndFrames& env,
            const HydroDBParser& parser,
            const std::string& body_name,
            const std::string& force_model_name);
        ssc::kinematics::Vector6d evaluate(const BodyStates& states, const double t, const EnvironmentAndFrames& env);
        double get_interpolation_period(const double wave_angular_frequency, const Eigen::Vector2d& Vs, const Eigen::Vector2d& k) const;
        ssc::kinematics::Vector6d express_aquaplus_wrench_in_xdyn_coordinates(ssc::kinematics::Vector6d v) const;
        Eigen::Vector3d get_application_point() const;

    private:
        PhaseModuleRAOEvaluator();
        Eigen::Vector3d H0;
        DiffractionInterpolator rao_interpolator;
        bool use_encounter_period;
};

#endif