"""
Unit test for GravityForceModel
"""
import unittest

import numpy as np
from xdyn.core import BodyStates, EnvironmentAndFrames
from xdyn.core.io import YamlRotation
from xdyn.data.body import get_body
from xdyn.force import GravityForceModel
from xdyn.ssc.kinematics import Point as SscPoint
from xdyn.ssc.kinematics import Transform as SscTransform


def get_states(body_name: str) -> BodyStates:
    """Create a body state variable with one record"""
    body = get_body(body_name)
    states = body.get_states()
    states.x.record(0, 0)
    states.y.record(0, 0)
    states.z.record(0, 0)
    states.u.record(0, 1)
    states.v.record(0, 2)
    states.w.record(0, 0)
    states.p.record(0, 0)
    states.q.record(0, 0)
    states.r.record(0, 3)
    states.qr.record(0, 1)
    states.qi.record(0, 0)
    states.qj.record(0, 0)
    states.qr.record(0, 0)
    return states


class GravityForceModelTest(unittest.TestCase):
    """Test class for GravityForceModel"""

    def test_example(self):
        """Check that the model result with a (0,0,0) orientation"""
        body = "body 1"
        env = EnvironmentAndFrames()
        env.g = 9.81
        env.rot = YamlRotation("angle", ["z", "y'", "x''"])
        env.k_add(SscTransform(SscPoint("NED"), body))
        states = get_states(body)
        inertia = states.get_solid_body_inertia()
        inertia[:, :] = np.zeros((6, 6))
        inertia[0, 0] = 100
        inertia[1, 1] = 100
        inertia[2, 2] = 100
        model = GravityForceModel(body, env)
        self.assertEqual("gravity", model.model_name())
        wrench = model(states, 0.0, env)
        self.assertEqual("body 1", wrench.get_frame())
        self.assertEqual(0, wrench.K())
        self.assertEqual(0, wrench.M())
        self.assertEqual(0, wrench.N())
        self.assertEqual(0, wrench.X())
        self.assertEqual(0, wrench.Y())
        self.assertEqual(981, wrench.Z())

    def test_example_with_an_orientation(self):
        """Check that the model result with an orientation different from (0,0,0)"""
        body = "body 1"
        EPS = 1e-11
        cos_phi = 0.0
        cos_theta = 0.5 * np.sqrt(3)
        cos_psi = 0.5
        sin_phi = 1.0
        sin_theta = 0.5
        sin_psi = 0.5 * np.sqrt(3)
        rot = np.array(
            [
                [
                    cos_theta * cos_psi,
                    sin_phi * sin_theta * cos_psi - cos_phi * sin_psi,
                    cos_phi * sin_theta * cos_psi + sin_phi * sin_psi,
                ],
                [
                    cos_theta * sin_psi,
                    sin_phi * sin_theta * sin_psi + cos_phi * cos_psi,
                    cos_phi * sin_theta * sin_psi - sin_phi * cos_psi,
                ],
                [-sin_theta, sin_phi * cos_theta, cos_phi * cos_theta],
            ]
        )
        env = EnvironmentAndFrames()
        env.g = 9.81
        env.rot = YamlRotation("angle", ["z", "y'", "x''"])
        env.k_add(SscTransform(rot, "NED", body))
        states = get_states(body)
        inertia = states.get_solid_body_inertia()
        inertia[:, :] = np.zeros((6, 6))
        inertia[0, 0] = 100
        inertia[1, 1] = 100
        inertia[2, 2] = 100
        model = GravityForceModel(body, env)
        self.assertEqual("gravity", model.model_name())
        wrench = model(states, 42.0, env)
        self.assertEqual(body, wrench.get_frame())
        self.assertAlmostEqual(0, wrench.K(), delta=EPS)
        self.assertAlmostEqual(0, wrench.M(), delta=EPS)
        self.assertAlmostEqual(0, wrench.N(), delta=EPS)
        self.assertAlmostEqual(-0.5 * 981, wrench.X(), delta=EPS)
        self.assertAlmostEqual(+0.5 * np.sqrt(3.0) * 981, wrench.Y(), delta=EPS)
        self.assertAlmostEqual(0.0, wrench.Z(), delta=EPS)

    def test_potential_energy(self):
        """Check that the model evaluates successfully potential energy
        with an orientation different from (0,0,0)"""
        body = "body 1"
        EPS = 1e-11
        cos_phi = 0.0
        cos_theta = 0.5 * np.sqrt(3)
        cos_psi = 0.5
        sin_phi = 1.0
        sin_theta = 0.5
        sin_psi = 0.5 * np.sqrt(3)
        rot = np.array(
            [
                [
                    cos_theta * cos_psi,
                    sin_phi * sin_theta * cos_psi - cos_phi * sin_psi,
                    cos_phi * sin_theta * cos_psi + sin_phi * sin_psi,
                ],
                [
                    cos_theta * sin_psi,
                    sin_phi * sin_theta * sin_psi + cos_phi * cos_psi,
                    cos_phi * sin_theta * sin_psi - sin_phi * cos_psi,
                ],
                [-sin_theta, sin_phi * cos_theta, cos_phi * cos_theta],
            ]
        )
        env = EnvironmentAndFrames()
        env.g = 9.81
        env.rot = YamlRotation("angle", ["z", "y'", "x''"])
        env.k_add(SscTransform(rot, "NED", body))
        model = GravityForceModel(body, env)
        states = get_states(body)
        inertia = states.get_solid_body_inertia()
        inertia[:, :] = np.zeros((6, 6))
        inertia[0, 0] = 100
        inertia[1, 1] = 100
        inertia[2, 2] = 100
        self.assertAlmostEqual(0, model.potential_energy(states, [0.0, 0.0, 0.0]))
        self.assertAlmostEqual(
            -120663.0, model.potential_energy(states, [0.0, 0.0, 123])
        )


if __name__ == "__main__":

    unittest.main()
