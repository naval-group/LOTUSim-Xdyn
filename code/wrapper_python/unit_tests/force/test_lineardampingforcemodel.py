"""
Unit test for LinearDampingForceModel
"""
import unittest

import numpy as np

from xdyn.core import BodyStates, EnvironmentAndFrames
from xdyn.core.io import YamlRotation
from xdyn.data.body import get_body
from xdyn.force import LinearDampingForceModel
from xdyn.ssc.kinematics import Wrench as SscWrench
from xdyn.ssc.random import DataGenerator

EPS: float = 1e-2
BODY: str = "body 1"


def get_env() -> EnvironmentAndFrames:
    env = EnvironmentAndFrames()
    env.rho = 1024
    env.rot = YamlRotation("angle", ["z", "y'", "x''"])
    return env


def get_states0(name: str) -> BodyStates:
    """Create a body state variable with one record"""
    states = BodyStates(0)
    states.name = name
    states.convention = YamlRotation("angle", ["z", "y'", "x''"])
    return states


def get_states(body_name: str) -> BodyStates:
    """Create a body state variable with one record"""
    body = get_body(body_name)
    states = body.get_states()
    return states


class LinearDampingForceModelTest(unittest.TestCase):
    """Test class for LinearDampingForceModel"""

    def setUp(self) -> None:
        self.rng = DataGenerator(666)

    def random_double_between(self, low: float = 0.0, high: float = 1.0) -> float:
        return self.rng.random_double().between(low, high)()

    def test_can_parse(self):
        """Check that parse function produces a valid LinearDampingForceModelInput data object"""

        data = """
        damping matrix at the center of gravity projected in the body frame:
            row 1: [  2,   3,   5,   7,  11,  13]
            row 2: [ 17,  19,  23,  29,  31,  37]
            row 3: [ 41,  43,  47,  53,  59,  61]
            row 4: [ 67,  71,  73,  79,  83,  89]
            row 5: [ 97, 101, 103, 107, 109, 113]
            row 6: [127, 131, 137, 139, 149, 151]
        """
        damping_matrix = LinearDampingForceModel.parse(data)
        self.assertEqual(2.0, damping_matrix[0, 0])
        self.assertEqual(3.0, damping_matrix[0, 1])
        self.assertEqual(5.0, damping_matrix[0, 2])
        self.assertEqual(7.0, damping_matrix[0, 3])
        self.assertEqual(11.0, damping_matrix[0, 4])
        self.assertEqual(13.0, damping_matrix[0, 5])
        self.assertEqual(17.0, damping_matrix[1, 0])
        self.assertEqual(19.0, damping_matrix[1, 1])
        self.assertEqual(23.0, damping_matrix[1, 2])
        self.assertEqual(29.0, damping_matrix[1, 3])
        self.assertEqual(31.0, damping_matrix[1, 4])
        self.assertEqual(37.0, damping_matrix[1, 5])
        self.assertEqual(41.0, damping_matrix[2, 0])
        self.assertEqual(43.0, damping_matrix[2, 1])
        self.assertEqual(47.0, damping_matrix[2, 2])
        self.assertEqual(53.0, damping_matrix[2, 3])
        self.assertEqual(59.0, damping_matrix[2, 4])
        self.assertEqual(61.0, damping_matrix[2, 5])
        self.assertEqual(67.0, damping_matrix[3, 0])
        self.assertEqual(71.0, damping_matrix[3, 1])
        self.assertEqual(73.0, damping_matrix[3, 2])
        self.assertEqual(79.0, damping_matrix[3, 3])
        self.assertEqual(83.0, damping_matrix[3, 4])
        self.assertEqual(89.0, damping_matrix[3, 5])
        self.assertEqual(97.0, damping_matrix[4, 0])
        self.assertEqual(101.0, damping_matrix[4, 1])
        self.assertEqual(103.0, damping_matrix[4, 2])
        self.assertEqual(107.0, damping_matrix[4, 3])
        self.assertEqual(109.0, damping_matrix[4, 4])
        self.assertEqual(113.0, damping_matrix[4, 5])
        self.assertEqual(127.0, damping_matrix[5, 0])
        self.assertEqual(131.0, damping_matrix[5, 1])
        self.assertEqual(137.0, damping_matrix[5, 2])
        self.assertEqual(139.0, damping_matrix[5, 3])
        self.assertEqual(149.0, damping_matrix[5, 4])
        self.assertEqual(151.0, damping_matrix[5, 5])

    def test_example(self):
        env = get_env()
        states = get_states(BODY)
        damping_matrix = np.array(
            [
                [2, 3, 5, 7, 11, 13],
                [17, 19, 23, 29, 31, 37],
                [41, 43, 47, 53, 59, 61],
                [67, 71, 73, 79, 83, 89],
                [97, 101, 103, 107, 109, 113],
                [127, 131, 137, 139, 149, 151],
            ]
        )
        model = LinearDampingForceModel(damping_matrix, BODY, env)
        self.assertEqual("linear damping", model.model_name())
        for _ in range(100):
            states.u.record(0, self.random_double_between(-10.0, +10.0))
            states.v.record(0, self.random_double_between(-10.0, +10.0))
            states.w.record(0, self.random_double_between(-10.0, +10.0))
            states.p.record(0, self.random_double_between(-10.0, +10.0))
            states.q.record(0, self.random_double_between(-10.0, +10.0))
            states.r.record(0, self.random_double_between(-10.0, +10.0))
            f = model(states, self.random_double_between(), env)
            self.assertEqual(type(f), SscWrench)
            self.assertEqual(BODY, f.get_frame())
            assert_near = lambda x, y: self.assertAlmostEqual(x, y, delta=EPS)
            for j in range(3):
                k = j + 3
                assert_near(
                    damping_matrix[j, 0] * states.u()
                    + damping_matrix[j, 1] * states.v()
                    + damping_matrix[j, 2] * states.w()
                    + damping_matrix[j, 3] * states.p()
                    + damping_matrix[j, 4] * states.q()
                    + damping_matrix[j, 5] * states.r(),
                    -f.force[j],
                )
                assert_near(
                    damping_matrix[k, 0] * states.u()
                    + damping_matrix[k, 1] * states.v()
                    + damping_matrix[k, 2] * states.w()
                    + damping_matrix[k, 3] * states.p()
                    + damping_matrix[k, 4] * states.q()
                    + damping_matrix[k, 5] * states.r(),
                    -f.torque[j],
                )


if __name__ == "__main__":

    unittest.main()
