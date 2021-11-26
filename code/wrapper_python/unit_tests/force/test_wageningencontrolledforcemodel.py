"""
Unit test for WageningenControlledForceModel
"""
import io
import re
import unittest
from contextlib import redirect_stderr
import unittest

import numpy as np
from xdyn.core import BodyStates, EnvironmentAndFrames
from xdyn.core.io import YamlRotation
from xdyn.data.yaml import wageningen
from xdyn.exceptions import InvalidInputException
from xdyn.force import (
    WageningenControlledForceModel,
    WageningenControlledForceModelInput,
)

EPS: float = 1e-2
NB_TRIALS: int = 100


def get_env() -> EnvironmentAndFrames:
    env = EnvironmentAndFrames()
    env.rho = 1024
    env.rot = YamlRotation("angle", ["z", "y'", "x''"])
    return env


class WageningenControlledForceModelTest(unittest.TestCase):
    """Test class for WageningenControlledForceModel"""

    def setUp(self) -> None:
        self.rng = np.random.default_rng(666)

    def test_can_parse(self):
        """Check that parse function produces a valid WageningenControlledForceModelInput data object"""
        data = WageningenControlledForceModel.parse(wageningen())
        self.assertEqual(0.5, data.blade_area_ratio)
        self.assertEqual("port side propeller", data.name)
        self.assertEqual(3, data.number_of_blades)
        self.assertEqual(0, data.position_of_propeller_frame.angle.phi)
        self.assertEqual(
            -10 * np.pi / 180.0, data.position_of_propeller_frame.angle.theta
        )
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

    def test_should_throw_if_blade_area_ratio_is_outside_bounds(self):
        data = WageningenControlledForceModel.parse(wageningen())
        env = get_env()
        expected_msg = "Invalid number of blade area ratio AE_A0 received: expected 0.3 <= AE_A0 <= 1.05 but got AE_A0="

        def check_raises(data: WageningenControlledForceModelInput):
            with self.assertRaises(InvalidInputException) as pcm:
                WageningenControlledForceModel(data, "", env)
            self.assertTrue(expected_msg in str(pcm.exception), str(pcm.exception))

        for _ in range(NB_TRIALS):
            data.blade_area_ratio = self.rng.uniform(low=0.0, high=0.3)
            check_raises(data)
            data.blade_area_ratio = self.rng.uniform(low=1.06, high=10)
            check_raises(data)
            data.blade_area_ratio = self.rng.uniform(low=-10, high=0.3)
            check_raises(data)
            data.blade_area_ratio = self.rng.uniform(low=0.3, high=1.05)
            WageningenControlledForceModel(data, "", env)

    def test_should_throw_if_number_of_blades_is_outside_bounds(self):
        expected_msg = "Invalid number of blades Z received: expected 2 <= Z <= 7"
        data = WageningenControlledForceModel.parse(wageningen())
        env = get_env()

        def check_raises(data: WageningenControlledForceModelInput):
            with self.assertRaises(InvalidInputException) as pcm:
                WageningenControlledForceModel(data, "", env)
            self.assertTrue(expected_msg in str(pcm.exception), str(pcm.exception))

        def check_no_raises(data: WageningenControlledForceModelInput):
            WageningenControlledForceModel(data, "", env)

        data.number_of_blades = 0
        check_raises(data)
        data.number_of_blades = 1
        check_raises(data)
        data.number_of_blades = 8
        check_raises(data)
        data.number_of_blades = 2
        check_no_raises(data)
        data.number_of_blades = 3
        check_no_raises(data)
        data.number_of_blades = 4
        check_no_raises(data)
        data.number_of_blades = 5
        check_no_raises(data)
        data.number_of_blades = 6
        check_no_raises(data)
        data.number_of_blades = 7
        check_no_raises(data)
        for _ in range(NB_TRIALS):
            data.number_of_blades = self.rng.integers(low=8, high=int(1e6))
            check_raises(data)

    def test_Kt_should_issue_a_warning_if_P_D_is_outside_bounds(self):
        pass

    def test_Kt_should_throw_if_J_is_outside_bounds(self):
        pass

    def test_Kq_should_throw_if_P_D_is_outside_bounds(self):
        pass

    def test_Kq_should_throw_if_J_is_outside_bounds(self):
        pass

    def test_KT(self):
        data = WageningenControlledForceModel.parse(wageningen())
        env = get_env()
        model = WageningenControlledForceModel(data, "", env)
        self.assertEqual("wageningen B-series", model.model_name())
        almost_equal = lambda x, y, delta=EPS: self.assertAlmostEqual(x, y, delta=delta)
        # B6-65 (cf. The Wageningen Propeller Series, 1992, Gert Kuiper, Marin publication 92-001 page 128
        almost_equal(0.603363, model.Kt(Z=6, AE_A0=0.65, P_D=1.4, J=0))
        almost_equal(0.544592015, model.Kt(Z=6, AE_A0=0.65, P_D=1.4, J=0.3))
        almost_equal(0.569237985, model.Kt(Z=6, AE_A0=0.65, P_D=1.4, J=0.2))
        almost_equal(0.588950654, model.Kt(Z=6, AE_A0=0.65, P_D=1.4, J=0.1))
        almost_equal(0.161314404, model.Kt(Z=6, AE_A0=0.65, P_D=1.4, J=1.2))
        almost_equal(0.10751365, model.Kt(Z=6, AE_A0=0.65, P_D=1.4, J=1.3))
        almost_equal(0.05281524, model.Kt(Z=6, AE_A0=0.65, P_D=1.4, J=1.4))
        # B2-30 (cf. The Wageningen Propeller Series, 1992, Gert Kuiper, Marin publication 92-001 page 111
        almost_equal(0.242745716, model.Kt(Z=2, AE_A0=0.30, P_D=0.7, J=0))
        almost_equal(0.21922492, model.Kt(Z=2, AE_A0=0.30, P_D=0.7, J=0.1))
        almost_equal(0.193313888, model.Kt(Z=2, AE_A0=0.30, P_D=0.7, J=0.2))
        almost_equal(0.165272939, model.Kt(Z=2, AE_A0=0.30, P_D=0.7, J=0.3))
        almost_equal(0.13535988, model.Kt(Z=2, AE_A0=0.30, P_D=0.7, J=0.4))
        almost_equal(0.103833149, model.Kt(Z=2, AE_A0=0.30, P_D=0.7, J=0.5))
        almost_equal(0.070951181, model.Kt(Z=2, AE_A0=0.30, P_D=0.7, J=0.6))
        # B3-40
        almost_equal(0.188195882, model.Kt(Z=3, AE_A0=0.4, P_D=0.5, J=0.0))
        almost_equal(0.163443634, model.Kt(Z=3, AE_A0=0.4, P_D=0.5, J=0.1))
        almost_equal(0.135394546, model.Kt(Z=3, AE_A0=0.4, P_D=0.5, J=0.2))
        almost_equal(0.104372086, model.Kt(Z=3, AE_A0=0.4, P_D=0.5, J=0.3))

    def test_KQ(self):
        data = WageningenControlledForceModel.parse(wageningen())
        env = get_env()
        model = WageningenControlledForceModel(data, "", env)
        self.assertEqual("wageningen B-series", model.model_name())
        almostEqual = lambda x, y, delta=EPS: self.assertAlmostEqual(x, y, delta=delta)
        almostEqual(0.47, 10 * model.Kq(Z=2, AE_A0=0.30, P_D=1.4, J=0.8), delta=1e-2)
        almostEqual(0.51, 10 * model.Kq(Z=3, AE_A0=0.65, P_D=1.2, J=0.7), delta=1e-2)
        almostEqual(0.31, 10 * model.Kq(Z=3, AE_A0=0.65, P_D=0.8, J=0.3), delta=1e-2)
        almostEqual(0.51, 10 * model.Kq(Z=6, AE_A0=0.65, P_D=1.2, J=0.8), delta=1e-2)
        almostEqual(0.99, 10 * model.Kq(Z=6, AE_A0=0.65, P_D=1.4, J=0.5), delta=1e-2)
        almostEqual(0.20, 10 * model.Kq(Z=2, AE_A0=0.30, P_D=1.2, J=1), delta=1e-2)

        # B6-65 (cf. The Wageningen Propeller Series, 1992, Gert Kuiper, Marin publication 92-001 page 128
        almostEqual(1.209493726, 10 * model.Kq(Z=6, AE_A0=0.65, P_D=1.4, J=0))
        almostEqual(1.1769086, 10 * model.Kq(Z=6, AE_A0=0.65, P_D=1.4, J=0.1))
        almostEqual(1.138594465, 10 * model.Kq(Z=6, AE_A0=0.65, P_D=1.4, J=0.2))
        almostEqual(1.094459956, 10 * model.Kq(Z=6, AE_A0=0.65, P_D=1.4, J=0.3))
        almostEqual(0.420257329, 10 * model.Kq(Z=6, AE_A0=0.65, P_D=1.4, J=1.2))
        almostEqual(0.312894007, 10 * model.Kq(Z=6, AE_A0=0.65, P_D=1.4, J=1.3))
        almostEqual(0.198705297, 10 * model.Kq(Z=6, AE_A0=0.65, P_D=1.4, J=1.4))

        # B2-30 (cf. The Wageningen Propeller Series, 1992, Gert Kuiper, Marin publication 92-001 page 111
        almostEqual(0.654180539, 10 * model.Kq(Z=2, AE_A0=0.30, P_D=1.2, J=0))
        almostEqual(0.618341564, 10 * model.Kq(Z=2, AE_A0=0.30, P_D=1.2, J=0.1))
        almostEqual(0.580406412, 10 * model.Kq(Z=2, AE_A0=0.30, P_D=1.2, J=0.2))
        almostEqual(0.540388943, 10 * model.Kq(Z=2, AE_A0=0.30, P_D=1.2, J=0.3))
        almostEqual(0.498303012, 10 * model.Kq(Z=2, AE_A0=0.30, P_D=1.2, J=0.4))
        almostEqual(0.454162477, 10 * model.Kq(Z=2, AE_A0=0.30, P_D=1.2, J=0.5))
        almostEqual(0.407981197, 10 * model.Kq(Z=2, AE_A0=0.30, P_D=1.2, J=0.6))

        # B3-40
        almostEqual(0.160531235, 10 * model.Kq(Z=3, AE_A0=0.4, P_D=0.5, J=0.0))
        almostEqual(0.143676942, 10 * model.Kq(Z=3, AE_A0=0.4, P_D=0.5, J=0.1))
        almostEqual(0.125396067, 10 * model.Kq(Z=3, AE_A0=0.4, P_D=0.5, J=0.2))
        almostEqual(0.105448718, 10 * model.Kq(Z=3, AE_A0=0.4, P_D=0.5, J=0.3))

    def test_can_calculate_advance_ratio(self):
        data = WageningenControlledForceModel.parse(wageningen())
        env = get_env()
        model = WageningenControlledForceModel(data, "", env)
        states = BodyStates()
        states.u.record(0, 3)
        commands = {"rpm": 20 * 2 * np.pi}
        self.assertAlmostEqual(
            3.0 / 400.0, model.advance_ratio(states, commands), delta=1e-5
        )

    def test_force(self):
        data = WageningenControlledForceModel.parse(wageningen())
        data.blade_area_ratio = 0.4
        env = get_env()
        model = WageningenControlledForceModel(data, "", env)
        states = BodyStates()
        states.u.record(0, 1)

        commands = {"rpm": 5 * 2 * np.pi, "P/D": 0.5}

        self.assertAlmostEqual(
            0.3 * 1024 * 25 * 16 * 0.18587823151195928539,
            model.get_force(states, self.rng.uniform(), env, commands).X(),
            delta=EPS,
        )
        wrench = model.get_force(states, self.rng.uniform(), env, commands)
        self.assertEqual(0, wrench.Y())
        self.assertEqual(0, wrench.Z())
        self.assertEqual(0, wrench.M())
        self.assertEqual(0, wrench.N())

    def test_torque(self):
        data = WageningenControlledForceModel.parse(wageningen())
        data.blade_area_ratio = 0.4
        env = get_env()
        model = WageningenControlledForceModel(data, "", env)
        states = BodyStates()
        states.u.record(0, 1)
        commands = {"rpm": 5 * 2 * np.pi, "P/D": 0.5}
        wrench = model.get_force(states, self.rng.uniform(), env, commands)
        self.assertAlmostEqual(
            -1024 * 25 * 32 * 0.015890316523410611543, wrench.K(), delta=EPS
        )

    def test_torque_should_have_sign_corresponding_to_rotation(self):
        data = WageningenControlledForceModel.parse(wageningen())
        states = BodyStates()
        states.u.record(0, self.rng.uniform(low=1.0, high=1e6))
        env = get_env()
        env.rho = self.rng.uniform(low=1.0, high=1e6)

        w_clockwise = WageningenControlledForceModel(data, "", env)
        data.rotating_clockwise = False
        w_anti_clockwise = WageningenControlledForceModel(data, "", env)
        commands = {
            "rpm": self.rng.uniform(low=states.u(), high=2 * states.u()),
            "P/D": self.rng.uniform(low=0.5, high=1.4),
        }
        self.assertGreater(
            0, w_clockwise.get_force(states, self.rng.uniform(), env, commands).K()
        )
        self.assertLess(
            0, w_anti_clockwise.get_force(states, self.rng.uniform(), env, commands).K()
        )

    def test_bug_2825_can_use_propeller_with_rpm_zero(self):
        data = WageningenControlledForceModel.parse(wageningen())
        data.blade_area_ratio = 0.4
        env = get_env()
        model = WageningenControlledForceModel(data, "", env)
        states = BodyStates()
        states.u.record(0, 1)
        commands = {"rpm": 0, "P/D": 0.5}
        buf = io.StringIO()
        with redirect_stderr(buf):
            wrench = model.get_force(states, self.rng.uniform(), env, commands)
        expected_regex = "Warning: Wageningen model used outside of its domain. Maybe n is too small\? Invalid advance ratio J: expected 0 <= J <= 1.5 but got J=inf. Saturating at 1.5 to continue simulation."
        self.assertTrue(re.search(expected_regex, buf.getvalue()), buf.getvalue())
        self.assertAlmostEqual(0, wrench.X(), delta=EPS)
        self.assertAlmostEqual(0, wrench.Y(), delta=EPS)
        self.assertAlmostEqual(0, wrench.Z(), delta=EPS)
        self.assertAlmostEqual(0, wrench.K(), delta=EPS)
        self.assertAlmostEqual(0, wrench.M(), delta=EPS)
        self.assertAlmostEqual(0, wrench.N(), delta=EPS)

    def test_bug_2825_can_use_propeller_with_rpm_near_zero(self):
        data = WageningenControlledForceModel.parse(wageningen())
        data.blade_area_ratio = 0.4
        env = get_env()
        model = WageningenControlledForceModel(data, "", env)
        states = BodyStates()
        states.u.record(0, 1)
        commands = {"rpm": 1e-16, "P/D": 0.5}
        buf = io.StringIO()
        with redirect_stderr(buf):
            wrench = model.get_force(states, self.rng.uniform(), env, commands)
        expected_regex = "Warning: Wageningen model used outside of its domain. Maybe n is too small\? Invalid advance ratio J: expected 0 <= J <= 1.5 but got J=.*. Saturating at 1.5 to continue simulation."
        self.assertTrue(re.search(expected_regex, buf.getvalue()), buf.getvalue())

        self.assertAlmostEqual(0, wrench.X(), delta=EPS)
        self.assertAlmostEqual(0, wrench.Y(), delta=EPS)
        self.assertAlmostEqual(0, wrench.Z(), delta=EPS)
        self.assertAlmostEqual(0, wrench.K(), delta=EPS)
        self.assertAlmostEqual(0, wrench.M(), delta=EPS)
        self.assertAlmostEqual(0, wrench.N(), delta=EPS)


if __name__ == "__main__":

    unittest.main()
