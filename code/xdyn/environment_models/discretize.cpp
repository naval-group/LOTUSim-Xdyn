/*
 * discretize.cpp
 *
 *  Created on: Aug 1, 2014
 *      Author: cady
 */
#include "discretize.hpp"
#include "WaveDirectionalSpreading.hpp"
#include "WaveSpectralDensity.hpp"
#include "Stretching.hpp"
#include "xdyn/exceptions/InternalErrorException.hpp"
#include "xdyn/exceptions/InvalidInputException.hpp"

#define _USE_MATH_DEFINE
#define PI M_PI
#include <list>
#include <algorithm>
#include <numeric>
#define EPS 1e-8
#include TR1INC(memory)
typedef std::pair<double, size_t> ValIdx;
bool comparator(const ValIdx& l, const ValIdx& r);
bool comparator(const ValIdx& l, const ValIdx& r) { return l.first > r.first; }

DiscreteDirectionalWaveSpectrum common(
        const WaveSpectralDensity& S,      //!< Frequency spectrum
        const WaveDirectionalSpreading& D, //!< Spatial spectrum
        const double omega_min,            //!< Lower bound of the angular frequency range (in rad/s)
        const double omega_max,            //!< Upper bound of the angular frequency range (in rad/s)
        const size_t nfreq,                //!< Number of frequencies in discrete spectrum
        const size_t ndir,                 //!< Number of directions in discrete spectrum
        const bool equal_energy_bins,      //!< Choose omegas so that integral of S between two successive omegas is constant
        const double energy_fraction,      //!< Between 0 and 1: sum(rays taken into account)/sum(rays total)
        const bool periodic,               //!< Space periodic waves or not
        const int resolution,              //!< Number of discretization points in the renderer
        const std::vector<double> sizes    //!< Different repetition sizes in meters in the renderer (largers first)
        );
DiscreteDirectionalWaveSpectrum common(
        const WaveSpectralDensity& S,      //!< Frequency spectrum
        const WaveDirectionalSpreading& D, //!< Spatial spectrum
        const double omega_min,            //!< Lower bound of the angular frequency range (in rad/s)
        const double omega_max,            //!< Upper bound of the angular frequency range (in rad/s)
        const size_t nfreq,                //!< Number of frequencies in discrete spectrum
        const size_t ndir,                 //!< Number of directions in discrete spectrum
        const bool equal_energy_bins,      //!< Choose omegas so that integral of S between two successive omegas is constant
        const double energy_fraction,      //!< Between 0 and 1: sum(rays taken into account)/sum(rays total)
        const bool periodic,               //!< Space periodic waves or not
        const int resolution,              //!< Number of discretization points in the renderer
        const std::vector<double> sizes    //!< Different repetition sizes in meters in the renderer (largers first)
        )
{
    DiscreteDirectionalWaveSpectrum ret;
    if (ndir == 0)
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "ndir == 0");
    }
    ret.psi = D.get_directions(ndir, periodic);
    std::vector<double> omega = S.get_angular_frequencies(omega_min, omega_max, nfreq, equal_energy_bins, periodic, sizes);
    if (periodic)
    {
        std::vector<size_t> indices(omega.size());
        std::iota(begin(indices), end(indices), static_cast<size_t>(0));
        std::sort(begin(indices), end(indices), [&omega](size_t a, size_t b) { return omega[a] < omega[b]; } );
        double last_omega = 0.f;
        std::vector<double> temp_omega;
        for (size_t i = 0; i < omega.size(); i++)
        {
            double this_omega = omega[indices[i]];
            if (std::abs(this_omega - last_omega) > EPS)
            {
                last_omega = this_omega;
                temp_omega.push_back(this_omega);
            }
        }
        omega = temp_omega;
    }
    ret.omega = omega;
    ret.Si.reserve(ret.omega.size());
    for (const auto omega:ret.omega) ret.Si.push_back(S(omega));
    ret.Dj.reserve(ret.psi.size());
    for (const auto psi:ret.psi) ret.Dj.push_back(D(psi));
    ret.S = TR1(shared_ptr)<WaveSpectralDensity>(S.clone());
    ret.D = TR1(shared_ptr)<WaveDirectionalSpreading>(D.clone());
    ret.resolution = resolution;
    ret.sizes = sizes;
    ret.energy_fraction = energy_fraction;
    return ret;
}

