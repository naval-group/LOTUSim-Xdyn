"""
Unit test for ConstantForceModel
"""
import unittest

import numpy as np

from xdyn import (
    BodyBuilder,
    BodyPtr,
    BodyStates,
    ConstantForceModel,
    EnvironmentAndFrames,
    YamlRotation,
)
from xdyn.data import two_triangles
from xdyn.ssc.kinematics import EulerAngles as SscEulerAngles


def get_input() -> str:
    """Create a YAML data model string"""
    return """
    model: constant force
    frame: NED
    x: {value: 0.5, unit: m}
    y: {value: -0.2, unit: m}
    z: {value: -440, unit: m}
    X: {value: 10, unit: kN}
    Y: {value: 20, unit: kN}
    Z: {value: 30, unit: kN}
    K: {value: 100, unit: kN*m}
    M: {value: 200, unit: kN*m}
    N: {value: 300, unit: kN*m}
    """


def get_env() -> EnvironmentAndFrames:
    env = EnvironmentAndFrames()
    env.rot = YamlRotation("angle", ["z", "y'", "x''"])
    env.rho = 1024
    return env


def get_body(name: str) -> BodyPtr:
    rot = YamlRotation("angle", ["z", "y'", "x''"])
    builder = BodyBuilder(rot)
    return builder.build_for_test(name, two_triangles(), 0, 0.0, rot, 0.0, False)


def get_states(
    body_name: str, phi: float, theta: float, psi: float, env: EnvironmentAndFrames
) -> BodyStates:
    """Create a body state variable with one record"""
    body = get_body(body_name)
    states = body.get_states()
    states.convention = YamlRotation("angle", ["z", "y'", "x''"])
    quaternion = states.convert_to_quaternion(
        SscEulerAngles(phi, theta, psi), states.convention
    )
    states.x.record(0, 0.1)
    states.y.record(0, 2.04)
    states.z.record(0, 6.28)
    states.u.record(0, 0.45)
    states.v.record(0, 0.01)
    states.w.record(0, 5.869)
    states.p.record(0, 0.23)
    states.q.record(0, 0)
    states.r.record(0, 0.38)
    states.qr.record(0, quaternion[0])  # To improve
    states.qi.record(0, quaternion[1])
    states.qj.record(0, quaternion[2])
    states.qk.record(0, quaternion[3])
    body.update_kinematics(states.get_current_state_values(0), env.k)
    return states


class ConstantForceModelTest(unittest.TestCase):
    """Test class for ConstantForceModel"""

    def test_can_parse_reference_frame(self):
        """Check that parse function produces a valid GravityForceModelTest object"""
        data = ConstantForceModel.parse(get_input())
        self.assertEqual("NED", data.frame)

    def test_can_parse_x(self):
        data = ConstantForceModel.parse(get_input())
        self.assertEqual(0.5, data.x)

    def test_can_parse_y(self):
        data = ConstantForceModel.parse(get_input())
        self.assertEqual(-0.2, data.y)

    def test_can_parse_z(self):
        data = ConstantForceModel.parse(get_input())
        self.assertEqual(-440.0, data.z)

    def test_can_parse_X(self):
        data = ConstantForceModel.parse(get_input())
        self.assertEqual(+10e3, data.X)

    def test_can_parse_Y(self):
        data = ConstantForceModel.parse(get_input())
        self.assertEqual(+20e3, data.Y)

    def test_can_parse_Z(self):
        data = ConstantForceModel.parse(get_input())
        self.assertEqual(+30e3, data.Z)

    def test_can_parse_K(self):
        data = ConstantForceModel.parse(get_input())
        self.assertEqual(+100e3, data.K)

    def test_can_parse_M(self):
        data = ConstantForceModel.parse(get_input())
        self.assertEqual(+200e3, data.M)

    def test_can_parse_N(self):
        data = ConstantForceModel.parse(get_input())
        self.assertEqual(+300e3, data.N)

    def test_ship_at_45_deg(self):
        data = ConstantForceModel.parse(get_input())
        env = get_env()
        phi = 0
        theta = 0
        psi = 45 * np.pi / 180.0
        body_name = "Anthineas"
        states = get_states(body_name, phi, theta, psi, env)
        force_model = ConstantForceModel(data, body_name, env)
        wrench = force_model(states, 0.0, env)
        self.assertAlmostEqual(
            10e3 * np.sqrt(2) / 2 + 20e3 * np.sqrt(2) / 2, wrench.X()
        )
        self.assertAlmostEqual(
            -10e3 * np.sqrt(2) / 2 + 20e3 * np.sqrt(2) / 2, wrench.Y()
        )
        self.assertAlmostEqual(30e3, wrench.Z())

    def test_ship_at_30_deg(self):
        data = ConstantForceModel.parse(get_input())
        env = get_env()
        phi = 0
        theta = 0
        psi = 30 * np.pi / 180.0
        body_name = "Anthineas"
        states = get_states(body_name, phi, theta, psi, env)
        force_model = ConstantForceModel(data, body_name, env)
        wrench = force_model(states, 0.0, env)
        self.assertAlmostEqual(
            10e3 * np.sqrt(3) / 2 + 20e3 * np.sqrt(1) / 2, wrench.X()
        )
        self.assertAlmostEqual(
            -10e3 * np.sqrt(1) / 2 + 20e3 * np.sqrt(3) / 2, wrench.Y()
        )
        self.assertAlmostEqual(30e3, wrench.Z())


if __name__ == "__main__":

    unittest.main()
