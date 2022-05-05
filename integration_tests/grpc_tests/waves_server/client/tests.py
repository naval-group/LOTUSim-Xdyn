from waves import Waves
from waves import LOGGER
import numpy as np
import json
import grpc


DATA_NO_WAVE_1d = \
"""
rotations convention: [psi, theta', phi'']

environmental constants:
    g: {value: 9.81, unit: m/s^2}
    rho: {value: 1000, unit: kg/m^3}
    nu: {value: 1.18e-6, unit: m^2/s}
environment models: []
"""

DATA_NO_WAVE = \
"""
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

def test_01_no_wave():
    """Launch the server & run some gRPC calls."""
    with grpc.insecure_channel('server:50051') as channel:
        parameters = DATA_NO_WAVE
        LOGGER.info('Creating Waves instance')
        waves = Waves(channel, parameters)
        elevations_request = {'t': 5, 'points': [(1, 2), (3, 5)]}
        LOGGER.info('Elevations for %s', json.dumps(elevations_request))
        elevations = waves.elevations(elevations_request)
        expected_elevations  = [{'t':5,'x':1,'y':2,'z':0.0}, {'t':5,'x':2,'y':5,'z':0.0}]
        for expected_elevation in expected_elevations:
            assert np.allclose(expected_elevation['z'], 0.0)
        LOGGER.info('Got following elevations from server: %s', json.dumps(elevations))
        orbital_velocities_request = {'t': 5, 'points': [(1, 2, 10), (3, 5, 15)]}
        orbital_velocities = waves.orbital_velocities(orbital_velocities_request)
        LOGGER.info('Got following orbital velocities from server: %s', json.dumps(orbital_velocities))
        dynamic_pressures_request = {'t': 5, 'points': [(1, 2, 10), (3, 5, 15)]}
        dynamic_pressures = waves.dynamic_pressures(dynamic_pressures_request)
        LOGGER.info('Got following dynamic pressures from server: %s', json.dumps(dynamic_pressures))
        spectrum = waves.spectrum()
        LOGGER.info('Got following spectrum from server: %s', json.dumps(spectrum))
        angular_frequencies_for_rao = waves.angular_frequencies_for_rao()
        LOGGER.info('Got following angular_frequencies_for_rao from server: %s', json.dumps(angular_frequencies_for_rao))
        directions_for_rao = waves.directions_for_rao()
        LOGGER.info('Got following directions_for_rao from server: %s', json.dumps(directions_for_rao))

def main():
    test_01_no_wave()

if __name__ =="__main__":
    main()