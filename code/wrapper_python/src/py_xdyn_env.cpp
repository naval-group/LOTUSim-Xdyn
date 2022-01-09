#include "py_xdyn_env.hpp"
#include "py_pybind_additions.hpp"
#include "environment_models/inc/discretize.hpp"
#include "environment_models/inc/Airy.hpp"
#include "environment_models/inc/BretschneiderSpectrum.hpp"
#include "environment_models/inc/Cos2sDirectionalSpreading.hpp"
#include "environment_models/inc/DefaultWindModel.hpp"
#include "environment_models/inc/DiracDirectionalSpreading.hpp"
#include "environment_models/inc/DiracSpectralDensity.hpp"
#include "environment_models/inc/DiscreteDirectionalWaveSpectrum.hpp"
#include "environment_models/inc/JonswapSpectrum.hpp"
#include "environment_models/inc/LogWindVelocityProfile.hpp"
#include "environment_models/inc/PiersonMoskowitzSpectrum.hpp"
#include "environment_models/inc/PowerLawWindVelocityProfile.hpp"
#include "environment_models/inc/Stretching.hpp"
#include "environment_models/inc/SumOfWaveDirectionalSpreadings.hpp"
#include "environment_models/inc/SumOfWaveSpectralDensities.hpp"
#include "environment_models/inc/UniformWindVelocityProfile.hpp"
#include "environment_models/inc/WaveDirectionalSpreading.hpp"
#include "environment_models/inc/WaveModel.hpp"
#include "environment_models/inc/WaveNumberFunctor.hpp"
#include "environment_models/inc/WaveSpectralDensity.hpp"
#include "environment_models/inc/WindModel.hpp"
#include "environment_models/inc/WindMeanVelocityProfile.hpp"
#include "external_data_structures/inc/YamlModel.hpp"
#include "external_data_structures/inc/YamlWaveModelInput.hpp"
#include <vector>

namespace py = pybind11;

