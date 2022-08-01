#ifndef _RAOINTERPOLATORHPP_
#define _RAOINTERPOLATORHPP_

#include <cstdlib> // For size_t
#include <vector>
#include <Eigen/Dense>
#include <ssc/interpolation.hpp>
#include "YamlRAO.hpp"

typedef ssc::interpolation::TwoDimensionalInterpolationVariableStep Interpolator;
class HydroDBParser;


class RaoInterpolator
{
    public:
        RaoInterpolator(
            const HydroDBParser& data, //<! Data read from the HDB or Precal_R file
            const std::vector<double>& omega, //<! Angular frequencies in the wave spectrum (points at which to interpolate the HDB data)
            const std::vector<double>& psi, //<! Wave directions (points at which to interpolate the HDB data)
            const YamlRAO& diffraction_yaml //<! Contents of the force model's parsed YAML data
            );
        RaoInterpolator(
            const HydroDBParser& data, //<! Data read from the HDB or Precal_R file
            const YamlRAO& diffraction_yaml //<! Contents of the force model's parsed YAML data
            );
        virtual ~RaoInterpolator();

        /**
         * \brief Interpolates the RAO's module for a given axis, a wave period (*not* an encounter period)
         * and an incidence (between 0 and π for waves propagating towards port side, π and 2π for waves propagating towards starboard
         * 0 for waves propagating from aft to fore (heading waves), pi for waves propagating from fore to aft (facing waves))
         * \returns module of the rao for the axis, the frequency and the incidence
         */
        double interpolate_module(const size_t axis, const double Tp, double beta);

        /**
         * \brief Interpolates the RAO's phase for a given axis, a wave period (*not* an encounter period)
         * and an incidence (between 0 and π for waves propagating towards port side, π and 2π for waves propagating towards starboard
         * 0 for waves propagating from aft to fore (heading waves), pi for waves propagating from fore to aft (facing waves))
         * \returns module of the rao for the axis, the frequency and the incidence
         */
        double interpolate_phase(const size_t axis, const double Tp, double beta);

        std::vector<double> get_module_periods() const;
        Eigen::Vector3d get_rao_calculation_point() const;
        bool using_encounter_period() const;

    private:
        RaoInterpolator();
        struct RAO
        {
            RAO() = delete;
            static RAO get(const YamlRAO::TypeOfRao& type_of_rao, const HydroDBParser& parser);
            const std::array<std::vector<std::vector<double> >,6 > module_tables;
            const std::array<std::vector<std::vector<double> >,6 > phase_tables;
            const std::vector<double> module_periods;
            const std::vector<double> phase_periods;
            const std::vector<double> module_incidence;
            const std::vector<double> phase_incidence;

            private:
                static RAO for_diffraction(const HydroDBParser& parser);
                static RAO for_froude_krylov(const HydroDBParser& parser);
                RAO(const std::array<std::vector<std::vector<double> >,6 > module_tables_,
                    const std::array<std::vector<std::vector<double> >,6 > phase_tables_,
                    const std::vector<double> module_periods_,
                    const std::vector<double> phase_periods_,
                    const std::vector<double> module_incidence_,
                    const std::vector<double> phase_incidence_);
        };

    public:
        std::array<Interpolator,6> module; //!< 2D spline interpolation function (omega,psi) for RAO's module
        std::array<Interpolator,6> phase; //!< 2D spline interpolation function (omega,psi) for RAO's phase
        bool mirror;
        std::vector<double> omegas;
        std::vector<double> psis;
        std::pair<double, double> period_bounds;
        RAO rao;
        Eigen::Vector3d rao_calculation_point;
        bool use_encounter_period;
};

#endif