/*
 * builders.cpp
 *
 *  Created on: Aug 12, 2014
 *      Author: cady
 */

#include "builders.hpp"
#include "xdyn/core/DefaultSurfaceElevation.hpp"
#include "xdyn/core/EnvironmentAndFrames.hpp"
#include "xdyn/environment_models/discretize.hpp"
#include "xdyn/environment_models/Stretching.hpp"
#include "xdyn/exceptions/InvalidInputException.hpp"
#include "xdyn/external_data_structures/YamlGRPC.hpp"
#include "xdyn/grpc/SurfaceElevationFromGRPC.hpp"
#include "xdyn/yaml_parser/environment_parsers.hpp"
#include <algorithm>


boost::optional<SurfaceElevationInterfacePtr> SurfaceElevationBuilder<DefaultSurfaceElevation>::try_to_parse(const std::string& model, const std::string& yaml) const
{
    boost::optional<SurfaceElevationInterfacePtr> ret;
    if (model == "no waves")
    {
        if (yaml.empty())
        {
            THROW(__PRETTY_FUNCTION__, InvalidInputException, "No yaml data detected for default wave model (expected 'constant wave height in NED frame: {value: xx, unit: yy})'");
        }
        YamlDefaultWaveModel input = parse_default_wave_model(yaml);
        const auto output_mesh = make_wave_mesh(input.output);
        SurfaceElevationInterfacePtr p(new DefaultSurfaceElevation(input.zwave, output_mesh));
        ret.reset(p);
    }
    return ret;
}

boost::optional<WaveModelPtr> WaveModelBuilder<Airy>::try_to_parse(const std::string& model, const DiscreteDirectionalWaveSpectrum& spectrum, const std::string& yaml) const
{
    boost::optional<WaveModelPtr> ret;
    if (model == "airy")
    {
        const boost::optional<int> seed = parse_seed_of_random_number_generator(yaml);
        if (seed)
        {
            ret.reset(WaveModelPtr(new Airy(spectrum,*seed)));
        }
        else
        {
            ret.reset(WaveModelPtr(new Airy(spectrum,0.0)));
        }
    }
    return ret;
}

boost::optional<WaveModelPtr> WaveModelBuilder<Airy>::try_to_parse(const std::string& model, const FlatDiscreteDirectionalWaveSpectrum& spectrum, const std::string&) const
{
    boost::optional<WaveModelPtr> ret;
    if (model == "airy")
    {
        ret.reset(WaveModelPtr(new Airy(spectrum)));
    }
    return ret;
}

WaveSpectralDensityPtr SurfaceElevationBuilder<SurfaceElevationFromWaves>::parse_spectral_density(const YamlSpectrum& spectrum) const
{
    for (auto that_parser = spectrum_parsers->begin() ; that_parser != spectrum_parsers->end() ; ++that_parser)
    {
        boost::optional<WaveSpectralDensityPtr> w;
        try
        {
            w = (*that_parser)->try_to_parse(spectrum.spectral_density_type, spectrum.spectral_density_yaml);
        }
        catch (const InvalidInputException& exception)
        {
            THROW(__PRETTY_FUNCTION__, InvalidInputException, "Problem when parsing spectral density '" << spectrum.model << "': " << exception.get_message());
        }
        if (w) return w.get();
    }
    THROW(__PRETTY_FUNCTION__, InvalidInputException, "The wave spectral density specified in the YAML file is not understood by the simulator ('" << spectrum.spectral_density_type << "'): either it is misspelt or this simulator version is outdated.");
    return WaveSpectralDensityPtr();
}

WaveDirectionalSpreadingPtr SurfaceElevationBuilder<SurfaceElevationFromWaves>::parse_directional_spreading(const YamlSpectrum& spectrum) const
{
    for (auto that_parser = directional_spreading_parsers->begin() ; that_parser != directional_spreading_parsers->end() ; ++that_parser)
    {
        boost::optional<WaveDirectionalSpreadingPtr> w;
        try
        {
            w = (*that_parser)->try_to_parse(spectrum.directional_spreading_type, spectrum.directional_spreading_yaml);
        }
        catch (const InvalidInputException& exception)
        {
            THROW(__PRETTY_FUNCTION__, InvalidInputException, "Problem when parsing directional spreading '" << spectrum.model << "': " << exception.get_message());
        }
        if (w) return w.get();
    }
    THROW(__PRETTY_FUNCTION__, InvalidInputException, "The wave directional spreading specified in the YAML file is not understood by the simulator ('" << spectrum.directional_spreading_type << "'): either it is misspelt or this simulator version is outdated.");
    return WaveDirectionalSpreadingPtr ();
}

