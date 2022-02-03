"""
Unit test for AeroPolarForceModel
"""
import io
import re
import unittest
from contextlib import redirect_stderr
from typing import Optional

import numpy as np
from xdyn.core import BodyStates, EnvironmentAndFrames
from xdyn.core.io import YamlCoordinates, YamlRotation
from xdyn.env.wind import (UniformWindVelocityProfile,
                           WindMeanVelocityProfileInput)
from xdyn.exceptions import InvalidInputException
from xdyn.force import AeroPolarForceModel, AeroPolarForceModelInput


def get_data() -> str:
    """Create a YAML data model string"""
    return """
    name: test
    calculation point in body frame:
        x: {value: 1, unit: m}
        y: {value: 2, unit: m}
        z: {value: 3, unit: m}
    reference area: {value: 1000, unit: m^2}
    AWA: {unit: deg, values: [0,7,9,12,28,60,90,120,150,180]}
    lift coefficient: [0.00000,0.94828,1.13793,1.25000,1.42681,1.38319,1.26724,0.93103,0.38793,-0.11207]
    drag coefficient: [0.03448,0.01724,0.01466,0.01466,0.02586,0.11302,0.38250,0.96888,1.31578,1.34483]
    angle command: beta
    """


def get_states(u: Optional[float] = 0.0, v: Optional[float] = 0.0) -> BodyStates:
    """Create a body state variable with one record"""
    states = BodyStates(0)
    states.convention = YamlRotation("angle", ["z", "y'", "x''"])
    states.x.record(0, 0)
    states.y.record(0, 0)
    states.z.record(0, 0)
    states.u.record(0, u)
    states.v.record(0, v)
    states.w.record(0, 0)
    states.p.record(0, 0)
    states.q.record(0, 0)
    states.r.record(0, 0)
    states.qr.record(0, 1)
    states.qi.record(0, 0)
    states.qj.record(0, 0)
    states.qk.record(0, 0)
    return states


