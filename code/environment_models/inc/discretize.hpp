/*
 * discretize.hpp
 *
 *  Created on: Aug 1, 2014
 *      Author: cady
 */

#ifndef DISCRETIZE_HPP_
#define DISCRETIZE_HPP_

#include "DiscreteDirectionalWaveSpectrum.hpp"
#include "YamlRadiationDamping.hpp"

class Stretching;
class WaveSpectralDensity;
class WaveDirectionalSpreading;

FlatDiscreteDirectionalWaveSpectrum flatten(const DiscreteDirectionalWaveSpectrum& spectrum
                                            );

/**  \brief Only select the most important spectrum components & create single vector
  *  \details No need to loop on all frequencies & all directions: we only select
  *  the most important ones (i.e. those representing a given ratio of the total
  *  energy in the spectrum.
  *  \returns A flat spectrum (i.e. one where the freq & direct. loops have been unrolled)
  *  \snippet environment_models/unit_tests/src/discretizeTest.cpp discretizeTest flatten_example
  */
FlatDiscreteDirectionalWaveSpectrum filter(
        const FlatDiscreteDirectionalWaveSpectrum& spectrum, //!< Spectrum to filter
        const double energy_ratio = 1.0//!< Between 0 & 1: where should we cut off the spectra?
        );

/**  \author cec
  *  \date Aug 1, 2014, 5:04:24 PM
  *  \brief Discretize a wave spectrum
  *  \details Infinite depth hypothesis (for the wave number)
  *  \snippet environment_models/unit_tests/src/discretizeTest.cpp discretizeTest discretize_example
  */
DiscreteDirectionalWaveSpectrum discretize(const WaveSpectralDensity& S,      //!< Frequency spectrum
                                           const WaveDirectionalSpreading& D, //!< Spatial spectrum
                                           const double omega_min,            //!< Upper bound of the angular frequency range (in rad/s)
                                           const double omega_max,            //!< Upper bound of the angular frequency range (in rad/s)
                                           const size_t nfreq,                //!< Number of frequencies & number of directions in discrete spectrum
                                           const Stretching& stretching       //!< Dilate z-axis to properly compute orbital velocities (delta-stretching)
                                           );

/**  \author cec
  *  \date Aug 1, 2014, 5:04:24 PM
  *  \brief Discretize a wave spectrum
  *  \details Finite depth hypothesis (for the wave number)
  *  \snippet environment_models/unit_tests/src/discretizeTest.cpp discretizeTest discretize_example
  */
DiscreteDirectionalWaveSpectrum discretize(const WaveSpectralDensity& S,      //!< Frequency spectrum
                                           const WaveDirectionalSpreading& D, //!< Spatial spectrum
                                           const double omega_min,            //!< Upper bound of the angular frequency range (in rad/s)
                                           const double omega_max,            //!< Upper bound of the angular frequency range (in rad/s)
                                           const size_t nfreq,                //!< Number of frequencies & number of directions in discrete spectrum
                                           const double h,                    //!< Water depth (in meters)
                                           const Stretching& stretching       //!< Dilate z-axis to properly compute orbital velocities (delta-stretching)
                                           );

/**  \author cady
 *  \date Nov 23, 2020
 *  \brief Calculates consecutive intervals where a function has constant area.
* \param `xs` Vector of strictly increasing abscissae
* \param `ys` Values of the function at each `xs`.
* \returns `x` a vector of abscissae between xmin and xmax (x[0] == xs[0] and
* x[n-1] == xs[n-1] where n denotes the number of values in `xs` and `ys`).
* \details If `f` denotes a function such that ys[i] = f(xs[i]), this function
* returns a list of increasing `x`-values such that integrating f between x[i-1]
* and x[i] will give a constant value, for each i between 1 and n-1.
* \snippet /unit_tests/src/discretizeTest.cpp discretizeTest equal_area_abscissae_example
*/
std::vector<double> equal_area_abscissae(
        const std::vector<double>& xs, //!< Input abscissae at which the function is defined
        const std::vector<double>& ys, //!< Value of the function for each xs
        const TypeOfQuadrature& type_of_quadrature //!< Algorithm to use for quadrature (numerical integration)
         );

/**  \brief Utility function used by the discretize function. Infinite depth approximation. This is where the stretching is taken into account.
  *  \returns Factor \f$f(k,z)\f$ such that \f$p_{\mbox{dyn}}=\rho g \eta_a f(k,z)\f$ (no unit), infinite depth approximation
  *  \snippet environment_models/unit_tests/src/discretizeTest.cpp discretizeTest dynamic_pressure_factor example
  */
double dynamic_pressure_factor(const double k,              //!< Wave number (in 1/m)
                               const double z,              //!< z-position in the NED frame (in meters)
                               const double eta,            //!< Wave elevation at (x,y) in the NED frame (in meters)
                               const Stretching& stretching //!< Dilate z-axis to properly compute orbital velocities (delta-stretching)
                              );

/**  \brief Utility function used by the discretize function. Finite depth. This is where the stretching is taken into account.
  *  \returns Factor \f$f(k,z,h)\f$ such that \f$p_{\mbox{dyn}}=\rho g \eta_a f(k,z,h)\f$ (no unit), finite depth.
  *  \snippet environment_models/unit_tests/src/discretizeTest.cpp discretizeTest dynamic_pressure_factor example
  */
double dynamic_pressure_factor(const double k,              //!< Wave number (in 1/m)
                               const double z,              //!< z-position in the NED frame (in meters)
                               const double h,              //!< Average water depth (in meters)
                               const double eta,            //!< Wave elevation at (x,y) in the NED frame (in meters)
                               const Stretching& stretching //!< Dilate z-axis to properly compute orbital velocities (delta-stretching)
                              );

/**  \brief Utility function used by the discretize function. Finite depth.
  *  \returns Factor \f$f(k,z,h)\f$ such that \f$p_{\mbox{dyn}}=\rho g \eta_a f(k,z,h)\f$ (no unit), finite depth.
  *  \snippet environment_models/unit_tests/src/discretizeTest.cpp discretizeTest dynamic_pressure_factor example
  */
double dynamic_pressure_factor_sh(const double k,              //!< Wave number (in 1/m)
                                  const double z,              //!< z-position in the NED frame (in meters)
                                  const double h,              //!< Average water depth (in meters)
                                  const double eta,            //!< Wave elevation at (x,y) in the NED frame (in meters)
                                  const Stretching& stretching //!< Dilate z-axis to properly compute orbital velocities (delta-stretching)
                                 );

#endif /* DISCRETIZE_HPP_ */
