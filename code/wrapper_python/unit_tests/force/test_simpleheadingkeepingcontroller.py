"""
Unit test for SimpleHeadingKeepingController
"""
import unittest

import numpy as np
from xdyn.core import BodyStates, EnvironmentAndFrames
from xdyn.core.io import YamlRotation
from xdyn.data.yaml import simple_track_keeping
from xdyn.force import SimpleHeadingKeepingController
from xdyn.ssc.random import DataGenerator

EPS: float = 1e-14

def get_env() -> EnvironmentAndFrames:
    env = EnvironmentAndFrames()
    env.rho = 1024
    env.rot = YamlRotation("angle", ["z", "y'", "x''"])
    return env


class SimpleHeadingKeepingControllerTest(unittest.TestCase):
    """Test class for SimpleHeadingKeepingController"""

    def setUp(self) -> None:
        self.rng = DataGenerator(666)

    def test_can_parse(self):
        """Check that parse function produces a valid SimpleHeadingKeepingControllerInput data object"""

        data = SimpleHeadingKeepingController.parse(simple_track_keeping())
        self.assertEqual(4, data.Tp)
        self.assertEqual(0.9, data.ksi)
        self.assertEqual("controller", data.name)

    def test_force_and_torque(self):
        data = SimpleHeadingKeepingController.parse(simple_track_keeping())
        data.Tp = 2 * np.pi
        env = get_env()
        w = SimpleHeadingKeepingController(data, "body", env)
        self.assertEqual("simple heading controller", w.model_name())
        states = BodyStates()
        psi = 1.234
        states.qr.record(0, np.cos(psi / 2))
        states.qi.record(0, 0)
        states.qj.record(0, 0)
        states.qk.record(0, np.sin(psi / 2))
        states.r.record(0, 10)
        states.get_total_inertia()[2, 2] = 4
        commands = {"psi_co": 5}
        F = w.get_force(states, self.rng.random_double()(), env, commands)

        assert_equal = lambda x, y: self.assertAlmostEqual(x, y, delta=EPS)
        assert_equal(0, F.X())
        assert_equal(0, F.Y())
        assert_equal(0, F.Z())
        assert_equal(0, F.K())
        assert_equal(0, F.M())
        assert_equal(4 * 1 * (5 - psi) - 2 * 0.9 * 4 * 1 * 10, F.N())


if __name__ == "__main__":

    unittest.main()
