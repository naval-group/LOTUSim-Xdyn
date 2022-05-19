"""Integration tests for gRPC model exchange."""
# pylint: disable=C0111, invalid-name, old-style-class

import logging
import os
import unittest
import grpc
from model_exchange_pb2 import ModelExchangeRequestEuler
import model_exchange_pb2_grpc

SERVICE_NAME = "xdyn-client"

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
                              'z': 3,
                              'u': 4,
                              'v': 5,
                              'w': 6,
                              'p': 0,
                              'q': 0,
                              'r': 0,
                              'phi': 0,
                              'theta': 0,
                              'psi': 0}
        requested_output = ['Fz(gravity,ball,ball)','My(gravity,ball,ball)']
        self.d_dt = self.xdyn.dx_dt(self.initial_state, requested_output)

    def test_can_call_the_model_over_grpc(self):
        """Make sure the derivatives resul are correct."""
        assert abs(self.d_dt['x'] - self.initial_state['u']) < EPS
        assert abs(self.d_dt['y'] - self.initial_state['v']) < EPS
        assert abs(self.d_dt['u']) < EPS
        assert abs(self.d_dt['v']) < EPS
        assert abs(self.d_dt['w']-9.81) < EPS
        assert abs(self.d_dt['p']) < EPS
        assert abs(self.d_dt['q']) < EPS
        assert abs(self.d_dt['r']) < EPS
        assert abs(self.d_dt['r']) < EPS
        assert abs(self.d_dt['qr']) < EPS
        assert abs(self.d_dt['qi']) < EPS
        assert abs(self.d_dt['qj']) < EPS
        assert abs(self.d_dt['qk']) < EPS
        assert abs(self.d_dt['z'] - self.initial_state['w']) < EPS
        assert abs(self.d_dt['phi']) < EPS
        assert abs(self.d_dt['theta']) < EPS
        assert abs(self.d_dt['psi']) < EPS

    def test_can_get_extra_observations(self):
        """Extra observations should be available."""
        assert 'Fz(gravity,ball,ball)' in self.d_dt['extra_observations']
        assert 'My(gravity,ball,ball)' in self.d_dt['extra_observations']

    def test_can_recover_from_error(self):
        """Check that xdyn does not stay locked in an error state after a failed gRPC."""
        request = ModelExchangeRequestEuler()
        request.states.t[:] = [2]  # v, w, p and q are missing
        request.states.x[:] = [1]
        request.states.y[:] = [2]
        request.states.z[:] = [3]
        request.states.u[:] = [4]
        request.states.r[:] = [0]
        request.states.phi[:] = [0]
        request.states.theta[:] = [0]
        request.states.psi[:] = [0]
        with self.assertRaises(grpc._channel._InactiveRpcError):
            self.xdyn.xdyn_stub.dx_dt_euler_321(request)  # This is expected to fail
        request.states.v[:] = [5]
        request.states.w[:] = [6]
        request.states.p[:] = [0]
        request.states.q[:] = [0]
        self.xdyn.xdyn_stub.dx_dt_euler_321(request)  # This should succeed
