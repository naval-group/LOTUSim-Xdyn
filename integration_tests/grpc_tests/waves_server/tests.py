import unittest
import grpc
import numpy as np

from xdyngrpc.waves.client.waves import LOGGER, Waves

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
  - model: waves
    discretization:
       ndir: 1
       nfreq: 1
       omega min: {value: 0.1, unit: rad/s}
       omega max: {value: 6, unit: rad/s}
       energy fraction: 1
       equal energy bins: false
    spectra:
    - model: airy
      depth: {value: 1e9, unit: m} # Infinite depth
      seed of the random data generator: 0
      stretching:
          delta: 0
          h: {unit: m, value: 1e9}
      spectral density:
          type: dirac
          Hs: {value: 5, unit: m}
          omega0: {value: 0.5235987755982988, unit: rad/s} # Tp = 12 sec
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


class AiryWaveGrpcServerTest(unittest.TestCase):
    """Test class for Airy wave gRPC server"""

    def test_00_invalid_parameters(self):
        """Check that an invalid string can not build a gRPC Waves client"""
        with grpc.insecure_channel("waves-server:50051") as channel:
            LOGGER.info("Creating Waves instance")
            with self.assertRaises(Exception) as pcm:
                Waves(channel, "random string")
            LOGGER.info(pcm.exception)
            LOGGER.info(pcm.exception.details)

    def test_00_incomplete_yaml_parameters(self):
        """Check that an input without a wave model can not build a gRPC Waves client"""
        with grpc.insecure_channel("waves-server:50051") as channel:
            LOGGER.info("Creating Waves instance")
            with self.assertRaises(Exception) as pcm:
                Waves(channel, DATA_NO_ENV)
            LOGGER.info(pcm.exception)
            LOGGER.info(pcm.exception.details)

    def test_00_invalid_requests(self):
        """Check that exceptions are raised with invalid requests"""
        with grpc.insecure_channel("waves-server:50051") as channel:
            LOGGER.info("Creating Waves instance")
            waves = Waves(channel, DATA_NO_WAVE)
            with self.assertRaises(KeyError):
                waves.elevations({"points": [(1, 2), (3, 5)]})
            with self.assertRaises(Exception):
                waves.elevations({"t": 10, "points": [(1, 2), (3, 5), (1,)]})
            with self.assertRaises(Exception):
                waves.dynamic_pressures({"t": 5, "points": [(1, 2, 10), (3, 5,)]})
            with self.assertRaises(Exception):
                waves.orbital_velocities({"t": 5, "points": [(1, 2, 10), (3, 5,)]})

    def test_01_no_wave(self):
        """Test that all responses are 0 when no wave is defined"""
        with grpc.insecure_channel("waves-server:50051") as channel:
            LOGGER.info("Creating Waves instance")
            waves = Waves(channel, DATA_NO_WAVE)
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

    def test_02_monochromatic_spectrum(self):
        """Test all responses for a monochromatic wave spectrum
        Monochromatic wave phase is not known, so assertions made on elevation,
        orbital velocities and dynamic pressure are different from zero.
        """
        with grpc.insecure_channel("waves-server:50051") as channel:
            LOGGER.info("Creating Waves instance")
            waves = Waves(channel, DATA_WAVE_MONOCHROMATIC)
            elevations_request = {"t": 5, "points": [(1, 2), (3, 5)]}
            elevations = waves.elevations(elevations_request)
            for elevation in elevations:
                assert not np.allclose(elevation["z"], 0.0)
            orbital_velocities_request = {"t": 5, "points": [(1, 2, 10), (3, 5, 15)]}
            orbital_velocities = waves.orbital_velocities(orbital_velocities_request)
            for orbital_velocity in orbital_velocities:
                assert not np.allclose(orbital_velocity["vx"], 0.0)
                assert np.allclose(orbital_velocity["vy"], 0.0)
                assert not np.allclose(orbital_velocity["vz"], 0.0)
            dynamic_pressures_request = {"t": 5, "points": [(1, 2, 10), (3, 5, 15)]}
            dynamic_pressures = waves.dynamic_pressures(dynamic_pressures_request)
            for dynamic_pressure in dynamic_pressures:
                assert not np.allclose(dynamic_pressure["pdyn"], 0.0)
            spectrum = waves.spectrum()
            assert len(spectrum) == 1
            LOGGER.info(spectrum)
            assert spectrum[0]["dj"][0] == 1.0
            assert (2 * spectrum[0]["si"][0]) ** 0.5 == (0.5 * 5.0)
            assert spectrum[0]["omega"] == [0.5235987755982988]
            assert spectrum[0]["psi"] == [0.0]
            assert spectrum[0]["k"] == [0.5235987755982988**2 / 9.81]
            angular_frequencies_for_rao = waves.angular_frequencies_for_rao()
            assert len(angular_frequencies_for_rao) == 1
            assert angular_frequencies_for_rao[0] == [0.5235987755982988]
            directions_for_rao = waves.directions_for_rao()
            assert len(directions_for_rao) == 1
            assert directions_for_rao[0] == [0]

    def test_03_two_spectra(self):
        """Test server works with two spectra, that every response has the
        right size.
        """
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


if __name__ == "__main__":
    unittest.main()