class AeroPolarForceModelTest(unittest.TestCase):
    """Test class for AeroPolarForceModel"""

    def test_can_parse(self):
        data = AeroPolarForceModel.parse(get_data())
        self.assertEqual(type(data), AeroPolarForceModelInput)
        self.assertEqual(data.name, "test")
        self.assertEqual(data.reference_area, 1000)
        self.assertEqual(data.calculation_point_in_body_frame.x, 1)
        self.assertEqual(data.calculation_point_in_body_frame.y, 2)
        self.assertEqual(data.calculation_point_in_body_frame.z, 3)
        AWA = [0,7,9,12,28,60,90,120,150,180]
        Cl = [0.00000,0.94828,1.13793,1.25000,1.42681,1.38319,1.26724,0.93103,0.38793,-0.11207]
        Cd = [0.03448,0.01724,0.01466,0.01466,0.02586,0.11302,0.38250,0.96888,1.31578,1.34483]
        self.assertEqual(len(data.apparent_wind_angle), 10)
        self.assertEqual(len(data.lift_coefficient), 10)
        self.assertEqual(len(data.drag_coefficient), 10)
        assert_equal = lambda x, y: self.assertAlmostEqual(x, y, delta=1e-12)
        for i in range(10):
            assert_equal(data.apparent_wind_angle[i], np.pi * AWA[i] / 180)
            assert_equal(data.lift_coefficient[i], Cl[i])
            assert_equal(data.drag_coefficient[i], Cd[i])
        self.assertEqual(data.angle_command, "beta")

    def test_several_values(self):
        data = AeroPolarForceModelInput()
        data.name = "test"
        data.calculation_point_in_body_frame = YamlCoordinates(0,0,0)
        data.reference_area = 1000
        data.apparent_wind_angle = [0.,0.12217305,0.15707963,0.20943951,0.48869219,1.04719755,1.57079633,2.0943951,2.61799388,np.pi]
        data.lift_coefficient = [0.00000,0.94828,1.13793,1.25000,1.42681,1.38319,1.26724,0.93103,0.38793,-0.11207]
        data.drag_coefficient = [0.03448,0.01724,0.01466,0.01466,0.02586,0.11302,0.38250,0.96888,1.31578,1.34483]
        env = EnvironmentAndFrames()
        force_model = AeroPolarForceModel(data, "body", env)
        states = get_states()
        env.set_rho_air(1.2)
        wind_data = WindMeanVelocityProfileInput()
        wind_data.velocity = 10

        wind_data.direction = 0 * np.pi / 180
        env.set_wind_model(wind_data)
        eps = 1E-10
        assert_equal = lambda x, y: self.assertAlmostEqual(x, y, delta=eps)
        wrench = force_model.get_force(states, 0, env)
        assert_equal(80689.800000000003, wrench.X())
        assert_equal(+6724.2000000000098, force_model.get_force(states, 0, env).Y())
        wind_data.direction = 45*np.pi / 180
        env.set_wind_model(wind_data)
        wrench = force_model.get_force(states, 0, env)
        assert_equal(78937.416185313908, wrench.X())
        assert_equal(21889.902640217559, wrench.Y())
        wind_data.direction = 90*np.pi / 180
        env.set_wind_model(wind_data)
        wrench = force_model.get_force(states, 0, env)
        assert_equal(76034.400072671793, wrench.X())
        assert_equal(22949.999826340063, wrench.Y())
        wind_data.direction = 135*np.pi / 180
        env.set_wind_model(wind_data)
        wrench = force_model.get_force(states, 0, env)
        assert_equal(58259.36619649193, wrench.X())
        assert_equal(63226.236365071731, wrench.Y())
        wind_data.direction = 180*np.pi / 180
        env.set_wind_model(wind_data)
        wrench = force_model.get_force(states, 0, env)
        assert_equal(-2068.8000000000002, wrench.X())
        assert_equal(0, force_model.get_force(states, 0, env).Y())
        wind_data.direction = 225*np.pi / 180
        env.set_wind_model(wind_data)
        wrench = force_model.get_force(states, 0, env)
        assert_equal(58259.366196491937, wrench.X())
        assert_equal(-63226.236365071782, wrench.Y())
        wind_data.direction = 270*np.pi / 180
        env.set_wind_model(wind_data)
        wrench = force_model.get_force(states, 0, env)
        assert_equal(76034.4000726718080, wrench.X())
        assert_equal(-22949.999826340088, wrench.Y())
        wind_data.direction = 315*np.pi / 180
        env.set_wind_model(wind_data)
        wrench = force_model.get_force(states, 0, env)
        assert_equal(78937.416185313894, wrench.X())
        assert_equal(-21889.902640217573, wrench.Y())
        wind_data.direction = 355*np.pi / 180
        env.set_wind_model(wind_data)
        wrench = force_model.get_force(states, 0, env)
        assert_equal(80930.439263099062, wrench.X())
        assert_equal(-9736.6379043679153, wrench.Y())

    def test_orientation_test_no_forward_speed(self):
        data = AeroPolarForceModelInput()
        data.name = "test"
        data.calculation_point_in_body_frame = YamlCoordinates(0, 0, 0)
        data.reference_area = 1000
        data.apparent_wind_angle = [0.,0.12217305,0.15707963,0.20943951,0.48869219,1.04719755,1.57079633,2.0943951,2.61799388,np.pi]
        data.lift_coefficient = [0.00000,0.94828,1.13793,1.25000,1.42681,1.38319,1.26724,0.93103,0.38793,-0.11207]
        data.drag_coefficient = [0.03448,0.01724,0.01466,0.01466,0.02586,0.11302,0.38250,0.96888,1.31578,1.34483]
        env = EnvironmentAndFrames()
        force_model = AeroPolarForceModel(data, "body", env)
        states = get_states()
        env.set_rho_air(1.2)
        wind_data = WindMeanVelocityProfileInput()
        wind_data.velocity = 10
        # North wind (270°), heading North, Vs=0 --> AWA = 0°
        wind_data.direction = np.pi * 180 / 180
        env.set_wind_model(wind_data)
        F = force_model.get_force(states, 0, env)
        self.assertLess(F.X(), 0)
        self.assertAlmostEqual(F.Y(), 0, delta=1e-10)
        # South wind (270°), heading North, Vs=0 --> 180 = 180°
        wind_data.direction = 0
        env.set_wind_model(wind_data)
        F = force_model.get_force(states, 0, env)
        self.assertGreater(F.X(), 0)
        # East wind (270°), heading North, Vs=0 --> AWA = 90°
        wind_data.direction = np.pi * 270 / 180
        env.set_wind_model(wind_data)
        F = force_model.get_force(states, 0, env)
        self.assertGreater(F.X(), 0)
        self.assertLess(F.Y(), 0)
        # West wind (90°), heading North, Vs=0 --> AWA = 270°
        wind_data.direction = np.pi * 90 / 180
        env.set_wind_model(wind_data)
        F = force_model.get_force(states, 0, env)
        self.assertGreater(F.X(), 0)
        self.assertGreater(F.Y(), 0)


    def test_orientation_test_with_forward_speed(self):
        data = AeroPolarForceModelInput()
        data.name = "test"
        data.calculation_point_in_body_frame = YamlCoordinates(0,0,0)
        data.reference_area = 1000
        data.apparent_wind_angle = [0.,0.12217305,0.15707963,0.20943951,0.48869219,1.04719755,1.57079633,2.0943951,2.61799388,np.pi]
        data.lift_coefficient = [0.00000,0.94828,1.13793,1.25000,1.42681,1.38319,1.26724,0.93103,0.38793,-0.11207]
        data.drag_coefficient = [0.03448,0.01724,0.01466,0.01466,0.02586,0.11302,0.38250,0.96888,1.31578,1.34483]
        env = EnvironmentAndFrames()
        force_model = AeroPolarForceModel(data, "body", env)
        states = get_states()
        env.set_rho_air(1.2)
        wind_data = WindMeanVelocityProfileInput()
        wind_data.velocity = 10
        # North wind (270°), heading North, Vs=10m/s --> AWA = 0°
        wind_data.direction = np.pi * 180 / 180
        env.set_wind_model(wind_data)
        F = force_model.get_force(states, 0, env)
        self.assertLess(F.X(), 0)
        self.assertAlmostEqual(F.Y(), 0, delta=1e-10)
        # South wind (270°), heading North, Vs=10m/s --> 180 = 180°
        wind_data.direction = 0
        env.set_wind_model(wind_data)
        F = force_model.get_force(states, 0, env)
        self.assertGreater(F.X(), 0)
        # North-East wind (225°), heading North, Vs=10m/s --> AWA = 90°
        wind_data.direction = np.pi * 225 / 180
        env.set_wind_model(wind_data)
        F = force_model.get_force(states, 0, env)
        self.assertGreater(F.X(), 0)
        self.assertLess(F.Y(), 0)
        # North-West wind (135°), heading North, Vs=10m/s --> AWA = 270°
        wind_data.direction = np.pi * 135 / 180
        env.set_wind_model(wind_data)
        F = force_model.get_force(states, 0, env)
        self.assertGreater(F.X(), 0)
        self.assertGreater(F.Y(), 0)


    def test_should_throw_for_invalid_polar_data(self):
        data = AeroPolarForceModelInput()
        data.name = "test"
        data.calculation_point_in_body_frame = YamlCoordinates(0,0,0)
        data.reference_area = 1000
        env = EnvironmentAndFrames()

        # The vectors do not have the same size
        data.apparent_wind_angle = [0.,0.12217305,0.15707963,0.20943951,0.48869219,1.04719755,1.57079633,2.0943951,2.61799388,np.pi]
        data.lift_coefficient = [0.00000,0.94828,1.13793,1.25000,1.42681,1.38319,1.26724,0.93103,0.38793]
        data.drag_coefficient = [0.03448,0.01724,0.01466,0.01466,0.02586,0.11302,0.38250,0.96888]
        expected_msg = "Apparent wind angle, lift coefficient and drag coefficient must all have the same size."
        with self.assertRaises(InvalidInputException) as pcm:
            AeroPolarForceModel(data, "body", env)
        self.assertTrue(expected_msg in str(pcm.exception), str(pcm.exception))

        # The maximum value for AWA is below 180°
        data.apparent_wind_angle = [0.,0.12217305,0.15707963,0.20943951,0.48869219,1.04719755,1.57079633,2.0943951,2.61799388]
        data.lift_coefficient = [0.00000,0.94828,1.13793,1.25000,1.42681,1.38319,1.26724,0.93103,0.38793]
        data.drag_coefficient = [0.03448,0.01724,0.01466,0.01466,0.02586,0.11302,0.38250,0.96888,1.31578]
        expected_msg = "Apparent wind angle must be provided from 0deg to either 180deg (symmetry) or 360deg."
        with self.assertRaises(InvalidInputException) as pcm:
            AeroPolarForceModel(data, "body", env)
        self.assertTrue(expected_msg in str(pcm.exception), str(pcm.exception))


    def test_should_print_warning_for_polar_data_with_unexpected_bounds(self):
        data = AeroPolarForceModelInput()
        data.name = "test"
        data.calculation_point_in_body_frame = YamlCoordinates(0,0,0)
        data.reference_area = 1000
        env = EnvironmentAndFrames()

        # The maximum value for AWA is between 180° and 360°
        data.apparent_wind_angle = [0.,0.12217305,0.15707963,0.20943951,0.48869219,1.04719755,1.57079633,2.0943951,2.61799388,np.pi,3*np.pi/2]
        data.lift_coefficient = [0.00000,0.94828,1.13793,1.25000,1.42681,1.38319,1.26724,0.93103,0.38793,-0.11207,-0.11207]
        data.drag_coefficient = [0.03448,0.01724,0.01466,0.01466,0.02586,0.11302,0.38250,0.96888,1.31578,1.34483,1.34483]
        buf = io.StringIO()
        with redirect_stderr(buf):
            AeroPolarForceModel(data, "body", env)
        expected_regex = "WARNING: In an aerodynamic polar force model 'test', you provided a maximum apparent wind angle between 180deg and 360deg. Symmetry will be assumed and values over 180deg will be ignored."
        self.assertTrue(re.search(expected_regex, buf.getvalue()), buf.getvalue())

        # The maximum value for AWA is over 360°
        data.apparent_wind_angle = [0.,0.12217305,0.15707963,0.20943951,0.48869219,1.04719755,1.57079633,2.0943951,2.61799388,np.pi,3*np.pi/2,5*np.pi/2]
        data.lift_coefficient = [0.00000,0.94828,1.13793,1.25000,1.42681,1.38319,1.26724,0.93103,0.38793,-0.11207,-0.11207,-0.11207]
        data.drag_coefficient = [0.03448,0.01724,0.01466,0.01466,0.02586,0.11302,0.38250,0.96888,1.31578,1.34483,1.34483,1.34483]
        buf = io.StringIO()
        with redirect_stderr(buf):
            AeroPolarForceModel(data, "body", env)
        expected_regex = "WARNING: In an aerodynamic polar force model 'test', you provided a maximum apparent wind higher than 360deg. All values over 360deg will be ignored."
        self.assertTrue(re.search(expected_regex, buf.getvalue()), buf.getvalue())

    def test_angle_can_be_controlled(self):
        data = AeroPolarForceModelInput()
        data.name = "test"
        data.calculation_point_in_body_frame = YamlCoordinates(0,0,0)
        data.reference_area = 1000
        data.apparent_wind_angle = [0.,0.12217305,0.15707963,0.20943951,0.48869219,1.04719755,1.57079633,2.0943951,2.61799388,np.pi]
        data.lift_coefficient = [0.00000,0.94828,1.13793,1.25000,1.42681,1.38319,1.26724,0.93103,0.38793,-0.11207]
        data.drag_coefficient = [0.03448,0.01724,0.01466,0.01466,0.02586,0.11302,0.38250,0.96888,1.31578,1.34483]
        data.angle_command = "beta"
        env = EnvironmentAndFrames()
        force_model = AeroPolarForceModel(data, "body", env)
        states = get_states()
        env.set_rho_air(1.2)
        wind_data = WindMeanVelocityProfileInput()
        wind_data.velocity = 10
        wind_data.direction = 0
        env.set_wind_model(wind_data)
        eps = 1E-10
        assert_equal = lambda x, y: self.assertAlmostEqual(x, y, delta=eps)
        assert_equal(80689.800000000003, force_model.get_force(states, 0, env, {"beta": 0}).X())
        assert_equal(6724.2000000000098, force_model.get_force(states, 0, env, {"beta": 0}).Y())

        with self.assertRaises(IndexError) as pcm:
            force_model.get_force(states, 0, env)
        expected_msg = "map::at"
        self.assertTrue(expected_msg in str(pcm.exception), str(pcm.exception))

        wind_data.direction = 90.0 * np.pi / 180.0
        env.set_wind_model(wind_data)
        assert_equal(-6724.2000000000098, force_model.get_force(states, 0, env, {"beta": 90 * np.pi / 180.0}).X())
        assert_equal(80689.800000000003, force_model.get_force(states, 0, env, {"beta": 90 * np.pi / 180.0}).Y())

        wind_data.direction = -90 * np.pi / 180.0
        env.set_wind_model(wind_data)
        assert_equal(-6724.2000000000098, force_model.get_force(states, 0, env, {"beta": -90 * np.pi / 180.0}).X())
        assert_equal(-80689.800000000003, force_model.get_force(states, 0, env, {"beta": -90 * np.pi / 180.0}).Y())


if __name__ == "__main__":

    unittest.main()