void py_add_module_xdyn_env_wave_io(py::module& m);
void py_add_module_xdyn_env_wave_io(py::module& m)
{
    py::class_<YamlWaveOutput>(m, "YamlWaveOutput")
        .def(py::init<>())
        .def_readwrite("frame_of_reference",&YamlWaveOutput::frame_of_reference, "Name of the frame of reference the x & y coordinates are expressed in")
        .def_readwrite("xmin", &YamlWaveOutput::xmin, "Minimum x value (in meters) of the points in the output mesh")
        .def_readwrite("xmax", &YamlWaveOutput::xmax, "Maximum x value (in meters) of the points in the output mesh")
        .def_readwrite("nx"  , &YamlWaveOutput::nx, "Discretization of the mesh along the x axis (number of different x values)")
        .def_readwrite("ymin", &YamlWaveOutput::ymin, "Minimum y value (in meters) of the points in the output mesh")
        .def_readwrite("ymax", &YamlWaveOutput::ymax, "Maximum y value (in meters) of the points in the output mesh")
        .def_readwrite("ny" , &YamlWaveOutput::ny, "Discretization of the mesh along the y axis (number of different x values)")
        ;

    py::class_<YamlDiscretization>(m, "YamlDiscretization")
        .def(py::init<>())
        .def_readwrite("nfreq", &YamlDiscretization::nfreq, "Nb of frequencies in the discretization")
        .def_readwrite("ndir", &YamlDiscretization::ndir, "Nb of directions in the discretization")
        .def_readwrite("omega_min", &YamlDiscretization::omega_min, "First angular frequency (in rad/s)")
        .def_readwrite("omega_max", &YamlDiscretization::omega_max, "Last angular frequency (in rad/s)")
        .def_readwrite("energy_fraction", &YamlDiscretization::energy_fraction, "Between 0 and 1: sum(S(omega[i]).S(psi[j]),taken into account)/sum(S(omega[i]).S(psi[j]),total)")
        .def_readwrite("equal_energy_bins", &YamlDiscretization::equal_energy_bins, "False (by default) or true. When false, omegas are sampled regularly between omega_min and omega_max. When true, they are sampled such that the integral of the spectrum is constant between two consecutive omega values.")
        ;

    py::class_<YamlStretching>(m, "YamlStretching")
        .def(py::init<>())
        .def_readwrite("delta", &YamlStretching::delta, "0 for Wheeler stretching, 1 for linear extrapolation")
        .def_readwrite("h", &YamlStretching::h, "Depth (in meters) over which the stretching is taken into account. Should usually be equal to ""depth"" (or 0 for no stretching)")
        ;

    py::class_<YamlSpectra>(m, "YamlSpectra")
        .def(py::init<>())
        .def_readwrite("model", &YamlSpectra::model, "Wave model (eg. airy)")
        .def_readwrite("model_yaml", &YamlSpectra::model_yaml, "Model data in YAML format")
        .def_readwrite("directional_spreading_type", &YamlSpectra::directional_spreading_type, "Spreading type (eg. cos2s)")
        .def_readwrite("directional_spreading_yaml", &YamlSpectra::directional_spreading_yaml, "Model data in YAML format")
        .def_readwrite("spectral_density_type", &YamlSpectra::spectral_density_type, "Type of spectrum (eg. jonswap)")
        .def_readwrite("spectral_density_yaml", &YamlSpectra::spectral_density_yaml, "Spectral model parameters in YAML format")
        .def_readwrite("depth", &YamlSpectra::depth, "Water depth (in meters): 0 for infinite depth")
        .def_readwrite("stretching", &YamlSpectra::stretching, "Stretching model for orbital wave velocities (delta-stretching model)")
        ;

    py::class_<YamlWaveModel>(m, "YamlWaveModel")
        .def(py::init<>())
        .def_readwrite("discretization", &YamlWaveModel::discretization, "Spectral discretization parameters")
        .def_readwrite("spectra", &YamlWaveModel::spectra, "Wave spectra to generate")
        .def_readwrite("output", &YamlWaveModel::output, "Defines what wave data is outputted during the simulation & how it is generated")
        ;

    py::class_<YamlJonswap>(m, "YamlJonswap")
        .def(py::init<>())
        .def_readwrite("Hs", &YamlJonswap::Hs, "Significant wave height (in meters)")
        .def_readwrite("Tp", &YamlJonswap::Tp, "Mean wave period (in seconds)")
        .def_readwrite("gamma", &YamlJonswap::gamma, "Non-dimensional peak shape parameter")
        ;

    py::class_<YamlBretschneider>(m, "YamlBretschneider")
        .def(py::init<>())
        .def_readwrite("Hs", &YamlBretschneider::Hs, "Significant wave height (in meters)")
        .def_readwrite("Tp", &YamlBretschneider::Tp, "Mean wave period (in seconds)")
        ;

    py::class_<YamlCos2s>(m, "YamlCos2s")
        .def(py::init<>())
        .def_readwrite("psi0", &YamlCos2s::psi0, "Primary wave direction in radians.")
        .def_readwrite("s", &YamlCos2s::s, "Exponent")
        ;

    py::class_<YamlDiracDirection>(m, "YamlDiracDirection")
        .def(py::init<>())
        .def_readwrite("psi0", &YamlDiracDirection::psi0,"Primary wave direction in radians.")
        ;

    py::class_<YamlDiracSpectrum>(m, "YamlDiracSpectrum")
        .def(py::init<>())
        .def_readwrite("omega0", &YamlDiracSpectrum::omega0, "Angular frequency (2 * pi * f) in rad/s of the significant wave height")
        .def_readwrite("Hs", &YamlDiracSpectrum::Hs, "Significant wave height (in meters)")
        ;

    py::class_<YamlPiersonMoskowitz>(m, "YamlPiersonMoskowitz")
        .def(py::init<>())
        .def_readwrite("Hs", &YamlPiersonMoskowitz::Hs, "Significant wave height (in meters)")
        .def_readwrite("Tp", &YamlPiersonMoskowitz::Tp, "Mean wave period (in seconds)")
        ;

    py::class_<YamlDefaultWaveModel>(m, "YamlDefaultWaveModel")
        .def(py::init<>())
        .def_readwrite("zwave", &YamlDefaultWaveModel::zwave, "Constant free surface elevation (in meters)")
        .def_readwrite("output", &YamlDefaultWaveModel::output, "Defines what wave data is outputted during the simulation & how it is generated")
        ;
}

