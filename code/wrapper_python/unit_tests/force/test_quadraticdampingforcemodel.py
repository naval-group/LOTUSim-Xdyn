"""
Unit test for QuadraticDampingForceModel
"""
import unittest

import numpy as np
from xdyn.core import BodyStates, EnvironmentAndFrames
from xdyn.core.io import YamlRotation
from xdyn.data.body import get_body
from xdyn.force import QuadraticDampingForceModel
from xdyn.ssc.kinematics import Wrench as SscWrench
from xdyn.ssc.random import DataGenerator

EPS: float = 1e-2
BODY: str = "body 1"


def get_env() -> EnvironmentAndFrames:
    env = EnvironmentAndFrames()
    env.rho = 1024
    env.rot = YamlRotation("angle", ["z", "y'", "x''"])
    return env


def get_states(body_name: str) -> BodyStates:
    """Create a body state variable with one record"""
    body = get_body(body_name)
    states = body.get_states()
    return states


class QuadraticDampingForceModelTest(unittest.TestCase):
    """Test class for QuadraticDampingForceModel"""

    def setUp(self) -> None:
        self.rng = DataGenerator(666)

    def random_double(self) -> float:
        return self.rng.random_double()()

    def random_double_between(self, low: float = 0.0, high: float = 1.0) -> float:
        return self.rng.random_double().between(low, high)()

    def random_double_greater_than_zero(self) -> float:
        return self.rng.random_double().greater_than(0.0)()

    def random_double_66_matrix(self) -> np.ndarray:
        mat = np.zeros((6, 6))
        for i in range(6):
            for j in range(6):
                mat[i, j] = self.random_double()
        return mat

    def test_example_with_null_velocities(self):
        env = get_env()
        damping_matrix = self.random_double_66_matrix()
        model = QuadraticDampingForceModel(damping_matrix, BODY, env)
        self.assertEqual("quadratic damping", model.model_name())
        states = get_states(BODY)
        t = self.random_double()
        f = model(states, t, env)
        self.assertEqual(type(f), SscWrench)
        self.assertEqual(BODY, f.get_frame())
        self.assertEqual(0, f.K())
        self.assertEqual(0, f.M())
        self.assertEqual(0, f.N())
        self.assertEqual(0, f.X())
        self.assertEqual(0, f.Y())
        self.assertEqual(0, f.Z())

    def test_example_with_random_positive_velocities_and_identity_damping_matrix(self):
        EPS = 1e-11
        damping_matrix = np.eye(6, 6)
        env = get_env()
        model = QuadraticDampingForceModel(damping_matrix, BODY, env)
        states = get_states(BODY)
        for _ in range(100):
            u = self.random_double_greater_than_zero()
            v = self.random_double_greater_than_zero()
            w = self.random_double_greater_than_zero()
            p = self.random_double_greater_than_zero()
            q = self.random_double_greater_than_zero()
            r = self.random_double_greater_than_zero()
            states.u.record(0, u)
            states.v.record(0, v)
            states.w.record(0, w)
            states.p.record(0, p)
            states.q.record(0, q)
            states.r.record(0, r)
            t = self.random_double_between()
            f = model(states, t, env)
            self.assertEqual(type(f), SscWrench)
            self.assertEqual(BODY, f.get_frame())
            self.assertEqual(-u * u, f.X(), EPS)
            self.assertEqual(-v * v, f.Y(), EPS)
            self.assertEqual(-w * w, f.Z(), EPS)
            self.assertEqual(-p * p, f.K(), EPS)
            self.assertEqual(-q * q, f.M(), EPS)
            self.assertEqual(-r * r, f.N(), EPS)

    def test_example_with_dense_damping_matrix(self):
        EPS = 1e-9
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
        model = QuadraticDampingForceModel(damping_matrix, BODY, env)
        for _ in range(100):
            u = self.random_double_between(-10.0, +10.0)
            v = self.random_double_between(-10.0, +10.0)
            w = self.random_double_between(-10.0, +10.0)
            p = self.random_double_between(-10.0, +10.0)
            q = self.random_double_between(-10.0, +10.0)
            r = self.random_double_between(-10.0, +10.0)

            states.u.record(0, u)
            states.v.record(0, v)
            states.w.record(0, w)
            states.p.record(0, p)
            states.q.record(0, q)
            states.r.record(0, r)
            uu = abs(u) * u
            vv = abs(v) * v
            ww = abs(w) * w
            pp = abs(p) * p
            qq = abs(q) * q
            rr = abs(r) * r
            t = self.random_double_between()
            f = model(states, t, env)
            self.assertEqual(BODY, f.get_frame())
            assert_near = lambda x, y: self.assertAlmostEqual(x, y, delta=EPS)
            for j in range(3):
                k = j + 3
                assert_near(
                    damping_matrix[j, 0] * uu
                    + damping_matrix[j, 1] * vv
                    + damping_matrix[j, 2] * ww
                    + damping_matrix[j, 3] * pp
                    + damping_matrix[j, 4] * qq
                    + damping_matrix[j, 5] * rr,
                    -f.force[j],
                )
                assert_near(
                    damping_matrix[k, 0] * uu
                    + damping_matrix[k, 1] * vv
                    + damping_matrix[k, 2] * ww
                    + damping_matrix[k, 3] * pp
                    + damping_matrix[k, 4] * qq
                    + damping_matrix[k, 5] * rr,
                    -f.torque[j],
                )


if __name__ == "__main__":

    unittest.main()
