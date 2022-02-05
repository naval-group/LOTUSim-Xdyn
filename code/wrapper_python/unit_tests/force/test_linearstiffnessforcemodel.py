"""
Unit test for LinearStiffnessForceModel
"""
import unittest

import numpy as np
from xdyn.core import BodyStates, EnvironmentAndFrames
from xdyn.core.io import YamlAngle, YamlCoordinates, YamlPosition, YamlRotation
from xdyn.force import LinearStiffnessForceModel, LinearStiffnessForceModelInput
from xdyn.ssc.kinematics import EulerAngles as SscEulerAngles
from xdyn.ssc.random import DataGenerator


def get_env() -> EnvironmentAndFrames:
    env = EnvironmentAndFrames()
    env.rho = 1024
    env.rot = YamlRotation("angle", ["z", "y'", "x''"])
    return env


def get_position_states(
    x: float, y: float, z: float, phi: float, theta: float, psi: float
) -> BodyStates:
    states = BodyStates()
    states.convention = YamlRotation("angle", ["z", "y'", "x''"])
    quaternion = states.convert_to_quaternion(SscEulerAngles(phi, theta, psi), states.convention)
    states.x.record(0, x)
    states.y.record(0, y)
    states.z.record(0, z)
    states.u.record(0, 0)
    states.v.record(0, 0)
    states.w.record(0, 0)
    states.p.record(0, 0)
    states.q.record(0, 0)
    states.r.record(0, 0)
    states.qr.record(0, quaternion[0])  # To improve
    states.qi.record(0, quaternion[1])
    states.qj.record(0, quaternion[2])
    states.qk.record(0, quaternion[3])
    return states