DiscreteDirectionalWaveSpectrum SurfaceElevationBuilder<SurfaceElevationFromWaves>::parse_directional_spectrum(const YamlDiscretization& discretization, const YamlSpectrum& spectrum) const
{
    WaveSpectralDensityPtr spectral_density = parse_spectral_density(spectrum);
    WaveDirectionalSpreadingPtr directional_spreading = parse_directional_spreading(spectrum);
    if (spectrum.depth>0)
    {
        return discretize(*spectral_density, *directional_spreading, discretization.omega_min, discretization.omega_max, discretization.nfreq, discretization.ndir, Stretching(spectrum.stretching), discretization.equal_energy_bins);
    }
    return discretize(*spectral_density, *directional_spreading, discretization.omega_min, discretization.omega_max, discretization.nfreq, discretization.ndir, spectrum.depth, Stretching(spectrum.stretching), discretization.equal_energy_bins);
}

WaveModelPtr SurfaceElevationBuilder<SurfaceElevationFromWaves>::parse_wave_model(const YamlDiscretization& discretization, const YamlSpectrum& spectrum) const
{
    const DiscreteDirectionalWaveSpectrum discrete_spectrum = parse_directional_spectrum(discretization, spectrum);
    for (auto that_parser = wave_parsers->begin() ; that_parser != wave_parsers->end() ; ++that_parser)
    {
        boost::optional<WaveModelPtr> w = (*that_parser)->try_to_parse(spectrum.model, discrete_spectrum, spectrum.model_yaml);
        if (w) return w.get();
    }
    THROW(__PRETTY_FUNCTION__,
          InvalidInputException,
          "The wave model specified in the YAML file is not understood by the simulator ('" << spectrum.model << "'): either it is misspelt or this simulator version is outdated.");
    return WaveModelPtr();
}

FlatDiscreteDirectionalWaveSpectrum SurfaceElevationBuilder<SurfaceElevationFromWaves>::parse_flat_spectrum(const YamlSpectrumFromRays& spectrum) const
{
    FlatDiscreteDirectionalWaveSpectrum f;
    std::copy(spectrum.rays.a.begin(), spectrum.rays.a.end(), std::back_inserter(f.a));
    std::copy(spectrum.rays.psi.begin(), spectrum.rays.psi.end(), std::back_inserter(f.psi));
    std::copy(spectrum.rays.omega.begin(), spectrum.rays.omega.end(), std::back_inserter(f.omega));
    std::copy(spectrum.rays.k.begin(), spectrum.rays.k.end(), std::back_inserter(f.k));
    std::copy(spectrum.rays.phase.begin(), spectrum.rays.phase.end(), std::back_inserter(f.phase));
    for (const auto& psi: spectrum.rays.psi)
    {
        f.cos_psi.push_back(cos(psi));
        f.sin_psi.push_back(sin(psi));
    }
    const Stretching stretching(spectrum.stretching);
    if (spectrum.depth==0.0)
    {
        // Infinite depth
        f.pdyn_factor = [stretching](const double k, const double z, const double eta){return dynamic_pressure_factor(k,z,eta,stretching);};
        f.pdyn_factor_sh = [stretching](const double k, const double z, const double eta){return dynamic_pressure_factor(k,z,eta,stretching);};
    }
    else
    {
        // Finite depth
        const double h = spectrum.depth;
        f.pdyn_factor = [h,stretching](const double k, const double z, const double eta){return dynamic_pressure_factor(k,z,h,eta,stretching);};
        f.pdyn_factor_sh = [h,stretching](const double k, const double z, const double eta){return dynamic_pressure_factor_sh(k,z,h,eta,stretching);};
    }
    return f;
}

WaveModelPtr SurfaceElevationBuilder<SurfaceElevationFromWaves>::parse_wave_model(const YamlSpectrumFromRays& spectrum) const
{
    const FlatDiscreteDirectionalWaveSpectrum flat_spectrum = parse_flat_spectrum(spectrum);
    for (auto that_parser = wave_parsers->begin() ; that_parser != wave_parsers->end() ; ++that_parser)
    {
        boost::optional<WaveModelPtr> w = (*that_parser)->try_to_parse(spectrum.model, flat_spectrum, spectrum.model_yaml);
        if (w) return w.get();
    }
    THROW(__PRETTY_FUNCTION__,
          InvalidInputException,
          "The wave model specified in the YAML file is not understood by the simulator ('" << spectrum.model << "'): either it is misspelt or this simulator version is outdated.");
    return WaveModelPtr();
}