DiscreteDirectionalWaveSpectrum discretize(
        const WaveSpectralDensity& S,      //!< Frequency spectrum
        const WaveDirectionalSpreading& D, //!< Spatial spectrum
        const double omega_min,            //!< Lower bound of the angular frequency range (in rad/s)
        const double omega_max,            //!< Upper bound of the angular frequency range (in rad/s)
        const size_t nfreq,                //!< Number of frequencies in discrete spectrum
        const size_t ndir,                 //!< Number of directions in discrete spectrum
        const Stretching& stretching,      //!< Dilate z-axis to properly compute orbital velocities (delta-stretching)
        const bool equal_energy_bins,      //!< Choose omegas so the integral of S between two successive omegas is constant
        const double energy_fraction,      //!< Between 0 and 1: sum(rays taken into account)/sum(rays total)
        const bool periodic,               //!< Space periodic waves or not
        const int resolution,              //!< Number of discretization points in the renderer
        const std::vector<double> sizes    //!< Different repetition sizes in meters in the renderer (largers first)
)
{
    DiscreteDirectionalWaveSpectrum ret = common(S, D, omega_min, omega_max, nfreq, ndir, equal_energy_bins, energy_fraction, periodic, resolution, sizes);
    ret.k.reserve(ret.omega.size());
    for (const auto omega:ret.omega) ret.k.push_back(S.get_wave_number(omega));
    ret.pdyn_factor = [stretching](const double k, const double z, const double eta){return dynamic_pressure_factor(k,z,eta,stretching);};
    ret.pdyn_factor_sh = [stretching](const double k, const double z, const double eta){return dynamic_pressure_factor(k,z,eta,stretching);};
    return ret;
}

/**  \author cady
  *  \date Aug 1, 2014, 5:04:24 PM
  *  \brief Discretize a wave spectrum
  *  \details Finite depth hypothesis (for the wave number)
  *  \snippet environment_models/unit_tests/discretizeTest.cpp discretizeTest discretize_example
  */
DiscreteDirectionalWaveSpectrum discretize(
        const WaveSpectralDensity& S,      //!< Frequency spectrum
        const WaveDirectionalSpreading& D, //!< Spatial spectrum
        const double omega_min,            //!< Lower bound of the angular frequency range (in rad/s)
        const double omega_max,            //!< Upper bound of the angular frequency range (in rad/s)
        const size_t nfreq,                //!< Number of frequencies in discrete spectrum
        const size_t ndir,                 //!< Number of directions in discrete spectrum
        const double h,                    //!< Water depth (in meters)
        const Stretching& stretching,      //!< Dilate z-axis to properly compute orbital velocities (delta-stretching)
        const bool equal_energy_bins,      //!< Choose omegas so the integral of S between two successive omegas is constant
        const double energy_fraction,      //!< Between 0 and 1: sum(rays taken into account)/sum(rays total)
        const bool periodic,               //!< Space periodic waves or not
        const int resolution,              //!< Number of discretization points in the renderer
        const std::vector<double> sizes    //!< Different repetition sizes in meters in the renderer (largers first)
)
{
    DiscreteDirectionalWaveSpectrum ret = common(S, D, omega_min, omega_max, nfreq, ndir, equal_energy_bins, energy_fraction, periodic, resolution, sizes);
    ret.k.reserve(ret.omega.size());
    for (const auto omega:ret.omega) ret.k.push_back(S.get_wave_number(omega,h));
    for (size_t i = 0 ; i < ret.k.size() ; ++i)
    {
        const double k = ret.k.at(i);
        if (k*h<PI/10.)
        {
            THROW(__PRETTY_FUNCTION__, InvalidInputException, "You should be using a shallow water model (currently none exist in X-DYN) because the water depth (h = " << h << " m) is lower than the wave length divided by twenty (lambda/20 = " << 2*PI/k << ") for omega = " << ret.omega.at(i));
        }
    }
    ret.pdyn_factor = [h,stretching](const double k, const double z, const double eta){return dynamic_pressure_factor(k,z,h,eta,stretching);};
    ret.pdyn_factor_sh = [h,stretching](const double k, const double z, const double eta){return dynamic_pressure_factor_sh(k,z,h,eta,stretching);};
    return ret;
}

