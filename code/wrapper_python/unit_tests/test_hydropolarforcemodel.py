"""
Unit test for HydroPolarForceModel
"""
import unittest
from typing import Optional

import numpy as np
from xdyn import (
    BodyStates,
    EnvironmentAndFrames,
    HydroPolarForceModel,
    HydroPolarForceModelInput,
    YamlAngle,
    YamlCoordinates,
    YamlPosition,
    YamlRotation,
    ostream_redirect,
)


def get_input() -> str:
    """Create a YAML data model string"""
    return """
    name: test
    position of calculation frame:
        frame: body
        x: {value: 1, unit: m}
        y: {value: 2, unit: m}
        z: {value: 3, unit: m}
        phi: {value: 10, unit: deg}
        theta: {value: 20, unit: deg}
        psi: {value: 30, unit: deg}
    reference area: {value: 1000, unit: m^2}
    angle of attack: {unit: deg, values: [0,7,9,12,28,60,90,120,150,180]}
    lift coefficient: [0.00000,0.94828,1.13793,1.25000,1.42681,1.38319,1.26724,0.93103,0.38793,-0.11207]
    drag coefficient: [0.03448,0.01724,0.01466,0.01466,0.02586,0.11302,0.38250,0.96888,1.31578,1.34483]
    take waves orbital velocity into account: False
    """


def get_input_full() -> str:
    return (
        get_input()
        + """
    moment coefficient: [0.03448,0.01724,0.01466,0.01466,0.02586,0.11302,0.38250,0.96888,1.31578,1.34483]
    chord length: {value: 5, unit: m}
    """
    )


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