class LinearStiffnessForceModelTest(unittest.TestCase):
    """Test class for LinearStiffnessForceModel"""

    def setUp(self) -> None:
        self.rng = DataGenerator(666)

    def random_double_between(self, low: float = 0.0, high: float = 1.0) -> float:
        return self.rng.random_double().between(low, high)()

    def test_can_parse(self):
        """Check that parse function produces a valid LinearStiffnessForceModelInput data object"""
        yaml_data = """
        name: test
        stiffness matrix:
            row 1: [  2,   3,   5,   7,  11,  13]
            row 2: [ 17,  19,  23,  29,  31,  37]
            row 3: [ 41,  43,  47,  53,  59,  61]
            row 4: [ 67,  71,  73,  79,  83,  89]
            row 5: [ 97, 101, 103, 107, 109, 113]
            row 6: [127, 131, 137, 139, 149, 151]
        """
        data = LinearStiffnessForceModel.parse(yaml_data)
        self.assertEqual(data.name, "test")
        self.assertEqual(2.0, data.K[0, 0])
        self.assertEqual(3.0, data.K[0, 1])
        self.assertEqual(5.0, data.K[0, 2])
        self.assertEqual(7.0, data.K[0, 3])
        self.assertEqual(11.0, data.K[0, 4])
        self.assertEqual(13.0, data.K[0, 5])
        self.assertEqual(17.0, data.K[1, 0])
        self.assertEqual(19.0, data.K[1, 1])
        self.assertEqual(23.0, data.K[1, 2])
        self.assertEqual(29.0, data.K[1, 3])
        self.assertEqual(31.0, data.K[1, 4])
        self.assertEqual(37.0, data.K[1, 5])
        self.assertEqual(41.0, data.K[2, 0])
        self.assertEqual(43.0, data.K[2, 1])
        self.assertEqual(47.0, data.K[2, 2])
        self.assertEqual(53.0, data.K[2, 3])
        self.assertEqual(59.0, data.K[2, 4])
        self.assertEqual(61.0, data.K[2, 5])
        self.assertEqual(67.0, data.K[3, 0])
        self.assertEqual(71.0, data.K[3, 1])
        self.assertEqual(73.0, data.K[3, 2])
        self.assertEqual(79.0, data.K[3, 3])
        self.assertEqual(83.0, data.K[3, 4])
        self.assertEqual(89.0, data.K[3, 5])
        self.assertEqual(97.0, data.K[4, 0])
        self.assertEqual(101.0, data.K[4, 1])
        self.assertEqual(103.0, data.K[4, 2])
        self.assertEqual(107.0, data.K[4, 3])
        self.assertEqual(109.0, data.K[4, 4])
        self.assertEqual(113.0, data.K[4, 5])
        self.assertEqual(127.0, data.K[5, 0])
        self.assertEqual(131.0, data.K[5, 1])
        self.assertEqual(137.0, data.K[5, 2])
        self.assertEqual(139.0, data.K[5, 3])
        self.assertEqual(149.0, data.K[5, 4])
        self.assertEqual(151.0, data.K[5, 5])

    def test_can_parse_optional(self):
        """Check that parse function produces a valid LinearStiffnessForceModelInput data object"""
        yaml_data = """
        name: test
        stiffness matrix:
            row 1: [  2,   3,   5,   7,  11,  13]
            row 2: [ 17,  19,  23,  29,  31,  37]
            row 3: [ 41,  43,  47,  53,  59,  61]
            row 4: [ 67,  71,  73,  79,  83,  89]
            row 5: [ 97, 101, 103, 107, 109, 113]
            row 6: [127, 131, 137, 139, 149, 151]
        equilibrium position:
            frame: body
            x: {value: 1, unit: m}
            y: {value: 2, unit: m}
            z: {value: 3, unit: m}
            phi: {value: 4, unit: rad}
            theta: {value: 5, unit: rad}
            psi: {value: 6, unit: rad}
        """
        data = LinearStiffnessForceModel.parse(yaml_data)
        self.assertEqual(data.equilibrium_position.frame, "body")
        self.assertEqual(data.equilibrium_position.coordinates.x, 1)
        self.assertEqual(data.equilibrium_position.coordinates.y, 2)
        self.assertEqual(data.equilibrium_position.coordinates.z, 3)
        self.assertEqual(data.equilibrium_position.angle.phi, 4)
        self.assertEqual(data.equilibrium_position.angle.theta, 5)
        self.assertEqual(data.equilibrium_position.angle.psi, 6)

    def test_example(self):
        EPS = 1e-9
        assert_near = lambda x, y: self.assertAlmostEqual(x, y, delta=EPS)
        env = get_env()
        data = LinearStiffnessForceModelInput()
        data.name = "test"
        data.K = np.array(
            [
                [2, 3, 5, 7, 11, 13],
                [17, 19, 23, 29, 31, 37],
                [41, 43, 47, 53, 59, 61],
                [67, 71, 73, 79, 83, 89],
                [97, 101, 103, 107, 109, 113],
                [127, 131, 137, 139, 149, 151],
            ]
        )
        K = data.K
        model = LinearStiffnessForceModel(data, "body", env)
        for _ in range(100):
            x = self.random_double_between(-10.0, +10.0)
            y = self.random_double_between(-10.0, +10.0)
            z = self.random_double_between(-10.0, +10.0)
            phi = self.random_double_between(-np.pi, np.pi)
            theta = self.random_double_between(
                -np.pi / 3, np.pi / 3
            )  # Restricting the range of angles to avoid gimbal lock
            psi = self.random_double_between(-np.pi, np.pi)
            states = get_position_states(x, y, z, phi, theta, psi)
            wrench = model.get_force(states, self.random_double_between(), env)
            self.assertEqual("body", wrench.get_frame())
            for j in range(3):
                assert_near(
                    sum(K[j, c] * dof for c, dof in enumerate((x, y, z, phi, theta, psi))),
                    -wrench.force[j],
                )
                assert_near(
                    sum(K[j + 3, c] * dof for c, dof in enumerate((x, y, z, phi, theta, psi))),
                    -wrench.torque[j],
                )

    def test_example_with_equilibrium_input(self):
        EPS = 1e-9
        assert_near = lambda x, y: self.assertAlmostEqual(x, y, delta=EPS)
        env = get_env()
        data = LinearStiffnessForceModelInput()
        data.name = "test"
        data.K = np.array(
            [
                [2, 3, 5, 7, 11, 13],
                [17, 19, 23, 29, 31, 37],
                [41, 43, 47, 53, 59, 61],
                [67, 71, 73, 79, 83, 89],
                [97, 101, 103, 107, 109, 113],
                [127, 131, 137, 139, 149, 151],
            ]
        )
        x0 = 1
        y0 = 2
        z0 = 3
        phi0 = np.pi / 10.0
        theta0 = np.pi / 8.0
        psi0 = np.pi / 12.0
        data.equilibrium_position = YamlPosition(
            YamlCoordinates(x0, y0, z0), YamlAngle(phi0, theta0, psi0), "body"
        )
        K = data.K
        model = LinearStiffnessForceModel(data, "body", env)
        for _ in range(100):
            x = self.random_double_between(-10.0, +10.0)
            y = self.random_double_between(-10.0, +10.0)
            z = self.random_double_between(-10.0, +10.0)
            phi = self.random_double_between(-np.pi, np.pi)
            theta = self.random_double_between(
                -np.pi / 3, np.pi / 3
            )  # Restricting the range of angles to avoid gimbal lock
            psi = self.random_double_between(-np.pi, np.pi)
            states = get_position_states(x, y, z, phi, theta, psi)
            wrench = model.get_force(states, self.random_double_between(), env)
            self.assertEqual("body", wrench.get_frame())
            for j in range(3):
                k = j + 3
                assert_near(
                    K[j, 0] * (x - x0)
                    + K[j, 1] * (y - y0)
                    + K[j, 2] * (z - z0)
                    + K[j, 3] * (phi - phi0)
                    + K[j, 4] * (theta - theta0)
                    + K[j, 5] * (psi - psi0),
                    -wrench.force[j],
                )
                assert_near(
                    K[k, 0] * (x - x0)
                    + K[k, 1] * (y - y0)
                    + K[k, 2] * (z - z0)
                    + K[k, 3] * (phi - phi0)
                    + K[k, 4] * (theta - theta0)
                    + K[k, 5] * (psi - psi0),
                    -wrench.torque[j],
                )


if __name__ == "__main__":

    unittest.main()
