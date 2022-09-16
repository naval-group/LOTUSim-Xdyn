#ifndef _PHASEMODULERAOEVALUATORHPP_
#define _PHASEMODULERAOEVALUATORHPP_

#include "xdyn/hdb_interpolators/RaoInterpolator.hpp"
#include <ssc/kinematics.hpp>
#include <string>
#include <vector>

struct BodyStates;
struct EnvironmentAndFrames;
struct FlatDiscreteDirectionalWaveSpectrum;

class PhaseModuleRAOEvaluator
{
    public:

        PhaseModuleRAOEvaluator(
            const RaoInterpolator& rao_interpolator,
            const EnvironmentAndFrames& env,
            const std::string& body_name,
            const std::string& force_model_name);
        ssc::kinematics::Vector6d evaluate(const BodyStates& states, const double t, const EnvironmentAndFrames& env);
        double get_interpolation_period(const double wave_angular_frequency, const Eigen::Vector2d& Vs, const Eigen::Vector2d& k) const;
        Eigen::Vector3d get_application_point() const;

    private:
        PhaseModuleRAOEvaluator();
        RaoInterpolator rao_interpolator;
        Eigen::Vector3d H0;
        bool use_encounter_period;
        void check_spectra_periods_are_in_rao_period_bounds(const std::vector<FlatDiscreteDirectionalWaveSpectrum>& spectra) const;
};

#endif /* _PHASEMODULERAOEVALUATORHPP_ */
