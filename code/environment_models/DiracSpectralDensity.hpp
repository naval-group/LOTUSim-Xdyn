/*
 * DiracSpectralDensity.hpp
 *
 *  Created on: Jul 31, 2014
 *      Author: cady
 */

#ifndef DIRACSPECTRALDENSITY_HPP_
#define DIRACSPECTRALDENSITY_HPP_

#include "WaveSpectralDensity.hpp"

/** \author cady
 *  \date Jul 31, 2014, 2:30:28 PM
 *  \brief Only one harmonic (cosine wave)
 *  \details
 *  \ingroup wave_models
 *  \section ex1 Example
 *  \snippet environment_models/unit_tests/DiracSpectralDensityTest.cpp DiracSpectralDensityTest example
 *  \section ex2 Expected output
 *  \snippet environment_models/unit_tests/DiracSpectralDensityTest.cpp DiracSpectralDensityTest expected output
 */
class DiracSpectralDensity : public WaveSpectralDensity
{
    public:
        DiracSpectralDensity(const double omega0, //!< Angular frequency (\f$2\pi f\f$) in rad/s of the significant wave height
                             const double Hs      //!< Significant wave height (in meters)
                             );

        /**  \author cady
          *  \date Jul 31, 2014, 2:30:28 PM
          *  \brief Computes the amplitude of the power spectrum at a given angular frequency
          *  \returns Amplitude of the power spectrum (in m^2 s)
          *  \snippet environment_models/unit_tests/DiracSpectralDensityTest.cpp DiracSpectralDensityTest get_example
          */
        double operator()(const double omega //!< Angular frequency (\f$2\pi f\f$) in rad/s of the significant wave height
                          ) const;

        /**  \author cady
          *  \date Jul 31, 2014, 2:41:16 PM
          *  \brief A vector containing only omega0 (in rad/s)
          *  \snippet environment_models/unit_tests/DiracSpectralDensityTest.cpp DiracSpectralDensityTest get_omega0_example
          */
        std::vector<double> get_angular_frequencies(const double omega_min, const double omega_max, const size_t n, const bool constant_energy_bins) const;

        WaveSpectralDensity* clone() const;

    private:
        double omega0;
        double Hs;
};

#endif /* DIRACSPECTRALDENSITY_HPP_ */