/**
 * \param spectrum
 * \return flattened spectrum
 * \note We keep all components for the time being
 * If we filter and discard some rays, we should also take into account rao...
 */
FlatDiscreteDirectionalWaveSpectrum flatten(
    const DiscreteDirectionalWaveSpectrum& spectrum //!< Spectrum to flatten
)
{
    FlatDiscreteDirectionalWaveSpectrum ret;
    ret.pdyn_factor = spectrum.pdyn_factor;
    ret.pdyn_factor_sh = spectrum.pdyn_factor_sh;
    const size_t nOmega = spectrum.omega.size();
    const size_t nPsi = spectrum.psi.size();
    if (nOmega * nPsi > 0)
    {
        ret.phase.reserve(nOmega * nPsi);
    }
    double dpsi;
    double Si;
    double domega;
    double mult_factor;
    for (size_t i = 0; i < nOmega; ++i)
    {
        if (nOmega > 1)
        {
            if (i == 0)
            {
                domega = (spectrum.omega.at(1) - spectrum.omega.at(0)) / 2;
            }
            else if (i == nOmega - 1)
            {
                domega = (spectrum.omega.at(nOmega - 1) - spectrum.omega.at(nOmega - 2)) / 2;
            }
            else
            {
                domega = (spectrum.omega.at(i) - spectrum.omega.at(i - 1)) / 2
                        + (spectrum.omega.at(i + 1) - spectrum.omega.at(i)) / 2;
            }
        }
        else
        {
            domega = 1;
        }
        for (size_t j = 0 ; j < nPsi ; ++j)
        {
            bool is_in = false;
            if (nPsi > 1)
            {
                if (j == 0)
                {
                    dpsi = (spectrum.psi.at(1)-spectrum.psi.at(0))/2;
                }
                else if (j == nPsi - 1)
                {
                    dpsi = (spectrum.psi.at(nPsi-1)-spectrum.psi.at(nPsi-2))/2;
                }
                else
                {
                    dpsi = (spectrum.psi.at(j)-spectrum.psi.at(j-1))/2 + (spectrum.psi.at(j+1)-spectrum.psi.at(j))/2;
                }
            }
            else
            {
                dpsi = 1;
            }
            if (spectrum.periodic)
            {
                // These band_max variables represents the maximum possible wavenumber according to Nyquist–Shannon sampling theorem
                // https://en.wikipedia.org/wiki/Nyquist%E2%80%93Shannon_sampling_theorem
                // Here we ned to change the frequencies for the different directions psi to keep the wave periodic
                std::vector<std::pair<int,int>> coprimes = spectrum.D->build_coprimes(nPsi); 
                mult_factor = sqrt(pow(coprimes.at(j).first,2)+pow(coprimes.at(j).second,2));
                Si = spectrum.S->operator()(spectrum.omega[i]*sqrt(mult_factor));
                // The following lines allocate the frequencies to the right bands
                double max_k = spectrum.k[i] * std::max(std::abs(cos(spectrum.psi[j])), std::abs(sin(spectrum.psi[j])));
                if (max_k <= PI * spectrum.resolution / spectrum.sizes.at(0))
                {
                    ret.band.push_back(0);
                    is_in = true;
                }
                else if (max_k <= PI * spectrum.resolution / spectrum.sizes.at(1))
                {
                    if (std::abs(fmod(spectrum.sizes.at(1),2*PI/max_k)) < EPS)
                    {
                        ret.band.push_back(1);
                        is_in = true;
                    }
                }
                else if (max_k <= PI * spectrum.resolution / spectrum.sizes.at(2))
                {
                    if (std::abs(fmod(spectrum.sizes.at(2),2*PI/max_k)) < EPS)
                    {
                        ret.band.push_back(2);
                        is_in = true;
                    }
                }
            }
            else
            {
                Si = spectrum.Si[i];
                mult_factor = 1;
                is_in = true;
            }
            if (is_in)
            {
                ret.k.push_back(spectrum.k[i]*mult_factor);
                ret.omega.push_back(spectrum.omega[i]*sqrt(mult_factor));
                ret.a.push_back(sqrt(2 * Si * spectrum.Dj[j] * domega * sqrt(mult_factor) * dpsi));
                ret.psi.push_back(spectrum.psi[j]);
                ret.cos_psi.push_back(cos(spectrum.psi[j]));
                ret.sin_psi.push_back(sin(spectrum.psi[j]));
                ret.phase.push_back(spectrum.phase.at(i).at(j));
            }
        }
    }
    if (spectrum.energy_fraction < 1) {return filter(ret,spectrum.energy_fraction);}
    else {return ret;}
}

