"""Integration tests for gRPC model exchange."""
# pylint: disable=C0111, invalid-name, old-style-class

import logging
import os
import unittest

import grpc
from xdyngrpc._proto.model_exchange_pb2 import ModelExchangeRequestEuler
from xdyngrpc.modelexchange import ModelExchangeEuler

SERVICE_NAME = "xdyn-client"

logging.basicConfig(
    format="%(asctime)s,%(msecs)d ["
    + SERVICE_NAME
    + "] - %(levelname)-4s [%(filename)s:%(lineno)d] %(message)s",
    datefmt="%d-%m-%Y:%H:%M:%S",
)
LOGGER = logging.getLogger(__name__)
LOGGER.setLevel(logging.INFO)


EPS = 1e-6


class Tests(unittest.TestCase):
    """All unit tests."""

    xdyn = ModelExchangeEuler(os.environ["xdyn_server_url"])
    d_dt = {}
    initial_state = {}

    def setUp(self):
        self.initial_state = {
            "t": 2,
            "x": 1,
            "y": 2,
            "z": 3,
            "u": 4,
            "v": 5,
            "w": 6,
            "p": 0,
            "q": 0,
            "r": 0,
            "phi": 0,
            "theta": 0,
            "psi": 0,
        }
        requested_output = ["Fz(gravity,ball,ball)", "My(gravity,ball,ball)"]
        self.d_dt = self.xdyn.dx_dt(self.initial_state, requested_output)

    def test_can_call_the_model_over_grpc(self):
        """Make sure the derivatives resul are correct."""
        assert abs(self.d_dt["x"] - self.initial_state["u"]) < EPS
        assert abs(self.d_dt["y"] - self.initial_state["v"]) < EPS
        assert abs(self.d_dt["u"]) < EPS
        assert abs(self.d_dt["v"]) < EPS
        assert abs(self.d_dt["w"] - 9.81) < EPS
        assert abs(self.d_dt["p"]) < EPS
        assert abs(self.d_dt["q"]) < EPS
        assert abs(self.d_dt["r"]) < EPS
        assert abs(self.d_dt["r"]) < EPS
        assert abs(self.d_dt["qr"]) < EPS
        assert abs(self.d_dt["qi"]) < EPS
        assert abs(self.d_dt["qj"]) < EPS
        assert abs(self.d_dt["qk"]) < EPS
        assert abs(self.d_dt["z"] - self.initial_state["w"]) < EPS
        assert abs(self.d_dt["phi"]) < EPS
        assert abs(self.d_dt["theta"]) < EPS
        assert abs(self.d_dt["psi"]) < EPS

    def test_can_get_extra_observations(self):
        """Extra observations should be available."""
        assert "Fz(gravity,ball,ball)" in self.d_dt["extra_observations"]
        assert "My(gravity,ball,ball)" in self.d_dt["extra_observations"]

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


if __name__ == "__main__":
    unittest.main()
