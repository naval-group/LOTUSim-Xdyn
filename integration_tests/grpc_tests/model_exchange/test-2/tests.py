"""Integration tests for gRPC model exchange."""
# pylint: disable=C0111, invalid-name, old-style-class

import copy
import logging
import math
import os
import unittest
import grpc
from xdyngrpc._proto.model_exchange_pb2 import ModelExchangeRequestEuler
from xdyngrpc._proto import model_exchange_pb2_grpc

SERVICE_NAME = "xdyn-test-client"

logging.basicConfig(
    format='%(asctime)s,%(msecs)d ['
    + SERVICE_NAME
    + '] - %(levelname)-4s [%(filename)s:%(lineno)d] %(message)s',
    datefmt='%d-%m-%Y:%H:%M:%S')
LOGGER = logging.getLogger(__name__)
LOGGER.setLevel(logging.INFO)


class XDyn:
    """Launch model exchange steps. Wrapper arround gRPC."""

    def __init__(self):
        xdyn_server_url = os.environ['xdyn_server_url']
        xdyn_channel = grpc.insecure_channel(xdyn_server_url)
        self.xdyn_stub = model_exchange_pb2_grpc.ModelExchangeStub(
            xdyn_channel)
        self.request = ModelExchangeRequestEuler()

    def dx_dt(self, state, requested_output):
        """Run a cosimulation step."""
        self.request.states.t[:] = [state['t']]
        self.request.states.x[:] = [state['x']]
        self.request.states.y[:] = [state['y']]
        self.request.states.z[:] = [state['z']]
        self.request.states.u[:] = [state['u']]
        self.request.states.v[:] = [state['v']]
        self.request.states.w[:] = [state['w']]
        self.request.states.p[:] = [state['p']]
        self.request.states.q[:] = [state['q']]
        self.request.states.r[:] = [state['r']]
        self.request.states.phi[:] = [state['phi']]
        self.request.states.theta[:] = [state['theta']]
        self.request.states.psi[:] = [state['psi']]
        self.request.requested_output[:] = requested_output
        res = self.xdyn_stub.dx_dt_euler_321(self.request)
        return {'t': res.d_dt.t,
                'x': res.d_dt.x,
                'y': res.d_dt.y,
                'z': res.d_dt.z,
                'u': res.d_dt.u,
                'v': res.d_dt.v,
                'w': res.d_dt.w,
                'p': res.d_dt.p,
                'q': res.d_dt.q,
                'r': res.d_dt.r,
                'qr': res.d_dt.qr,
                'qi': res.d_dt.qi,
                'qj': res.d_dt.qj,
                'qk': res.d_dt.qk,
                'phi': res.d_dt.phi,
                'theta': res.d_dt.theta,
                'psi': res.d_dt.psi,
                'extra_observations': res.extra_observations}


EPS = 1E-6


class Tests(unittest.TestCase):
    """All unit tests."""

    xdyn = XDyn()
    d_dt = {}
    initial_state = {}

    def setUp(self):
        self.initial_state = {'t': 2,
                              'x': 1,
                              'y': 2,
                              'z': 0,
                              'u': 4,
                              'v': 5,
                              'w': 6,
                              'p': 0,
                              'q': 0,
                              'r': 0,
                              'phi': 0,
                              'theta': 0,
                              'psi': 0}

    def d_dt(self, state):
        return self.xdyn.dx_dt(state, ['GM(TestShip)'])

    def test_can_get_extra_observations(self):
        """Extra observations should be available."""
        state0 = copy.deepcopy(self.initial_state)
        state1 = copy.deepcopy(self.initial_state)
        state1['phi'] = 10*math.pi/180
        assert 'GM(TestShip)' in self.d_dt(state0)['extra_observations']
        gm_t0 = self.d_dt(state0)['extra_observations']['GM(TestShip)']
        assert self.d_dt(state0)['extra_observations']['GM(TestShip)'] == \
        gm_t0, "Two consecutive evaluations with same inputs should yield the \
        same result."
        assert self.d_dt(state1)['extra_observations']['GM(TestShip)'] != \
        gm_t0, "GM should have changed."


if __name__ == "__main__":
    unittest.main()