void py_add_module_xdyn_env_wave(py::module& m_env);
void py_add_module_xdyn_env_wave(py::module& m_env)
{
    py::module m_env_io = m_env.def_submodule("io");
    py_add_module_xdyn_env_wave_io(m_env_io);

    py::class_<WaveNumberFunctor>(m_env, "WaveNumberFunctor")
        .def(py::init<const double, const double>(),
            py::arg("h"),
            py::arg("omega"),
            R"(This is the functor used to solve the dispersion relation to compute
               the wave number.

               Provides the values of the function & its first & second derivatives.
               The function is \f$k\mapsto g\cdot k\cdot \tanh{kh} - \omega^2$\f

               Input:

               - `h` (float): Water depth (in meters)
               - `omega` (float): Angular frequency (in rad/s)
            )")
        .def("__call__", &WaveNumberFunctor::operator())
        ;

    py::class_<DiscreteDirectionalWaveSpectrum>(m_env, "DiscreteDirectionalWaveSpectrum")
        .def(py::init<>())
        .def_readwrite("Si", &DiscreteDirectionalWaveSpectrum::Si, "Discretized spectral density (in s m^2/rad)")
        .def_readwrite("Dj", &DiscreteDirectionalWaveSpectrum::Dj, "Spatial spreading (in 1/rad)")
        .def_readwrite("omega", &DiscreteDirectionalWaveSpectrum::omega, "Angular frequencies the spectrum was discretized at (in rad/s)")
        .def_readwrite("psi", &DiscreteDirectionalWaveSpectrum::psi, "Directions between 0 & 2pi the spatial spreading was discretized at (in rad)")
        .def_readwrite("k", &DiscreteDirectionalWaveSpectrum::k, "Discretized wave number (for each frequency) in rad/m")
        .def_readwrite("phase", &DiscreteDirectionalWaveSpectrum::phase, "Random phases, for each (frequency, direction) couple (but time invariant) in radian phases *phase[i_freq][i_dir]*")
        ;

    py::class_<FlatDiscreteDirectionalWaveSpectrum>(m_env, "FlatDiscreteDirectionalWaveSpectrum")
        .def(py::init<>())
        .def_readwrite("a", &FlatDiscreteDirectionalWaveSpectrum::a,
            "Amplitude (in m².s), for each angular frequency omega, and direction")
        .def_readwrite("omega", &FlatDiscreteDirectionalWaveSpectrum::omega,
            "Angular frequencies the spectrum was discretized at (in rad/s), for each angular frequency omega, and direction")
        .def_readwrite("psi", &FlatDiscreteDirectionalWaveSpectrum::psi,
            "Directions between 0 & 2pi the spatial spreading was discretized at (in rad)")
        .def_readwrite("cos_psi", &FlatDiscreteDirectionalWaveSpectrum::cos_psi,
            "Cosinus directions between 0 & 2pi the spatial spreading was discretized at (so we do not compute it each time), for each angular frequency omega, and direction")
        .def_readwrite("sin_psi", &FlatDiscreteDirectionalWaveSpectrum::sin_psi,
            "Sinus directions between 0 & 2pi the spatial spreading was discretized at (so we do not compute it each time), for each angular frequency omega, and direction")
        .def_readwrite("k", &FlatDiscreteDirectionalWaveSpectrum::k,
            "Discretized wave number (for each frequency) in rad/m, for each angular frequency omega, i.e. same size as omega")
        .def_readwrite("phase", &FlatDiscreteDirectionalWaveSpectrum::phase,
            "Random phases, for each (frequency, direction) couple (but time invariant) in radian, for each angular frequency omega, and direction")
        ;

    py::class_<WaveModel>(m_env, "WaveModel")
        // .def(py::init<const DiscreteDirectionalWaveSpectrum& /*spectrum*/, const double /*constant_random_phase*/>())
        // .def(py::init<const DiscreteDirectionalWaveSpectrum& /*spectrum*/, const int /*random_number_generator_seed*/>())
        .def("get_elevation", &WaveModel::get_elevation,
            py::arg("x"),
            py::arg("y"),
            py::arg("t"),
            R"(
            Computes the surface elevations at given points.
            Elevations of a list of points at a given instant, in meters.

            Input:

            - `x`: x-positions in the NED frame (in meters),
            - `y`: y-positions in the NED frame (in meters),
            - `t`: Current time instant (in seconds),
            )")
        .def("get_orbital_velocity", &WaveModel::get_orbital_velocity,
            py::arg("g"),
            py::arg("x"),
            py::arg("y"),
            py::arg("z"),
            py::arg("t"),
            py::arg("eta"),
            R"(
            Computes the orbital velocity at given points.
            Velocities of the fluid at given points & instant, in m/s

            Input:

            - `g`: gravity (in m/s^2),
            - `x`: x-positions in the NED frame (in meters),
            - `y`: y-positions in the NED frame (in meters),
            - `z`: z-positions in the NED frame (in meters),
            - `t`: Current time instant (in seconds),
            - `eta`: Wave heights at `x`, `y`, `t` (in meters),
            )")
        .def("get_dynamic_pressure", &WaveModel::get_dynamic_pressure,
            py::arg("rho"),
            py::arg("g"),
            py::arg("x"),
            py::arg("y"),
            py::arg("z"),
            py::arg("eta"),
            py::arg("t"),
            R"(
            Computes the dynamic pressure at a given point.
            Pressure (in Pa) induced by the waves, at given points in the fluid

            Input:

            - `rho` (float): water density (in $kg.m^3$),
            - `g` (float): gravity (in $`m/s^2`$),
            - `x` (List[float]): x-positions in the NED frame (in meters),
            - `y` (List[float]): y-positions in the NED frame (in meters),
            - `z` (List[float]): z-positions in the NED frame (in meters),
            - `eta` (List[float]): Wave heights at x,y,t (in meters),
            - `t` (float): Current time instant (in seconds),
            )")
        .def("get_omegas", &WaveModel::get_omegas,
            R"(
            List of angular frequencies for which the spectra will be calculated.
            Needed by the RAOs (RadiationForceModel)
            )")
        .def("get_psis", &WaveModel::get_psis,
            R"(
            List of incidences for which the spectra will be calculated.
            Needed by the RAOs (RadiationForceModel)
            )")
        .def("get_flat_spectrum", &WaveModel::get_flat_spectrum)
        .def("get_spectrum", &WaveModel::get_spectrum)
        ;

    py::class_<WaveModelPtr>(m_env, "WaveModelPtr");

    py::class_<Airy, WaveModel>(m_env, "Airy")
        .def(py::init<const DiscreteDirectionalWaveSpectrum& /*spectrum*/, const double /*constant_random_phase*/>(),
            py::arg("spectrum"), py::arg("constant_random_phase"))
        .def(py::init<const DiscreteDirectionalWaveSpectrum& /*spectrum*/, const int /*random_number_generator_seed*/>(),
            py::arg("spectrum"), py::arg("random_number_generator_seed"))
        .def("evaluate_rao", &Airy::evaluate_rao,
            py::arg("x"),
            py::arg("y"),
            py::arg("t"),
            py::arg("rao_module"),
            py::arg("rao_phase"),
            R"(
            Calculate radiation forces using first order force RAO.
            Force (or torque), depending on the RAO.

            Input:

            - `x` (float): x-position of the RAO's calculation point in the NED frame (in meters)
            - `y` (float): y-position of the RAO's calculation point in the NED frame (in meters)
            - `t` (float): Current time instant (in seconds)
            - `rao_module` (List[float]): Module of the RAO
            - `rao_phase` (List[float]): Phase of the RAO
            )")
        ;
    py::class_<Stretching>(m_env, "Stretching", "Rescale the z-axis with delta stretching model (used to compute orbital velocities")
        .def(py::init<const YamlStretching& /*input*/>(), "Usually read from YAML")
        .def(py::init([](double delta, double h) {
            YamlStretching ys;
            ys.delta = delta;
            ys.h = h;
            return std::unique_ptr<Stretching>(new Stretching(ys));
            }),
            py::arg("delta") = 0.0,
            py::arg("h") = 0.0
            )
        .def("rescaled_z", &Stretching::rescaled_z,
            py::arg("original_z"),
            py::arg("wave_height"),
            R"(

            Input:

            - `original_z` (float): z value we wish to rescale (in meters)
            - `wave_height` (float): Wave height (in meters), z being oriented downwards
            )")
        //.def("__repr__",
        //   [](const Stretching &a) {
        //       std::stringstream ss;
        //       ss << "{\"delta\":" << a.delta
        //          << ",\"h\":" << a.delta << "}";
        //        return ss.str();
        //    })
        ;

    py::class_<WaveSpectralDensity>(m_env, "WaveSpectralDensity")
        .def("get_wave_number",
            static_cast<double (WaveSpectralDensity::*)(const double /*omega*/) const>(&WaveSpectralDensity::get_wave_number),
            py::arg("omega"),
            "Compute wave number, infinite depth")
        .def("get_wave_number",
            static_cast<double (WaveSpectralDensity::*)(const double /*omega*/, const double /*h*/) const>(&WaveSpectralDensity::get_wave_number),
            py::arg("omega"),
            py::arg("h"),
            "Compute wave number, in finite depth")
        ;

    py::class_<SumOfWaveSpectralDensities, WaveSpectralDensity>(m_env, "SumOfWaveSpectralDensities")
        .def(py::init<>())
        .def(py::init<const WaveSpectralDensity& /*s*/>(), py::arg("s"))
        .def(py::init<const WaveSpectralDensity& /*s1*/, const SumOfWaveSpectralDensities& /*s2*/>(), py::arg("s1"), py::arg("s2"))
        // .def(py::init<const std::vector<WaveSpectralDensity>& /*ws*/>(), py::arg("ws"))
        .def("__call__", &SumOfWaveSpectralDensities::operator(), py::arg("omega"),
            R"(
            Wave density by wave number.

            - `omega` (float): Angular frequency (omega = 2 * pi * f) in rad/s
            )")
        .def("get_angular_frequencies", &SumOfWaveSpectralDensities::get_angular_frequencies,
            py::arg("omega_min"),
            py::arg("omega_max"),
            py::arg("n"),
            py::arg("equal_energy_bins"),
            R"(
            Returns `n` angular frequencies between omega_min (included) and omega_max (also included)

            Input:

            - `omega_min` (float): Minimum angular frequency (in rad/s)
            - `omega_max` (float): Maximum angular frequency (in rad/s)
            - `n` (int): Number of angular frequencies to return
            - `equal_energy_bins` (bool): Choose omegas so the integral of S between two successive omegas is constant
            )")
        ;

    py::class_<DiracSpectralDensity, WaveSpectralDensity>(m_env, "DiracSpectralDensity")
        .def(py::init<const double& /*omega0*/, const double /*Hs*/>(), py::arg("omega0"), py::arg("Hs"))
        .def(py::self + py::self)
        .def("__call__", &DiracSpectralDensity::operator(), py::arg("omega"), "Computes the amplitude of the power spectrum at a given angular frequency")
        .def("get_angular_frequencies", &DiracSpectralDensity::get_angular_frequencies,  "A vector containing only omega0 (in rad/s)")
        ;

    py::class_<BretschneiderSpectrum, WaveSpectralDensity>(m_env, "BretschneiderSpectrum")
        .def(py::init<const double& /*Hs*/, const double /*Tp*/>(), py::arg("Hs"), py::arg("Tp"),
            R"(
            BretschneiderSpectrum only constructor

            Input:

            - `Hs` (float): Significant wave height (in meters)
            - `Tp` (float): Mean wave period (in seconds)
            )")
        .def(py::self + py::self)
        .def("__call__", &BretschneiderSpectrum::operator(), py::arg("omega"),
            R"(
            Computes the amplitude of the power spectrum at a given angular frequency

            Returns Amplitude of the power spectrum (in m^2 s)

            Input:

            - `omega` (float): Angular frequency (2\pi f) in rad/s of the significant wave height
            )")
        ;

    py::class_<JonswapSpectrum, WaveSpectralDensity>(m_env, "JonswapSpectrum")
        .def(py::init<const double& /*Hs*/, const double /*Tp*/, const double /*gamma*/>(),
            py::arg("Hs"), py::arg("Tp"), py::arg("omega"),
            R"(

            Input:

            - `Hs` (float): Significant wave height (in meters)
            - `Tp` (float): Mean wave period (in seconds)
            - `gamma` (float): Non-dimensional peak shape parameter
            )")
        .def(py::self + py::self)
        .def("__call__", &JonswapSpectrum::operator(), py::arg("omega"),
            R"(
            Computes the amplitude of the power spectrum at a given angular frequency

            Returns Amplitude of the power spectrum (in $m^2.s$)

            Input:

            - `omega` (float): Angular frequency (2\pi f) in rad/s of the significant wave height
            )");

    py::class_<PiersonMoskowitzSpectrum, JonswapSpectrum, WaveSpectralDensity>(m_env, "PiersonMoskowitzSpectrum")
        .def(py::init<const double& /*Hs*/, const double /*Tp*/>(),
            py::arg("Hs"), py::arg("Tp"),
            R"(
            Input:

            - `Hs` (float): Significant wave height (in meters)
            - `Tp` (float): Mean wave period (in seconds)
            )")
        ;

    py::class_<WaveDirectionalSpreading>(m_env, "WaveDirectionalSpreading")
        // .def(py::init<>())
        // .def(py::init<const double /*psi0*/>(), py::arg("psi"),
        //     "Primary wave direction (NED, ""coming from"") in radians"
        //     )
        ;

    py::class_<SumOfWaveDirectionalSpreadings, WaveDirectionalSpreading>(m_env, "SumOfWaveDirectionalSpreadings")
        .def(py::init<const WaveDirectionalSpreading& /*s*/>(), py::arg("s"))
        .def(py::init<const WaveDirectionalSpreading& /*s1*/, const WaveDirectionalSpreading& /*s2*/>(), py::arg("s1"), py::arg("s2"))
        // .def(py::init<const std::vector<WaveDirectionalSpreading>& /*s*/>(), py::arg("s"))
        .def("__call__", &SumOfWaveDirectionalSpreadings::operator(), py::arg("psi"),
            R"(
            Wave density by direction.

            Input:

            - `psi` (float): Primary wave direction in radians.
            )")
        .def("get_directions", &SumOfWaveDirectionalSpreadings::get_directions, py::arg("n"))
        ;

    py::class_<DiracDirectionalSpreading, WaveDirectionalSpreading>(m_env, "DiracDirectionalSpreading")
        .def(py::init<const double /*psi0*/>(), py::arg("psi0"),
            "Constructor with psi0 Primary wave direction (NED, ""coming from"") in radians")
        .def(py::self + py::self)
        .def("__call__", &DiracDirectionalSpreading::operator(), py::arg("psi"),
        R"(
        Wave density by direction.
        Equals one if psi=psi0 & 0 otherwise.

        Input:

        - `psi` (float): Primary wave direction in radians.
        )")
        .def("get_directions", &DiracDirectionalSpreading::get_directions, "Returns a vector containing only psi0")
        ;

    py::class_<Cos2sDirectionalSpreading, WaveDirectionalSpreading>(m_env, "Cos2sDirectionalSpreading")
        .def(py::init<const double /*psi0*/, const double /*s*/>(), py::arg("psi0"), py::arg("s"))
        .def(py::self + py::self)
        .def("__call__", &Cos2sDirectionalSpreading::operator(), py::arg("psi"),
        R"(
        Wave density by direction.
        Equals one if psi=psi0 & 0 otherwise.

        Input:

        - `psi` (float): Primary wave direction in radians.
        )")
        ;

    m_env.def("discretize",
        [](const WaveSpectralDensity& S,      //!< Frequency spectrum
           const WaveDirectionalSpreading& D, //!< Spatial spectrum
           const double omega_min,            //!< Lower bound of the angular frequency range (in rad/s)
           const double omega_max,            //!< Upper bound of the angular frequency range (in rad/s)
           const size_t nfreq,                //!< Number of frequencies in discrete spectrum
           const size_t ndir,                 //!< Number of directions in discrete spectrum
           const Stretching& stretching,      //!< Dilate z-axis to properly compute orbital velocities (delta-stretching)
           const bool equal_energy_bins,      //!< Choose omegas so the integral of S between two successive omegas is constant
           boost::optional<double> h          //!< Water depth (in meters)
           ){
            if (h.is_initialized()) {
                return discretize(
                    S,                  //!< Frequency spectrum
                    D,                  //!< Spatial spectrum
                    omega_min,          //!< Lower bound of the angular frequency range (in rad/s)
                    omega_max,          //!< Upper bound of the angular frequency range (in rad/s)
                    (size_t)nfreq,      //!< Number of frequencies in discrete spectrum
                    (size_t)ndir,       //!< Number of directions in discrete spectrum
                    h.get(),            //!< Water depth (in meters)
                    stretching,         //!< Dilate z-axis to properly compute orbital velocities (delta-stretching)
                    equal_energy_bins   //!< Choose omegas so the integral of S between two successive omegas is constant
                    );
            } else {
                return discretize(
                    S,                  //!< Frequency spectrum
                    D,                  //!< Spatial spectrum
                    omega_min,          //!< Lower bound of the angular frequency range (in rad/s)
                    omega_max,          //!< Upper bound of the angular frequency range (in rad/s)
                    (size_t)nfreq,      //!< Number of frequencies in discrete spectrum
                    (size_t)ndir,       //!< Number of directions in discrete spectrum
                    stretching,         //!< Dilate z-axis to properly compute orbital velocities (delta-stretching)
                    equal_energy_bins   //!< Choose omegas so the integral of S between two successive omegas is constant
                    );
            }
        },
        py::arg("S"),
        py::arg("D"),
        py::arg("omega_min"),
        py::arg("omega_max"),
        py::arg("nfreq"),
        py::arg("ndir"),
        py::arg("stretching"),
        py::arg("equal_energy_bins") = false,
        py::arg("h") = py::none(),
        R"(
        Discretize a wave spectrum

        Input:

        - `S` (WaveSpectralDensity) : Frequency spectrum
        - `D` (WaveDirectionalSpreading) : Spatial spectrum
        - `omega_min` (float) : Lower bound of the angular frequency range (in rad/s)
        - `omega_max` (float) : Upper bound of the angular frequency range (in rad/s)
        - `nfreq` (int) : Number of frequencies in discrete spectrum
        - `ndir` (int) : Number of directions in discrete spectrum
        - `stretching` (Stretching) : Dilate z-axis to properly compute orbital velocities (delta-stretching)
        - `equal_energy_bins` (Optional[bool]) : Choose omegas so the integral of S between two successive omegas is constant. Default is False
        - `h` (Optional[float]) : Water depth (in meters). If None, or not provided, infinite depth will be considered
        )")
        ;

    m_env.def("dynamic_pressure_factor",
        [](const double k,               //!< Wave number (in 1/m)
           const double z,               //!< z-position in the NED frame (in meters)
           const double eta,             //!< Wave elevation at (x,y) in the NED frame (in meters) for stretching
           const Stretching& stretching, //!< Dilate z-axis to properly compute orbital velocities (delta-stretching)
           boost::optional<double> h     //!< Water depth (in meters)
           ){
            if (h.is_initialized()) {
                return dynamic_pressure_factor(
                    k,         //!< Wave number (in 1/m)
                    z,         //!< z-position in the NED frame (in meters)
                    h.get(),   //!< Average water depth (in meters)
                    eta,       //!< Wave elevation at (x,y) in the NED frame (in meters) for stretching
                    stretching //!< Dilate z-axis to properly compute orbital velocities (delta-stretching)
                    );
            } else {
                return dynamic_pressure_factor(
                    k,         //!< Wave number (in 1/m)
                    z,         //!< z-position in the NED frame (in meters)
                    eta,       //!< Wave elevation at (x,y) in the NED frame (in meters) for stretching
                    stretching //!< Dilate z-axis to properly compute orbital velocities (delta-stretching)
                    );
            }
        },
        py::arg("k"),
        py::arg("z"),
        py::arg("eta"),
        py::arg("stretching"),
        py::arg("h") = py::none(),
        R"(
        Utility function used by the discretize function. This is where the stretching is taken into account.
        Finite and infinite depth are considered: Use None or skip h declaration to consider infinite depth.

        Input:

        - `k` (float): Wave number (in 1/m)
        - `z` (float): z-position in the NED frame (in meters)
        - `eta` (float): Wave elevation at (x,y) in the NED frame (in meters) for stretching
        - `stretching` (Stretching): Dilate z-axis to properly compute orbital velocities (delta-stretching)
        - `h` (Optional[float]): Water depth (in meters). If None, or not provided, infinite depth will be considered
        )")
        ;

    m_env.def("find_integration_bound_yielding_target_area", &find_integration_bound_yielding_target_area,
        py::arg("target_area"),
        py::arg("xs"),
        py::arg("ys"),
        py::arg("as"),
        R"(
        Finds the abscissa at which the area curve has a given value.
        This is not a simple linear interpolation because integrating half
        of an interval does not necessarily give half the area.

        Input:

        - `target_area`
        - `xs` Abscissae at which the area curve is defined.
        - `ys` Positive values of the function at each `xs`.
        - `as` Area curve.

        Return
            Abscissa at which the integral has the given value.
        )")
        ;

    m_env.def("area_curve", &area_curve, py::arg("xs"), py::arg("ys"),
        R"(
        Calculates the cumulative integral using trapezoidal quadrature.

        Input:

        - `xs` Vector of strictly increasing abscissae.
        - `ys` Positive values of the function at each `xs`.

        Return
            For each x, integral of ys from xmin to x.
        )")
        ;

    m_env.def("flatten", &flatten, py::arg("spectrum"),
        R"(
        Discretize a wave spectrum

        Input:

        - `spectrum` (DiscreteDirectionalWaveSpectrum): Spectrum to flatten

        Returns a FlatDiscreteDirectionalWaveSpectrum
        )")
        ;


    m_env.def("filter_spectrum", &filter, py::arg("spectrum"), py::arg("ratio"),
        // Renamed function filter to filter_spectrum to avoid any conflict with python
        // wrapper function
        R"(
        Only select the most important spectrum components & create single vector.

        Output spectrum represents at least `ratio * Energy`
        No need to loop on all frequencies & all directions: we only select
        the most important ones (i.e. those representing a given ratio of the total
        energy in the spectrum).

        - `spectrum` (FlatDiscreteDirectionalWaveSpectrum): Spectrum to filter
        - `ratio` (float): Ratio between 0 & 1: where should we cut off the spectra?

        Returns FlatDiscreteDirectionalWaveSpectrum A flat spectrum
        (i.e. one where the freq & direct. loops have been unrolled)
        )")
        ;

    m_env.def("equal_area_abscissae", &equal_area_abscissae, py::arg("xs"), py::arg("ys"),
        R"(
        Calculates consecutive intervals where a function has constant area.

        Input:

        - `xs` Vector of strictly increasing abscissae
        - `ys` Positive values of the function at each `xs`.

        Returns

        - `x` a vector of abscissae between xmin and xmax (x[0] == xs[0] and
           x[n-1] == xs[n-1] where n denotes the number of values in `xs` and `ys`).

        If `f` denotes a function such that ys[i] = f(xs[i]), this function
        returns a list of increasing `x`-values such that integrating f between x[i-1]
        and x[i] will give a constant value, for each i between 1 and n-1.
        )")
        ;
}