/**
 * \brief Only select the most important spectrum components & create single vector.
 * \details Output spectrum represents at least `ratio * Energy`
 *  No need to loop on all frequencies & all directions: we only select
 *  the most important ones (i.e. those representing a given ratio of the total
 *  energy in the spectrum).
 * \returns A flat spectrum (i.e. one where the freq & direct. loops have been unrolled)
 * \snippet environment_models/unit_tests/discretizeTest.cpp discretizeTest flatten_example
 */
FlatDiscreteDirectionalWaveSpectrum filter(
        const FlatDiscreteDirectionalWaveSpectrum& spectrum, //!< Spectrum to filter
        const double ratio //!< Between 0 & 1: where should we cut off the spectra?
        )
{
    double S = 0;
    double a = 0;
    const size_t n = spectrum.omega.size();
    std::list<ValIdx> SiDj;
    FlatDiscreteDirectionalWaveSpectrum ret;
    ret.pdyn_factor=spectrum.pdyn_factor;
    ret.pdyn_factor_sh=spectrum.pdyn_factor_sh;
    for (size_t i = 0 ; i < n; ++i)
    {
        a = spectrum.a.at(i);
        S += a * a;
        SiDj.push_back(std::make_pair(a * a, i));
    }
    SiDj.sort(comparator);
    double cumsum = 0;
    const double max_energy = ratio * S;
    for (size_t i = 0 ; i < n ; ++i)
    {
        if (cumsum >= max_energy) return ret;
        const ValIdx sidj = SiDj.front();
        SiDj.pop_front();
        cumsum += sidj.first;
        ret.a.push_back(spectrum.a.at(sidj.second));
        ret.omega.push_back(spectrum.omega.at(sidj.second));
        ret.psi.push_back(spectrum.psi.at(sidj.second));
        ret.cos_psi.push_back(spectrum.cos_psi.at(sidj.second));
        ret.sin_psi.push_back(spectrum.sin_psi.at(sidj.second));
        ret.k.push_back(spectrum.k.at(sidj.second));
        ret.phase.push_back(spectrum.phase.at(sidj.second));   
        if (!spectrum.band.empty()) {ret.band.push_back(spectrum.band.at(sidj.second));}    
    }
    return ret;
}

double dynamic_pressure_factor(const double k,              //!< Wave number (in 1/m)
                               const double z,              //!< z-position in the NED frame (in meters)
                               const double eta,            //!< Wave elevation at (x,y) in the NED frame (in meters) for stretching
                               const Stretching& stretching //!< Dilate z-axis to properly compute orbital velocities (delta-stretching)
                              )
{
    if (std::isnan(z))
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "z (value to rescale, in meters) was NaN");
    }
    if (std::isnan(eta))
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "eta (wave height, in meters) was NaN");
    }
    if (eta != 0 && z<eta) return 0;
    return exp(-k*stretching.rescaled_z(z,eta));
}

