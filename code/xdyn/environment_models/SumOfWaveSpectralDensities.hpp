/*
 * SumOfWaveSpectralDensities.hpp
 *
 *  Created on: Sep 3, 2014
 *      Author: cady
 */

#ifndef SUMOFWAVESPECTRALDENSITIES_HPP_
#define SUMOFWAVESPECTRALDENSITIES_HPP_

#include <ssc/macros.hpp>
#include TR1INC(memory)

#include "xdyn/environment_models/WaveSpectralDensity.hpp"

typedef TR1(shared_ptr)<WaveSpectralDensity> WaveSpectralDensityPtr;

/** \brief Class returned when summing two WaveSpectralDensity objects
 *  \details
 *  \addtogroup waves
 *  \ingroup waves
 *  \section ex1 Example
 *  \snippet waves/unit_tests/SumOfWaveSpectralDensitiesTest.cpp SumOfWaveSpectralDensitiesTest example
 *  \section ex2 Expected output
 *  \snippet waves/unit_tests/SumOfWaveSpectralDensitiesTest.cpp SumOfWaveSpectralDensitiesTest expected output
 */
class SumOfWaveSpectralDensities : public WaveSpectralDensity
{
    public:
        SumOfWaveSpectralDensities();
        SumOfWaveSpectralDensities(const WaveSpectralDensity& w);
        SumOfWaveSpectralDensities(const WaveSpectralDensity& w1, const WaveSpectralDensity& w2);
        SumOfWaveSpectralDensities(const std::vector<WaveSpectralDensity>& ws);
        double operator()(const double omega //!< Angular frequency (\f$2\pi f\f$) in rad/s of the significant wave height
                                  ) const;

        WaveSpectralDensity* clone() const;

        /**  \brief Returns n angular frequencies between omega_min (included)
          *         and omega_max (also included)
          *  \snippet environment_models/unit_tests/WaveSpectralDensityTest.cpp WaveSpectralDensityTest get_omega0_example
          */
        std::vector<double> get_angular_frequencies(const double omega_min, //!< Minimum angular frequency (in rad/s)
                                                    const double omega_max, //!< Maximum angular frequency (in rad/s)
                                                    const size_t n,         //!< Number of angular frequencies to return
                                                    const bool equal_energy_bins,       //!< Choose omegas so the integral of S between two successive omegas is constant
                                                    const bool periodic = false,        //!< Space periodic waves or not
                                                    const std::vector<double> sizes = {}//!< Different repetition sizes in meters in the renderer (largers first)
                                                    ) const;

    private:
        std::vector<WaveSpectralDensityPtr> terms;
};

#endif /* SUMOFWAVESPECTRALDENSITIES_HPP_ */
