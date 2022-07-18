"""Integration tests for gRPC model exchange."""
# pylint: disable=C0111, invalid-name, old-style-class

import copy
import logging
import math
import os
import unittest

from xdyngrpc.modelexchange import ModelExchangeEuler

SERVICE_NAME = "xdyn-test-client"

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
            "z": 0,
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

    def d_dt(self, state):
        return self.xdyn.dx_dt(state, ["GM(TestShip)"])

    def test_can_get_extra_observations(self):
        """Extra observations should be available."""
        state0 = copy.deepcopy(self.initial_state)
        state1 = copy.deepcopy(self.initial_state)
        state1["phi"] = 10 * math.pi / 180
        assert "GM(TestShip)" in self.d_dt(state0)["extra_observations"]
        gm_t0 = self.d_dt(state0)["extra_observations"]["GM(TestShip)"]
        assert (
            self.d_dt(state0)["extra_observations"]["GM(TestShip)"] == gm_t0
        ), "Two consecutive evaluations with same inputs should yield the \
        same result."
        assert (
            self.d_dt(state1)["extra_observations"]["GM(TestShip)"] != gm_t0
        ), "GM should have changed."


if __name__ == "__main__":
    unittest.main()
