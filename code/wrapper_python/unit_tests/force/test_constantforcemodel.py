"""
Unit test for ConstantForceModel
"""
import unittest
from math import cos, sin

import numpy as np
from xdyn.core import BodyBuilder, BodyPtr, BodyStates, EnvironmentAndFrames
from xdyn.core.io import YamlRotation
from xdyn.data.mesh import two_triangles
from xdyn.force import ConstantForceModel
from xdyn.ssc.kinematics import EulerAngles as SscEulerAngles
from xdyn.ssc.random import DataGenerator


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
    env: EnvironmentAndFrames, body_name: str, phi: float, theta: float, psi: float
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


ctm_x = lambda alpha: np.array(
    [[1.0, 0.0, 0.0], [0.0, cos(alpha), sin(alpha)], [0.0, -sin(alpha), cos(alpha)]]
)
ctm_y = lambda alpha: np.array(
    [[cos(alpha), 0.0, -sin(alpha)], [0.0, 1.0, 0.0], [sin(alpha), 0.0, cos(alpha)]]
)
ctm_z = lambda alpha: np.array(
    [[cos(alpha), sin(alpha), 0.0], [-sin(alpha), cos(alpha), 0.0], [0.0, 0.0, 1.0]]
)


class ConstantForceModelTest(unittest.TestCase):
    """Test class for ConstantForceModel"""

    def setUp(self) -> None:
        self.rng = DataGenerator(666)

    def random_phi(self) -> float:
        return self.rng.random_double().between(-np.pi / 2, np.pi / 2)()

    def random_theta(self) -> None:
        return self.rng.random_double().between(-np.pi / 2 + 0.1, +np.pi / 2 - 0.1)()

    def random_psi(self) -> float:
        return self.rng.random_double().between(-np.pi, np.pi)()

    def test_can_parse_reference_frame(self):
        """Check that parse function produces a valid ConstantForceModelTest object"""
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

    def test_ship_at_0_deg(self):
        data = ConstantForceModel.parse(get_input())
        env = get_env()
        body_name = "Anthineas"
        states = get_states(env, body_name, 0, 0, 0)
        force_model = ConstantForceModel(data, body_name, env)
        wrench = force_model(states, 0.0, env)
        self.assertAlmostEqual(10e3, wrench.X())
        self.assertAlmostEqual(20e3, wrench.Y())
        self.assertAlmostEqual(30e3, wrench.Z())
        pos_x_force = 0.5 - 0.1
        pos_y_force = -0.2 - 2.04
        pos_z_force = -440.0 - 6.28
        pos_force = [pos_x_force, pos_y_force, pos_z_force]
        offset = np.cross(pos_force, [10e3, 20e3, 30e3])
        self.assertAlmostEqual(100e3 + offset[0], wrench.K())
        self.assertAlmostEqual(200e3 + offset[1], wrench.M())
        self.assertAlmostEqual(300e3 + offset[2], wrench.N())

    def test_ship_at_45_deg(self):
        data = ConstantForceModel.parse(get_input())
        env = get_env()
        phi = 0
        theta = 0
        psi = 45 * np.pi / 180.0
        body_name = "Anthineas"
        states = get_states(env, body_name, phi, theta, psi)
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
        states = get_states(env, body_name, phi, theta, psi)
        force_model = ConstantForceModel(data, body_name, env)
        wrench = force_model(states, 0.0, env)
        self.assertAlmostEqual(
            10e3 * np.sqrt(3) / 2 + 20e3 * np.sqrt(1) / 2, wrench.X()
        )
        self.assertAlmostEqual(
            -10e3 * np.sqrt(1) / 2 + 20e3 * np.sqrt(3) / 2, wrench.Y()
        )
        self.assertAlmostEqual(30e3, wrench.Z())

    def wrench_checks(self, W, frame_name: str, force: np.ndarray, torque: np.ndarray):
        # function to check output from ConstantForceModel
        eps = 1e-7
        self.assertAlmostEqual(force[0], W.X(), delta=eps)
        self.assertAlmostEqual(force[1], W.Y(), delta=eps)
        self.assertAlmostEqual(force[2], W.Z(), delta=eps)
        self.assertAlmostEqual(torque[0], W.K(), delta=eps)
        self.assertAlmostEqual(torque[1], W.M(), delta=eps)
        self.assertAlmostEqual(torque[2], W.N(), delta=eps)
        self.assertEqual(frame_name, W.get_frame())
        self.assertEqual(frame_name, W.get_point().get_frame())
        self.assertEqual(0.0, W.get_point().x())
        self.assertEqual(0.0, W.get_point().y())
        self.assertEqual(0.0, W.get_point().z())

    def test_should_be_able_to_call_get_force(self):
        # Whatever the ship state is, calling `get_force` should return the same wrench in `constant frame`
        data = ConstantForceModel.parse(get_input())
        env = get_env()
        body_name = "Anthineas"
        force = np.array([10e3, 20e3, 30e3])
        torque = np.array([100e3, 200e3, 300e3])
        for _ in range(100):
            phi = self.random_phi()
            theta = self.random_theta()
            psi = self.random_psi()
            states = get_states(env, body_name, phi, theta, psi)
            force_model = ConstantForceModel(data, body_name, env)
            wrench = force_model.get_force(states, 0.0, env)
            self.wrench_checks(wrench, "constant force", force, torque)

    def test_should_evaluate_correctly_wrench_in_ship_frame_with_psi_at_0_deg(self):
        data = ConstantForceModel.parse(get_input())
        env = get_env()
        body_name = "Anthineas"
        states = get_states(env, body_name, 0.0, 0.0, 0.0)
        force_model = ConstantForceModel(data, body_name, env)
        wrench = force_model(states, 0.0, env)
        pos_force = np.array([+0.5 - 0.1, -0.2 - 2.04, -440.0 - 6.28])
        force = np.array([10e3, 20e3, 30e3])
        torque = np.array([100e3, 200e3, 300e3])
        torque_ship_ned = torque + np.cross(pos_force, force)
        self.wrench_checks(wrench, body_name, force, torque_ship_ned)

    def test_should_evaluate_correctly_wrench_in_ship_frame_with_random_psi(self):
        data = ConstantForceModel.parse(get_input())
        env = get_env()
        body_name = "Anthineas"
        force_model = ConstantForceModel(data, body_name, env)
        pos_force = np.array([+0.5 - 0.1, -0.2 - 2.04, -440.0 - 6.28])
        force = np.array([10e3, 20e3, 30e3])
        torque = np.array([100e3, 200e3, 300e3])
        torque_ship_ned = torque + np.cross(pos_force, force)
        for _ in range(100):
            psi = self.random_psi()
            states = get_states(env, body_name, 0.0, 0.0, psi)
            wrench = force_model(states, 0.0, env)
            ctm = ctm_z(psi)
            self.wrench_checks(wrench, body_name, ctm @ force, ctm @ torque_ship_ned)

    def test_should_evaluate_correctly_wrench_in_ship_frame_with_random_phi_theta_psi(
        self,
    ):
        data = ConstantForceModel.parse(get_input())
        env = get_env()
        body_name = "Anthineas"
        force_model = ConstantForceModel(data, body_name, env)
        pos_force = np.array([+0.5 - 0.1, -0.2 - 2.04, -440.0 - 6.28])
        force = np.array([10e3, 20e3, 30e3])
        torque = np.array([100e3, 200e3, 300e3])
        torque_ship_ned = torque + np.cross(pos_force, force)
        for _ in range(100):
            phi = self.random_phi()
            theta = self.random_theta()
            psi = self.random_psi()
            states = get_states(env, body_name, phi, theta, psi)
            wrench = force_model(states, 0.0, env)
            ctm = ctm_x(phi) @ ctm_y(theta) @ ctm_z(psi)
            self.wrench_checks(wrench, body_name, ctm @ force, ctm @ torque_ship_ned)


if __name__ == "__main__":

    unittest.main()