boost::optional<SurfaceElevationInterfacePtr> SurfaceElevationBuilder<SurfaceElevationFromWaves>::try_to_parse(const std::string& model, const std::string& yaml) const
{
    boost::optional<SurfaceElevationInterfacePtr> ret;
    if (model == "waves") // The "model" key is always "waves" for xdyn's internal wave models, except for the default wave model "no waves"
    {
        const YamlWaveModel input = parse_waves(yaml);
        const auto output_mesh = make_wave_mesh(input.output);
        std::vector<WaveModelPtr> models;
        for (const auto& spectrum: input.spectra) models.push_back(parse_wave_model(input.discretization, spectrum));
        for (const auto& spectrum: input.spectra_from_rays) models.push_back(parse_wave_model(spectrum));
        ret.reset(SurfaceElevationInterfacePtr(new SurfaceElevationFromWaves(models,get_wave_mesh_size(input.output), output_mesh)));
    }
    return ret;
}

boost::optional<WaveSpectralDensityPtr> SpectrumBuilder<BretschneiderSpectrum>::try_to_parse(const std::string& model, const std::string& yaml) const
{
    boost::optional<WaveSpectralDensityPtr> ret;
    if (model == "bretschneider")
    {
        const YamlBretschneider data = parse_bretschneider(yaml);
        ret.reset(WaveSpectralDensityPtr(new BretschneiderSpectrum(data.Hs, data.Tp)));
    }
    return ret;
}

boost::optional<WaveSpectralDensityPtr> SpectrumBuilder<JonswapSpectrum>::try_to_parse(const std::string& model, const std::string& yaml) const
{
    boost::optional<WaveSpectralDensityPtr> ret;
    if (model == "jonswap")
    {
        const YamlJonswap data = parse_jonswap(yaml);
        ret.reset(WaveSpectralDensityPtr(new JonswapSpectrum(data.Hs, data.Tp, data.gamma)));
    }
    return ret;
}

boost::optional<WaveSpectralDensityPtr> SpectrumBuilder<PiersonMoskowitzSpectrum>::try_to_parse(const std::string& model, const std::string& yaml) const
{
    boost::optional<WaveSpectralDensityPtr> ret;
    if (model == "pierson-moskowitz")
    {
        const YamlPiersonMoskowitz data = parse_pierson_moskowitz(yaml);
        ret.reset(WaveSpectralDensityPtr(new PiersonMoskowitzSpectrum(data.Hs, data.Tp)));
    }
    return ret;
}

boost::optional<WaveSpectralDensityPtr> SpectrumBuilder<DiracSpectralDensity>::try_to_parse(const std::string& model, const std::string& yaml) const
{
    boost::optional<WaveSpectralDensityPtr> ret;
    if (model == "dirac")
    {
        const YamlDiracSpectrum data = parse_wave_dirac_spectrum(yaml);
        ret.reset(WaveSpectralDensityPtr(new DiracSpectralDensity(data.omega0, data.Hs)));
    }
    return ret;
}

boost::optional<WaveDirectionalSpreadingPtr> DirectionalSpreadingBuilder<DiracDirectionalSpreading>::try_to_parse(const std::string& model, const std::string& yaml) const
{
    boost::optional<WaveDirectionalSpreadingPtr> ret;
    if (model == "dirac")
    {
        const YamlDiracDirection data = parse_wave_dirac_direction(yaml);
        ret.reset(WaveDirectionalSpreadingPtr (new DiracDirectionalSpreading(data.psi0)));
    }
    return ret;
}

boost::optional<WaveDirectionalSpreadingPtr> DirectionalSpreadingBuilder<Cos2sDirectionalSpreading>::try_to_parse(const std::string& model, const std::string& yaml) const
{
    boost::optional<WaveDirectionalSpreadingPtr> ret;
    if (model == "cos2s")
    {
        const YamlCos2s data = parse_cos2s(yaml);
        ret.reset(WaveDirectionalSpreadingPtr (new Cos2sDirectionalSpreading(data.psi0, data.s)));
    }
    return ret;
}

boost::optional<SurfaceElevationInterfacePtr> SurfaceElevationBuilder<SurfaceElevationFromGRPC>::try_to_parse(const std::string& model, const std::string& yaml) const
{
    boost::optional<SurfaceElevationInterfacePtr> ret;
    if (model == "grpc")
    {
        const YamlGRPC input = parse_grpc(yaml);
        const auto output_mesh = make_wave_mesh(input.output);
        ret.reset(SurfaceElevationInterfacePtr(new SurfaceElevationFromGRPC(input, output_mesh)));
    }
    return ret;
}