void py_add_module_xdyn_env_wind_io(py::module& m);
void py_add_module_xdyn_env_wind_io(py::module& m)
{
    py::class_<YamlModel>(m, "YamlModel")
        .def(py::init<>())
        .def_readwrite("model", &YamlModel::model)
        .def_readwrite("yaml", &YamlModel::yaml)
        .def_readwrite("index_of_first_line_in_global_yaml", &YamlModel::index_of_first_line_in_global_yaml)
        ;
}

void py_add_module_xdyn_env_wind(py::module& m_env);
void py_add_module_xdyn_env_wind(py::module& m_env)
{
    py::module m_env_io = m_env.def_submodule("io");
    py_add_module_xdyn_env_wind_io(m_env_io);

    py::class_<WindModel>(m_env, "WindModel")
        // .def(py::init<>())
        // .def_static("build_parser", &WindModel::build_parser)
        ;

    py::class_<DefaultWindModel, WindModel>(m_env, "DefaultWindModel")
        .def(py::init<int>(), "Constructor argument is a dummy in order to be able to call DefaultWindModel(DefaultWindModel::parse(...)) from parser")
        .def_static("model_name", &DefaultWindModel::model_name)
        .def_static("parse", &DefaultWindModel::parse)
        .def("get_wind", &DefaultWindModel::get_wind,
            py::arg("position"),
            py::arg("t")
            )
        ;

    py::class_<WindMeanVelocityProfile::Input>(m_env, "WindMeanVelocityProfileInput")
        .def(py::init<>())
        .def_readwrite("velocity", &WindMeanVelocityProfile::Input::velocity)
        .def_readwrite("direction", &WindMeanVelocityProfile::Input::direction)
        ;

    py::class_<WindMeanVelocityProfile, WindModel>(m_env, "WindMeanVelocityProfile")
        // .def(py::init<const WindMeanVelocityProfile::Input& /*input*/>())
        .def("get_wind", &WindMeanVelocityProfile::get_wind,
            py::arg("position"),
            py::arg("t") = 0.0
            )
        .def_static("parse", &WindMeanVelocityProfile::parse)
        ;

    py::class_<LogWindVelocityProfile::Input, WindMeanVelocityProfile::Input>(m_env, "LogWindVelocityProfileInput")
        .def(py::init<>())
        .def_readwrite("z0", &LogWindVelocityProfile::Input::z0)
        .def_readwrite("z_ref", &LogWindVelocityProfile::Input::z_ref)
        ;

    py::class_<LogWindVelocityProfile, WindMeanVelocityProfile>(m_env, "LogWindVelocityProfile")
        .def(py::init<const LogWindVelocityProfile::Input& /*input*/>())
        .def_static("model_name", &LogWindVelocityProfile::model_name)
        .def_static("parse", &LogWindVelocityProfile::parse)
        ;

    py::class_<PowerLawWindVelocityProfile::Input, WindMeanVelocityProfile::Input>(m_env, "PowerLawWindVelocityProfileInput")
        .def(py::init<>())
        .def(py::init<const WindMeanVelocityProfile::Input& /*parent*/>())
        .def_readwrite("alpha", &PowerLawWindVelocityProfile::Input::alpha)
        .def_readwrite("z_ref", &PowerLawWindVelocityProfile::Input::z_ref)
        ;

    py::class_<PowerLawWindVelocityProfile, WindMeanVelocityProfile>(m_env, "PowerLawWindVelocityProfile")
        .def(py::init<const PowerLawWindVelocityProfile::Input& /*input*/>())
        .def_static("model_name", &PowerLawWindVelocityProfile::model_name)
        .def_static("parse", &PowerLawWindVelocityProfile::parse)
        ;

    py::class_<UniformWindVelocityProfile, WindMeanVelocityProfile>(m_env, "UniformWindVelocityProfile")
        .def(py::init<const WindMeanVelocityProfile::Input& /*input*/>())
        .def_static("model_name", &UniformWindVelocityProfile::model_name)
        .def_static("parse", &UniformWindVelocityProfile::parse)
        ;
}

void py_add_module_xdyn_env(py::module& m)
{
    py::module m_env = m.def_submodule("env", "Environment models");
    py::module m_env_wave = m_env.def_submodule("wave", "Wave environment");
    py_add_module_xdyn_env_wave(m_env_wave);
    py::module m_env_wind = m_env.def_submodule("wind", "Wind environment");
    py_add_module_xdyn_env_wind(m_env_wind);
}
