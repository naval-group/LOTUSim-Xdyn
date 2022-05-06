import grpc
import numpy as np

from waves import LOGGER, Waves

DATA_NO_ENV = """
rotations convention: [psi, theta', phi'']

environmental constants:
    g: {value: 9.81, unit: m/s^2}
    rho: {value: 1000, unit: kg/m^3}
    nu: {value: 1.18e-6, unit: m^2/s}
environment models: []
"""

DATA_NO_WAVE = """
rotations convention: [psi, theta', phi'']
environmental constants:
    g: {value: 9.81, unit: m/s^2}
    rho: {value: 1000, unit: kg/m^3}
    nu: {value: 1.18e-6, unit: m^2/s}
environment models:
    - constant sea elevation in NED frame:
        unit: m
        value: 0
      model: no waves
"""

DATA_WAVE_MONOCHROMATIC = """
rotations convention: [psi, theta', phi'']
environmental constants:
    g: {value: 9.81, unit: m/s^2}
    rho: {value: 1000, unit: kg/m^3}
    nu: {value: 1.18e-6, unit: m^2/s}
environment models:
- model: airy
  depth: {value: 100, unit: m}
  seed of the random data generator: 0
  stretching:
     delta: 0
     h: {unit: m, value: 100}
  spectral density:
    type: dirac
    Hs: {value: 5, unit: m}
    omega0: {value: 15, unit: rad/s}
  directional spreading:
    type: dirac
    waves propagating to: {value: 0, unit: deg}
"""

DATA_WAVE_TWO_SPECTRA = """
rotations convention: [psi, theta', phi'']

environmental constants:
    g: {value: 9.81, unit: m/s^2}
    rho: {value: 1026, unit: kg/m^3}
    nu: {value: 1.18e-6, unit: m^2/s}
environment models:
  - model: waves
    discretization:
       ndir: 128
       nfreq: 128
       omega min: {value: 0.1, unit: rad/s}
       omega max: {value: 6, unit: rad/s}
       energy fraction: 0.999
       equal energy bins: false
    spectra:
      - model: airy
        depth: {value: 100, unit: m}
        seed of the random data generator: 0
        stretching:
          delta: 1
          h: {unit: m, value: 0}
        directional spreading:
           type: dirac
           waves propagating to: {value: 90, unit: deg}
        spectral density:
           type: jonswap
           Hs: {value: 5, unit: m}
           Tp: {value: 15, unit: s}
           gamma: 1.2
      - model: airy
        depth: {value: 100, unit: m}
        seed of the random data generator: 10
        stretching:
          delta: 1
          h: {unit: m, value: 0}
        directional spreading:
           type: cos2s
           s: 2
           waves propagating to: {value: 90, unit: deg}
        spectral density:
           type: dirac
           omega0: {value: 0.05, unit: rad/s}
           Hs: {value: 15, unit: m}
"""


def test_01_no_wave():
    """Launch the server & run some gRPC calls."""
    with grpc.insecure_channel("waves-server:50051") as channel:
        parameters = DATA_NO_WAVE
        LOGGER.info("Creating Waves instance")
        waves = Waves(channel, parameters)
        elevations_request = {"t": 5, "points": [(1, 2), (3, 5)]}
        elevations = waves.elevations(elevations_request)
        for elevation in elevations:
            assert np.allclose(elevation["z"], 0.0)
        orbital_velocities_request = {"t": 5, "points": [(1, 2, 10), (3, 5, 15)]}
        orbital_velocities = waves.orbital_velocities(orbital_velocities_request)
        for orbital_velocity in orbital_velocities:
            assert np.allclose(orbital_velocity["vx"], 0.0)
            assert np.allclose(orbital_velocity["vy"], 0.0)
            assert np.allclose(orbital_velocity["vz"], 0.0)
        dynamic_pressures_request = {"t": 5, "points": [(1, 2, 10), (3, 5, 15)]}
        dynamic_pressures = waves.dynamic_pressures(dynamic_pressures_request)
        for dynamic_pressure in dynamic_pressures:
            assert np.allclose(dynamic_pressure["pdyn"], 0.0)
        spectrum = waves.spectrum()
        assert spectrum == []
        angular_frequencies_for_rao = waves.angular_frequencies_for_rao()
        assert angular_frequencies_for_rao == []
        directions_for_rao = waves.directions_for_rao()
        assert directions_for_rao == []


def test_02_monochromatic_spectra():
    pass


def test_03_two_spectra():
    with grpc.insecure_channel("waves-server:50051") as channel:
        parameters = DATA_WAVE_TWO_SPECTRA
        LOGGER.info("Creating Waves instance")
        waves = Waves(channel, parameters)
        elevations_request = {"t": 5, "points": [(1, 2), (3, 5)]}
        elevations = waves.elevations(elevations_request)
        for elevation in elevations:
            assert not np.allclose(elevation["z"], 0.0)
        orbital_velocities_request = {"t": 5, "points": [(1, 2, 10), (3, 5, 15)]}
        orbital_velocities = waves.orbital_velocities(orbital_velocities_request)
        for orbital_velocity in orbital_velocities:
            assert not np.allclose(orbital_velocity["vx"], 0.0)
            assert not np.allclose(orbital_velocity["vy"], 0.0)
            assert not np.allclose(orbital_velocity["vz"], 0.0)
        dynamic_pressures_request = {"t": 5, "points": [(1, 2, 10), (3, 5, 15)]}
        dynamic_pressures = waves.dynamic_pressures(dynamic_pressures_request)
        for dynamic_pressure in dynamic_pressures:
            assert not np.allclose(dynamic_pressure["pdyn"], 0.0)
        spectrum = waves.spectrum()
        assert spectrum != []
        angular_frequencies_for_rao = waves.angular_frequencies_for_rao()
        assert angular_frequencies_for_rao != []
        directions_for_rao = waves.directions_for_rao()
        assert directions_for_rao != []


def main():
    test_01_no_wave()
    test_02_monochromatic_spectra()
    test_03_two_spectra()


if __name__ == "__main__":
    main()
