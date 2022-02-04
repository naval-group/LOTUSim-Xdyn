"""
Unit test for KtKqForceModel
"""
import unittest

import numpy as np
from xdyn.core import BodyStates, EnvironmentAndFrames
from xdyn.core.io import YamlRotation
from xdyn.data.yaml import kt_kq
from xdyn.exceptions import NumericalErrorException
from xdyn.force import KtKqForceModel


class KtKqForceModelTest(unittest.TestCase):
    """Test class for KtKqForceModel"""

    def test_can_parse(self):
        """Check that parse function produces a valid KtKqForceModeldata object"""
        data = KtKqForceModel.parse(kt_kq())
        self.assertEqual("port side propeller", data.name)
        self.assertEqual(0, data.position_of_propeller_frame.angle.phi)
        self.assertEqual(-10 * np.pi / 180.0, data.position_of_propeller_frame.angle.theta)
        self.assertEqual(-1 * np.pi / 180.0, data.position_of_propeller_frame.angle.psi)
        self.assertEqual(-4, data.position_of_propeller_frame.coordinates.x)
        self.assertEqual(-2, data.position_of_propeller_frame.coordinates.y)
        self.assertEqual(2, data.position_of_propeller_frame.coordinates.z)
        self.assertEqual("mesh(body 1)", data.position_of_propeller_frame.frame)
        self.assertEqual(1, data.relative_rotative_efficiency)
        self.assertTrue(data.rotating_clockwise)
        self.assertEqual(0.7, data.thrust_deduction_factor)
        self.assertEqual(0.9, data.wake_coefficient)
        self.assertEqual(2, data.diameter)
        self.assertTrue(
            np.allclose(
                data.J,
                np.array(
                    [
                        -1.0e00,
                        -8.0e-1,
                        -5.0e-1,
                        -2.5e-1,
                        -1.0e-3,
                        1.0e-3,
                        2.0e-1,
                        4.0e-1,
                        6.0e-1,
                        7.0e-1,
                        8.0e-1,
                        1.0e00,
                    ]
                ),
            )
        )
        self.assertTrue(
            np.allclose(
                data.Kt,
                np.array(
                    [
                        -4.5e-1,
                        -2.5e-1,
                        -1.9e-1,
                        -2.0e-1,
                        -2.0e-1,
                        3.25e-1,
                        2.8e-1,
                        2.33e-1,
                        1.85e-1,
                        1.62e-1,
                        1.36e-1,
                        8.5e-2,
                    ]
                ),
            )
        )
        self.assertTrue(
            np.allclose(
                data.Kq,
                np.array(
                    [
                        -4.8e-2,
                        -3.3e-2,
                        -2.2e-2,
                        -2.5e-2,
                        -2.8e-2,
                        3.4e-2,
                        3.26e-2,
                        2.97e-2,
                        2.55e-2,
                        2.3e-2,
                        2.04e-2,
                        1.5e-2,
                    ]
                ),
            )
        )

    def test_force(self):
        EPS: float = 1e-6
        data = KtKqForceModel.parse(kt_kq())
        env = EnvironmentAndFrames()
        env.rho = 1024
        env.rot = YamlRotation("angle", ["z", "y'", "x''"])
        model = KtKqForceModel(data, "", env)
        self.assertEqual("Kt(J) & Kq(J)", model.model_name())
        states = BodyStates()
        states.u.record(0, 1)

        commands = {"rpm": 5 * (2 * np.pi)}
        wrench = model.get_force(states, 42.0, env, commands)
        self.assertAlmostEqual(306063.03332753148, wrench.X(), delta=EPS)
        self.assertEqual(0.0, wrench.Y())
        self.assertEqual(0.0, wrench.Z())
        self.assertEqual(0.0, wrench.M())
        self.assertEqual(0.0, wrench.N())

    def test_clarify_exception_message_for_Kt_Kq_interpolation_errors(self):
        data = KtKqForceModel.parse(kt_kq())
        env = EnvironmentAndFrames()
        env.rho = 1024
        env.rot = YamlRotation("angle", ["z", "y'", "x''"])
        model = KtKqForceModel(data, "", env)
        self.assertEqual("Kt(J) & Kq(J)", model.model_name())
        states = BodyStates()
        states.u.record(0, 1)
        commands = {"rpm": 2 * np.pi * 0.025}
        expected_msg = "Unable to interpolate Kt as a function of J when using model 'Kt(J) & Kq(J)'. Got the following error: Received x0 = 2, but x0 should be within [-1,1] (xmin-x0 = -3 and x0-xmax = 1"
        with self.assertRaises(NumericalErrorException) as pcm:
            model.get_force(states, 42.0, env, commands)
        self.assertTrue(expected_msg in str(pcm.exception), str(pcm.exception))


if __name__ == "__main__":

    unittest.main()