class HydroPolarForceModelTest(unittest.TestCase):
    """Test class for HydroPolarForceModel"""

    def test_can_parse(self):
        """Check that parse function produces a valid HydroPolarForceModel object"""
        data = HydroPolarForceModel.parse(get_input())
        self.assertEqual(data.name, "test")
        self.assertEqual(data.reference_area, 1000.0)
        self.assertEqual(data.internal_frame.coordinates.x, 1)
        self.assertEqual(data.internal_frame.coordinates.y, 2)
        self.assertEqual(data.internal_frame.coordinates.z, 3)
        self.assertEqual(data.internal_frame.angle.phi, 10 * np.pi / 180)
        self.assertEqual(data.internal_frame.angle.theta, 20 * np.pi / 180)
        self.assertEqual(data.internal_frame.angle.psi, 30 * np.pi / 180)
        self.assertTrue(
            np.allclose(
                data.angle_of_attack,
                np.array([0, 7, 9, 12, 28, 60, 90, 120, 150, 180]) * np.pi / 180,
            )
        )
        self.assertTrue(
            np.allclose(
                data.lift_coefficient,
                np.array(
                    [
                        0.00000,
                        0.94828,
                        1.13793,
                        1.25000,
                        1.42681,
                        1.38319,
                        1.26724,
                        0.93103,
                        0.38793,
                        -0.11207,
                    ]
                ),
            )
        )
        self.assertTrue(
            np.allclose(
                data.drag_coefficient,
                np.array(
                    [
                        0.03448,
                        0.01724,
                        0.01466,
                        0.01466,
                        0.02586,
                        0.11302,
                        0.38250,
                        0.96888,
                        1.31578,
                        1.34483,
                    ]
                ),
            )
        )
        self.assertIsNone(data.moment_coefficient)
        self.assertIsNone(data.chord_length)

    def test_can_parse_optional_output(self):
        data = HydroPolarForceModel.parse(get_input_full())
        self.assertIsNotNone(data.moment_coefficient)
        self.assertTrue(
            np.allclose(
                data.moment_coefficient,
                [
                    0.03448,
                    0.01724,
                    0.01466,
                    0.01466,
                    0.02586,
                    0.11302,
                    0.38250,
                    0.96888,
                    1.31578,
                    1.34483,
                ],
            )
        )
        self.assertIsNotNone(data.chord_length)
        self.assertEqual(data.chord_length, 5.0)
        self.assertTrue(
            np.allclose(
                data.moment_coefficient,
                [
                    0.03448,
                    0.01724,
                    0.01466,
                    0.01466,
                    0.02586,
                    0.11302,
                    0.38250,
                    0.96888,
                    1.31578,
                    1.34483,
                ],
            )
        )

    def test_orientation(self):
        data = HydroPolarForceModelInput()
        data.name = "test"
        # Internal frame is placed under water, 5m under the body frame's origin
        data.internal_frame = YamlPosition(
            YamlCoordinates(0, 0, 5), YamlAngle(0, 0, 0), "body"
        )
        data.reference_area = 100
        data.angle_of_attack = [
            0.0,
            0.12217305,
            0.15707963,
            0.20943951,
            0.48869219,
            1.04719755,
            1.57079633,
            2.0943951,
            2.61799388,
            np.pi,
        ]
        data.lift_coefficient = [
            0.00000,
            0.94828,
            1.13793,
            1.25000,
            1.42681,
            1.38319,
            1.26724,
            0.93103,
            0.38793,
            0.0,
        ]
        data.drag_coefficient = [
            0.03448,
            0.01724,
            0.01466,
            0.01466,
            0.02586,
            0.11302,
            0.38250,
            0.96888,
            1.31578,
            1.34483,
        ]
        data.use_waves_velocity = False
        env = EnvironmentAndFrames()
        env.rho = 1000
        env.rot = YamlRotation("angle", ["z", "y'", "x''"])
        force_model = HydroPolarForceModel(data, "body", env)
        states = BodyStates()
        # u=10, v=0 -> AoA=0°
        states = get_states(10, 0)
        wrench = force_model.get_force(states, 0, env, {})
        self.assertLess(wrench.X(), 0.0)
        self.assertAlmostEqual(wrench.Y(), 0.0, delta=1e-9)
        # u=0, v=10 -> AoA=-90°
        states = get_states(0, 10)
        wrench = force_model.get_force(states, 0, env, {})
        self.assertGreater(wrench.X(), 0.0)
        self.assertLess(wrench.Y(), 0.0)
        # u=0, v=-10 -> AoA=90°
        states = get_states(0, -10)
        wrench = force_model.get_force(states, 0, env, {})
        self.assertGreater(wrench.X(), 0.0)
        self.assertGreater(wrench.Y(), 0.0)
        # u=-10, v=0 -> AoA=+/-180°
        states = get_states(-10, 0)
        wrench = force_model.get_force(states, 0, env, {})
        self.assertGreater(wrench.X(), 0)
        self.assertAlmostEqual(wrench.Y(), 0.0, delta=1e-9)

    def test_should_throw_for_invalid_polar_input(self):
        data = HydroPolarForceModelInput()
        data.name = "test"
        data.internal_frame = YamlPosition(
            YamlCoordinates(0, 0, 5), YamlAngle(0, 0, 0), "body"
        )
        data.reference_area = 1000
        data.use_waves_velocity = False
        env = EnvironmentAndFrames()
        env.rho = 1000
        env.rot = YamlRotation("angle", ["z", "y'", "x''"])
        # The vectors do not have the same size
        data.angle_of_attack = [
            0.0,
            0.12217305,
            0.15707963,
            0.20943951,
            0.48869219,
            1.04719755,
            1.57079633,
            2.0943951,
            2.61799388,
            np.pi,
        ]
        data.lift_coefficient = [
            0.00000,
            0.94828,
            1.13793,
            1.25000,
            1.42681,
            1.38319,
            1.26724,
            0.93103,
            0.38793,
        ]
        data.drag_coefficient = [
            0.03448,
            0.01724,
            0.01466,
            0.01466,
            0.02586,
            0.11302,
            0.38250,
            0.96888,
        ]
        with self.assertRaises(RuntimeError):
            HydroPolarForceModel(data, "body", env)
        # The maximum value for AoA is below 180°
        data.angle_of_attack = [
            0.0,
            0.12217305,
            0.15707963,
            0.20943951,
            0.48869219,
            1.04719755,
            1.57079633,
            2.0943951,
            2.61799388,
        ]
        data.lift_coefficient = [
            0.00000,
            0.94828,
            1.13793,
            1.25000,
            1.42681,
            1.38319,
            1.26724,
            0.93103,
            0.38793,
        ]
        data.drag_coefficient = [
            0.03448,
            0.01724,
            0.01466,
            0.01466,
            0.02586,
            0.11302,
            0.38250,
            0.96888,
            1.31578,
        ]
        with self.assertRaises(RuntimeError):
            HydroPolarForceModel(data, "body", env)

    def test_should_print_warning_for_polar_input_with_unexpected_bounds(self):
        data = HydroPolarForceModelInput()
        data.name = "test"
        data.internal_frame = YamlPosition(
            YamlCoordinates(0, 0, 5), YamlAngle(0, 0, 0), "body"
        )
        data.reference_area = 1000
        data.use_waves_velocity = False
        env = EnvironmentAndFrames()
        env.rho = 1000
        env.rot = YamlRotation("angle", ["z", "y'", "x''"])
        #
        #    std::stringstream debug
        #    # Redirect cerr to our stringstream buffer or any other ostream
        #    std::streambuf* orig = std::cerr.rdbuf(debug.rdbuf())
        #    ASSERT_TRUE(debug.str().empty())
        #
        # The maximum value for AoA is over 180°
        data.angle_of_attack = [
            0.0,
            0.12217305,
            0.15707963,
            0.20943951,
            0.48869219,
            1.04719755,
            1.57079633,
            2.0943951,
            2.61799388,
            np.pi,
            3 * np.pi / 2,
        ]
        data.lift_coefficient = [
            0.00000,
            0.94828,
            1.13793,
            1.25000,
            1.42681,
            1.38319,
            1.26724,
            0.93103,
            0.38793,
            -0.11207,
            -0.11207,
        ]
        data.drag_coefficient = [
            0.03448,
            0.01724,
            0.01466,
            0.01466,
            0.02586,
            0.11302,
            0.38250,
            0.96888,
            1.31578,
            1.34483,
            1.34483,
        ]
        with ostream_redirect(stdout=False, stderr=False):
            HydroPolarForceModel(data, "body", env)
        print(ostream_redirect)
        # ASSERT_FALSE(debug.str().empty())

    ##    # Reset the stderr stringstream
    ##    debug.str("")
    ##    debug.clear()
    ##    ASSERT_TRUE(debug.str().empty())
    ##
    #        # The minimum value for AoA is between -180° and 0°
    #        data.angle_of_attack = [-np.pi/2,0.,0.12217305,0.15707963,0.20943951,0.48869219,1.04719755,1.57079633,2.0943951,2.61799388, np.pi]
    #        data.lift_coefficient = [0.00000,0.00000,0.94828,1.13793,1.25000,1.42681,1.38319,1.26724,0.93103,0.38793,-0.11207]
    #        data.drag_coefficient = [0.03448,0.03448,0.01724,0.01466,0.01466,0.02586,0.11302,0.38250,0.96888,1.31578,1.34483]
    #        HydroPolarForceModel(data, "body", env)
    #        # ASSERT_FALSE(debug.str().empty())
    ##
    ##    # Reset the stderr stringstream
    ##    debug.str("")
    ##    debug.clear()
    ##    ASSERT_TRUE(debug.str().empty())
    ##
    #        # The minimum value for AWA is under -180°
    #        data.angle_of_attack = [-3*np.pi/2,-np.pi/2,0.,0.12217305,0.15707963,0.20943951,0.48869219,1.04719755,1.57079633,2.0943951,2.61799388, np.pi]
    #        data.lift_coefficient = [0.00000,0.00000,0.00000,0.94828,1.13793,1.25000,1.42681,1.38319,1.26724,0.93103,0.38793,-0.11207]
    #        data.drag_coefficient = [0.03448,0.03448,0.03448,0.01724,0.01466,0.01466,0.02586,0.11302,0.38250,0.96888,1.31578,1.34483]
    #        HydroPolarForceModel(data, "body", env)
    ##    ASSERT_FALSE(debug.str().empty())
    ##
    ##    # Restore cerr's buffer
    ##    std::cerr.rdbuf(orig)

    def test_should_throw_if_empty_angle_vector(self):
        data = HydroPolarForceModelInput()
        data.name = "test"
        # Internal frame is placed under water, 5m under the body frame's origin
        data.internal_frame = YamlPosition(
            YamlCoordinates(0, 0, 5), YamlAngle(0, 0, 0), "body"
        )
        data.reference_area = 100
        data.angle_of_attack = []
        data.lift_coefficient = []
        data.drag_coefficient = []
        data.use_waves_velocity = True
        env = EnvironmentAndFrames()
        env.rho = 1000
        env.rot = YamlRotation("angle", ["z", "y'", "x''"])
        with self.assertRaises(RuntimeError):  # InvalidInputException
            HydroPolarForceModel(data, "body", env)

    def test_should_throw_if_wave_model_expected_but_not_defined(self):
        data = HydroPolarForceModelInput()
        data.name = "test"
        # Internal frame is placed under water, 5m under the body frame's origin
        data.internal_frame = YamlPosition(
            YamlCoordinates(0, 0, 5), YamlAngle(0, 0, 0), "body"
        )
        data.reference_area = 100
        data.angle_of_attack = [
            0.0,
            0.12217305,
            0.15707963,
            0.20943951,
            0.48869219,
            1.04719755,
            1.57079633,
            2.0943951,
            2.61799388,
            np.pi,
        ]
        data.lift_coefficient = [
            0.00000,
            0.94828,
            1.13793,
            1.25000,
            1.42681,
            1.38319,
            1.26724,
            0.93103,
            0.38793,
            0.0,
        ]
        data.drag_coefficient = [
            0.03448,
            0.01724,
            0.01466,
            0.01466,
            0.02586,
            0.11302,
            0.38250,
            0.96888,
            1.31578,
            1.34483,
        ]
        data.use_waves_velocity = True
        env = EnvironmentAndFrames()
        env.rho = 1000
        env.rot = YamlRotation("angle", ["z", "y'", "x''"])
        with self.assertRaises(RuntimeError):  # InvalidInputException
            HydroPolarForceModel(data, "body", env)

    def test_should_print_warning_and_return_zero_force_if_calculation_point_is_outside_the_water(
        self,
    ):
        data = HydroPolarForceModelInput()
        data.name = "test"
        # Internal frame is placed under water, 5m above the body frame's origin (in the air)
        data.internal_frame = YamlPosition(
            YamlCoordinates(0, 0, -5), YamlAngle(0, 0, 0), "body"
        )
        data.reference_area = 100
        data.angle_of_attack = [
            0.0,
            0.12217305,
            0.15707963,
            0.20943951,
            0.48869219,
            1.04719755,
            1.57079633,
            2.0943951,
            2.61799388,
            np.pi,
        ]
        data.lift_coefficient = [
            0.00000,
            0.94828,
            1.13793,
            1.25000,
            1.42681,
            1.38319,
            1.26724,
            0.93103,
            0.38793,
            0.0,
        ]
        data.drag_coefficient = [
            0.03448,
            0.01724,
            0.01466,
            0.01466,
            0.02586,
            0.11302,
            0.38250,
            0.96888,
            1.31578,
            1.34483,
        ]
        data.use_waves_velocity = False
        env = EnvironmentAndFrames()
        env.rho = 1000
        env.rot = YamlRotation("angle", ["z", "y'", "x''"])
        force_model = HydroPolarForceModel(data, "body", env)
        states = get_states(10, 0)
        #
        #    std::stringstream debug
        #    # Redirect cerr to our stringstream buffer or any other ostream
        #    std::streambuf* orig = std::cerr.rdbuf(debug.rdbuf())
        #    ASSERT_TRUE(debug.str().empty())
        #
        wrench = force_model.get_force(states, 0, env, {})
        # ASSERT_FALSE(debug.str().empty())
        self.assertAlmostEqual(wrench.X(), 0.0, delta=1e-12)
        self.assertAlmostEqual(wrench.Y(), 0.0, delta=1e-12)
        self.assertAlmostEqual(wrench.Z(), 0.0, delta=1e-12)
        self.assertAlmostEqual(wrench.K(), 0.0, delta=1e-12)
        self.assertAlmostEqual(wrench.M(), 0.0, delta=1e-12)
        self.assertAlmostEqual(wrench.N(), 0.0, delta=1e-12)


#
#    # Restore cerr's buffer
#    std::cerr.rdbuf(orig)


if __name__ == "__main__":

    unittest.main()
