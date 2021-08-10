"""Integration tests for JSON + websocket cosimulation."""

import json
import math
import logging
import os
import unittest
import websocket

SERVICE_NAME = "xdyn-client"

logging.basicConfig(
    format='%(asctime)s,%(msecs)d ['
    + SERVICE_NAME
    + '] - %(levelname)-4s [%(filename)s:%(lineno)d] %(message)s',
    datefmt='%d-%m-%Y:%H:%M:%S')
LOGGER = logging.getLogger(__name__)
LOGGER.setLevel(logging.INFO)


class Cosim:
    """Launch cosimulation steps. Wrapper arround json+ws."""

    def __init__(self):
        self.xdyn_server_url = os.environ['xdyn_server_url']

    def step(self, state, Dt, requested_output):
        """Run a cosimulation step."""
        request = { "Dt": Dt,
                    "states": [state],
                    "commands": {},
                    "requested_output": requested_output }
        ws = websocket.create_connection(self.xdyn_server_url)
        ws.send(json.dumps(request))
        res = ws.recv()
        ws.close()
        return json.loads(res)


EPS = 1E-6


class Tests(unittest.TestCase):
    """All unit tests."""

    cosim = Cosim()
    res = {}

    def setUp(self):
        state = {'t': 2,
                 'x': 1,
                 'y': 2,
                 'z': 3,
                 'u': 4,
                 'v': 5,
                 'w': 6,
                 'p': 0,
                 'q': 0,
                 'r': 0,
                 'qr': 1,
                 'qi': 0,
                 'qj': 0,
                 'qk': 0}
        requested_output = ['Fz(gravity,ball,ball)']
        self.res = self.cosim.step(state, 3, requested_output)

    def test_results_are_not_identical(self):
        """Make sure the cosimulation results do not have identical states."""
        for i in range(len(self.res['t'])):
            assert self.res['x'][i] != self.res['z'][i]
            assert self.res['z'][i] != self.res['y'][i]
            assert self.res['x'][i] != self.res['y'][i]

    def test_results_are_correct(self):
        """Make sure the cosimulation results are correct."""
        expected_z = [0.0, 0.049049963199999999999, 0.19619999999999999, 0.44144999999999995, 0.7847999999999998,
                1.2262499999999998, 1.7658, 2.40345, 3.1391999999999998, 3.9730499999999997, 4.904999999999999,
                5.9350499999999995, 7.063199999999999, 8.289449999999999, 9.613799999999998, 11.036249999999997,
                12.556799999999997, 14.175449999999998, 15.892199999999994, 17.707049999999988, 19.619999999999983,
                21.63104999999998, 23.740199999999977, 25.947449999999975, 28.25279999999997, 30.656249999999964,
                33.15779999999996, 35.75744999999995, 38.455199999999934, 41.251049999999935, 44.14499999999993]
        for i in range(len(self.res['t'])):
            t = self.res['t'][i]
            assert abs(self.res['z'][i] - expected_z[i] - 3-(t-2)*6) < EPS
            assert abs(self.res['x'][i]-(t-2)*4-1) < EPS
            assert abs(self.res['y'][i]-(t-2)*5-2) < EPS
            assert abs(self.res['u'][i]-4) < EPS
            assert abs(self.res['v'][i]-5) < EPS
            assert abs(self.res['p'][i]) < EPS
            assert abs(self.res['q'][i]) < EPS
            assert abs(self.res['r'][i]) < EPS

    def test_check_time_vector(self):
        """Time vector should have the right size & hold correct values."""
        t_ = 2
        assert len(self.res['t']) == 31
        for t in self.res['t']:
            assert abs(t - t_) < EPS
            t_ += 0.1

    def test_states_length(self):
        """States should all have the same size."""
        assert len(self.res['t']) == len(self.res['x'])
        assert len(self.res['t']) == len(self.res['y'])
        assert len(self.res['t']) == len(self.res['z'])
        assert len(self.res['t']) == len(self.res['u'])
        assert len(self.res['t']) == len(self.res['v'])
        assert len(self.res['t']) == len(self.res['w'])
        assert len(self.res['t']) == len(self.res['p'])
        assert len(self.res['t']) == len(self.res['q'])
        assert len(self.res['t']) == len(self.res['r'])
        assert len(self.res['t']) == len(self.res['qr'])
        assert len(self.res['t']) == len(self.res['qi'])
        assert len(self.res['t']) == len(self.res['qj'])
        assert len(self.res['t']) == len(self.res['qk'])
        assert len(self.res['t']) == len(self.res['phi'])
        assert len(self.res['t']) == len(self.res['theta'])
        assert len(self.res['t']) == len(self.res['psi'])
        
    def test_can_get_extra_observations(self):
        """Extra observations should be available."""
        assert 'Fz(gravity,ball,ball)' in self.res['extra_observations']
        assert len(self.res['extra_observations']['Fz(gravity,ball,ball)']) == len(self.res['t'])
        
    def test_response_starts_at_last_time_of_request(self):
        """Check that the first time stamp in the response is the last time stamp of the request."""
        assert self.res['t'][0] == 2
