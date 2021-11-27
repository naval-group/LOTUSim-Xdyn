"""
Unit test for SimpleStationKeepingController
"""
import unittest

import numpy as np

from xdyn.core import BodyStates, EnvironmentAndFrames
from xdyn.core.io import YamlRotation
from xdyn.data.yaml import simple_station_keeping
from xdyn.force import SimpleStationKeepingController
from xdyn.ssc.random import DataGenerator

EPS: float = 1e-14


def get_env() -> EnvironmentAndFrames:
    env = EnvironmentAndFrames()
    env.rho = 1024
    env.rot = YamlRotation("angle", ["z", "y'", "x''"])
    return env


class SimpleStationKeepingControllerTest(unittest.TestCase):
    """Test class for SimpleStationKeepingController"""

    def setUp(self) -> None:
        self.rng = DataGenerator(666)

    def test_can_parse(self):
        """Check that parse function produces a valid SimpleStationKeepingControllerInput data object"""

        data = SimpleStationKeepingController.parse(simple_station_keeping())
        self.assertEqual(2, data.T_x)
        self.assertEqual(3, data.T_y)
        self.assertEqual(4, data.T_psi)
        self.assertEqual(0.9, data.ksi_x)
        self.assertEqual(0.85, data.ksi_y)
        self.assertEqual(0.8, data.ksi_psi)
        self.assertEqual("controller", data.name)

    def test_force_and_torque(self):
        data = SimpleStationKeepingController.parse(simple_station_keeping())
        data.T_x = 2 * np.pi
        data.T_y = 2 * np.pi / 3
        data.T_psi = 2 * np.pi / 4
        env = get_env()
        w = SimpleStationKeepingController(data, "body", env)
        self.assertEqual("simple station-keeping controller", w.model_name())
        x = 0.456
        y = 5.769
        psi = 1.234
        states = BodyStates()
        states.qr.record(0, np.cos(psi / 2))
        states.qi.record(0, 0)
        states.qj.record(0, 0)
        states.qk.record(0, np.sin(psi / 2))
        states.u.record(0, 8)
        states.v.record(0, 9)
        states.r.record(0, 10)
        states.x.record(0, x)
        states.y.record(0, y)
        total_inertia = states.get_total_inertia()
        total_inertia[0, 0] = 4
        total_inertia[1, 1] = 5
        total_inertia[5, 5] = 6

        commands = {"x_co": 5, "y_co": 6, "psi_co": 7}
        F = w.get_force(states, self.rng.random_double()(), env, commands)
        assert_equal = lambda x, y: self.assertAlmostEqual(x, y, delta=EPS)
        assert_equal(4 * 1 * (5 - x) - 2 * 0.9 * 4 * 1 * 8, F.X())
        assert_equal(5 * 9 * (6 - y) - 2 * 0.85 * 5 * 3 * 9, F.Y())
        assert_equal(0, F.Z())
        assert_equal(0, F.K())
        assert_equal(0, F.M())
        assert_equal(6 * 16 * (7 - psi) - 2 * 0.8 * 6 * 4 * 10, F.N())


if __name__ == "__main__":

    unittest.main()