double dynamic_pressure_factor(const double k,              //!< Wave number (in 1/m)
                               const double z,              //!< z-position in the NED frame (in meters)
                               const double h,              //!< Average water depth (in meters)
                               const double eta,            //!< Wave elevation at (x,y) in the NED frame (in meters) for stretching
                               const Stretching& stretching //!< Dilate z-axis to properly compute orbital velocities (delta-stretching)
                              )
{
    if (std::isnan(z))
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "z (value to rescale, in meters) was NaN");
    }
    if (std::isnan(eta))
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "eta (wave height, in meters) was NaN");
    }
    if (eta != 0 && z<eta) return 0;
    if (z>h) return 0;
    return cosh(k*(h-stretching.rescaled_z(z,eta)))/cosh(k*h);
}

std::vector<double> area_curve(const std::vector<double>& xs, const std::vector<double>& ys)
{
    const size_t n = xs.size();
    if (ys.size() != n)
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "xs and ys should have the same number of points.");
    }
    std::vector<double> ret(n, 0);
    for (size_t i = 0 ; i < n ; ++i)
    {
        if (ys[i] < 0)
        {
            THROW(__PRETTY_FUNCTION__, InvalidInputException, "All values in ys should be positive.");
        }
        if (i>0)
        {
            if (xs[i-1] >= xs[i])
            {
                THROW(__PRETTY_FUNCTION__, InvalidInputException, "xs should be strictly increasing.");
            }
            ret[i] = ret[i-1] + 0.5*(ys[i] + ys[i-1])*(xs[i] - xs[i-1]);
        }
    }
    return ret;
}

double find_integration_bound_yielding_target_area_on_a_segment(const double target_area, const double xa, const double xb, const double ya, const double yb);
double find_integration_bound_yielding_target_area_on_a_segment(const double target_area, const double xa, const double xb, const double ya, const double yb)
{
    const double alpha = (yb - ya)/(xb - xa);
    const double beta = ya - xa*alpha;
    if (alpha == 0)
    {
        return xa + target_area/beta;
    }
    const double delta = beta*beta + alpha * (alpha*xa*xa + 2*beta*xa + 2*target_area);
    return (std::sqrt(delta)-beta)/alpha;
}

double find_integration_bound_yielding_target_area(const double target_area, const std::vector<double>& xs, const std::vector<double>& ys, const std::vector<double>& as)
{
    for (size_t i = 1 ; i < xs.size() ; ++i)
    {
        if ((target_area>=as.at(i-1)) && (target_area<=as.at(i)))
        {
            return find_integration_bound_yielding_target_area_on_a_segment(target_area-as.at(i-1), xs[i-1], xs[i], ys[i-1], ys[i]);
        }
    }
    return as.back();
}

std::vector<double> equal_area_abscissae(const std::vector<double>& xs, //!< Input abscissae at which the function is defined
        const std::vector<double>& ys //!< Value of the function for each xs
        )
{
    const size_t n = xs.size();
    if (ys.size() != n)
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "xs and ys should have the same number of points.");
    }
    std::vector<double> ret = xs;
    if (n > 0)
    {
        const auto as = area_curve(xs, ys);
        const double target_area = as.back()/((double)n-1);
        for (size_t i = 1 ; i < n-1 ; ++i)
        {
            ret[i] = find_integration_bound_yielding_target_area((double)i*target_area, xs, ys, as);
        }
    }
    return ret;
}

double dynamic_pressure_factor_sh(const double k,              //!< Wave number (in 1/m)
                                  const double z,              //!< z-position in the NED frame (in meters)
                                  const double h,              //!< Average water depth (in meters)
                                  const double eta,            //!< Wave elevation at (x,y) in the NED frame (in meters) for stretching
                                  const Stretching& stretching //!< Dilate z-axis to properly compute orbital velocities (delta-stretching)
                              )
{
    if (eta != 0 && z<eta) return 0;
    if (z>h) return 0;
    return sinh(k*(h-stretching.rescaled_z(z,eta)))/cosh(k*h);
}
