"""
Unit test for ManeuveringForceModel
"""
import unittest
from math import pi

from xdyn.core import BodyBuilder, BodyPtr, EnvironmentAndFrames
from xdyn.core.io import YamlRotation
from xdyn.data.mesh import two_triangles
from xdyn.data.yaml import man_with_delay, maneuvering, maneuvering_with_same_frame_of_reference
from xdyn.force import ManeuveringForceModel
from xdyn.ssc.datasource import DataSource
from xdyn.ssc.random import DataGenerator


def get_body(name: str) -> BodyPtr:
    rot = YamlRotation("angle", ["z", "y'", "x''"])
    builder = BodyBuilder(rot)
    return builder.build_for_test(name, two_triangles(), 0, 0.0, rot, 0.0, False)


def get_env_with_default_rotation_convention() -> EnvironmentAndFrames:
    env = EnvironmentAndFrames()
    env.rot = YamlRotation("angle", ["z", "y'", "x''"])
    env.rho = 1024
    return env


class ManeuveringForceModelTest(unittest.TestCase):
    def setUp(self) -> None:
        self.rng = DataGenerator(666)

    def random_double(self) -> float:
        return self.rng.random_double()()

    def random_double_between(self, low: float = 0.0, high: float = 1.0) -> float:
        return self.rng.random_double_between(low, high)()

    def test_can_parse_frame_of_reference(self):
        data = ManeuveringForceModel.parse(maneuvering())
        assert_equal = lambda x, y: self.assertAlmostEqual(x, y, delta=1e-12)
        assert_equal(0.696, data.frame_of_reference.coordinates.x)
        assert_equal(0, data.frame_of_reference.coordinates.y)
        assert_equal(1.418, data.frame_of_reference.coordinates.z)
        assert_equal(0.7, data.frame_of_reference.angle.phi)
        assert_equal(-166.0 * pi / 180.0, data.frame_of_reference.angle.theta)
        assert_equal(125.0 * pi / 180.0, data.frame_of_reference.angle.psi)

    def test_can_parse_X_Y_Z_K_M_N(self):
        data = ManeuveringForceModel.parse(maneuvering())
        self.assertEqual("0.5*rho*Vs^2*L^2*X_", data.var2expr["X"])
        self.assertEqual("0.5*rho*Vs^2*L^2*Y_", data.var2expr["Y"])
        self.assertEqual("0", data.var2expr["Z"])
        self.assertEqual("0", data.var2expr["K"])
        self.assertEqual("0", data.var2expr["M"])
        self.assertEqual("0.5*rho*Vs^2*L^3*N_", data.var2expr["N"])

    def test_can_parse_model_name(self):
        data = ManeuveringForceModel.parse(maneuvering())
        self.assertEqual("test", data.name)

    def test_can_parse_commands(self):
        data = ManeuveringForceModel.parse(maneuvering())
        self.assertEqual(data.commands, ["a", "b", "c"])

    def test_can_evaluate_simple_maneuvering_model(self):
        yaml = """
        reference frame:
            frame: some body
            x: {value: 0, unit: m}
            y: {value: 0, unit: m}
            z: {value: 0, unit: m}
            phi: {value: 0, unit: rad}
            theta: {value: 0, unit: deg}
            psi: {value: 0, unit: deg}
        name: something
        X: 2*Y+sqrt(x(t))
        Y: y(t)^2
        Z: 0
        K: 0
        M: 0
        N: 0
        """
        env = get_env_with_default_rotation_convention()
        force = ManeuveringForceModel(ManeuveringForceModel.parse(yaml), "some body", env)
        self.assertEqual("maneuvering", force.model_name())
        states = get_body("some body").get_states()
        t = 10
        states.x.record(t, 1024)
        states.y.record(t, 400)
        command_listener = DataSource()
        F = force(states, t, env, command_listener)

        self.assertEqual("some body", F.get_frame())
        self.assertEqual(320032, F.X())
        self.assertEqual(160000, F.Y())
        self.assertEqual(0, F.Z())
        self.assertAlmostEqual(0, F.K(), delta=1e-15)
        self.assertAlmostEqual(0, F.M(), delta=1e-15)
        self.assertAlmostEqual(0, F.N(), delta=1e-15)

    def test_can_evaluate_full_maneuvering_model_with_same_frame_of_reference(self):
        data = ManeuveringForceModel.parse(maneuvering_with_same_frame_of_reference())
        env = get_env_with_default_rotation_convention()
        t = 0
        states = get_body("TestShip").get_states()
        states.x.record(t, 1)
        states.y.record(t, 2)
        states.z.record(t, 3)
        states.u.record(t, 4)
        states.v.record(t, 5)
        states.w.record(t, 6)
        states.p.record(t, 7)
        states.q.record(t, 8)
        states.r.record(t, 9)

        command_listener = DataSource()
        command_listener.set_float("test(a)", 0.0)
        command_listener.set_float("test(b)", 0.0)
        command_listener.set_float("test(c)", 0.0)

        force = ManeuveringForceModel(data, "TestShip", env)
        F = force(states, t, env, command_listener)

        self.assertEqual("TestShip", F.get_frame())
        self.assertEqual(-93470409.32377005, F.X())
        self.assertEqual(190870415.43062863, F.Y())
        self.assertEqual(0, F.Z())
        self.assertAlmostEqual(0, F.K(), delta=1e-15)
        self.assertAlmostEqual(0, F.M(), delta=1e-15)
        self.assertEqual(253134236.3875341, F.N())

    def test_can_evaluate_full_maneuvering_model_with_same_frame_of_reference2(self):
        data = ManeuveringForceModel.parse(maneuvering_with_same_frame_of_reference())
        env = get_env_with_default_rotation_convention()
        t = 0
        states = get_body("TestShip").get_states()
        states.x.record(t, 0.1)
        states.y.record(t, 2.04)
        states.z.record(t, 6.28)
        states.u.record(t, 0.45)
        states.v.record(t, 0.01)
        states.w.record(t, 5.869)
        states.p.record(t, 0.23)
        states.q.record(t, 0)
        states.r.record(t, 0.38)

        command_listener = DataSource()
        command_listener.set_float("test(a)", 0.0)
        command_listener.set_float("test(b)", 0.0)
        command_listener.set_float("test(c)", 0.0)

        force = ManeuveringForceModel(data, "TestShip", env)
        F = force(states, t, env, command_listener)

        self.assertEqual("TestShip", F.get_frame())
        self.assertEqual(-160307.53008106418, F.X())
        self.assertEqual(349066.3153463915, F.Y())
        self.assertEqual(0, F.Z())
        self.assertAlmostEqual(0, F.K(), delta=1e-15)
        self.assertAlmostEqual(0, F.M(), delta=1e-15)
        self.assertEqual(-178317.02217866198, F.N())

    def test_can_evaluate_full_maneuvering_model(self):
        data = ManeuveringForceModel.parse(maneuvering())
        env = get_env_with_default_rotation_convention()
        t = 0
        states = get_body("TestShip").get_states()
        states.x.record(t, 1)
        states.y.record(t, 2)
        states.z.record(t, 3)
        states.u.record(t, 4)
        states.v.record(t, 5)
        states.w.record(t, 6)
        states.p.record(t, 7)
        states.q.record(t, 8)
        states.r.record(t, 9)

        command_listener = DataSource()
        command_listener.set_float("test(a)", 0.0)
        command_listener.set_float("test(b)", 0.0)
        command_listener.set_float("test(c)", 0.0)

        force = ManeuveringForceModel(data, "TestShip", env)
        F = force(states, t, env, command_listener)

        self.assertEqual("TestShip", F.get_frame())
        self.assertAlmostEqual(-154542121.87051487, F.X(), delta=1e-7)
        self.assertAlmostEqual(-33809357.80807002, F.Y(), delta=1e-7)
        self.assertAlmostEqual(-141922136.39238492, F.Z(), delta=1e-7)
        self.assertAlmostEqual(208388848.83699098, F.K(), delta=1e-7)
        self.assertAlmostEqual(-65195179.1700744, F.M(), delta=1e-7)
        self.assertAlmostEqual(-211388078.67544204, F.N(), delta=1e-7)

    def test_can_evaluate_full_maneuvering_model2(self):
        data = ManeuveringForceModel.parse(maneuvering())
        env = get_env_with_default_rotation_convention()
        t = 0
        states = get_body("TestShip").get_states()
        states.x.record(t, 0.1)
        states.y.record(t, 2.04)
        states.z.record(t, 6.28)
        states.u.record(t, 0.45)
        states.v.record(t, 0.01)
        states.w.record(t, 5.869)
        states.p.record(t, 0.23)
        states.q.record(t, 0)
        states.r.record(t, 0.38)

        command_listener = DataSource()
        command_listener.set_float("test(a)", 0.0)
        command_listener.set_float("test(b)", 0.0)
        command_listener.set_float("test(c)", 0.0)

        force = ManeuveringForceModel(data, "TestShip", env)
        F = force(states, t, env, command_listener)

        self.assertEqual("TestShip", F.get_frame())
        self.assertAlmostEqual(-276711.3255011981, F.X(), delta=1e-9)
        self.assertAlmostEqual(-70281.82055578004, F.Y(), delta=1e-9)
        self.assertAlmostEqual(-256976.85644612607, F.Z(), delta=1e-9)
        self.assertAlmostEqual(-13365.244892291332, F.K(), delta=1e-9)
        self.assertAlmostEqual(-252382.944875907, F.M(), delta=1e-9)
        self.assertAlmostEqual(83417.03517334405, F.N(), delta=1e-9)

    def test_can_use_euler_angles_in_maneuvering_with_same_frame_of_reference(self):
        yaml = """
        reference frame:
            frame: TestShip
            x: {value: 0, unit: m}
            y: {value: 0, unit: m}
            z: {value: 0, unit: m}
            phi: {value: 0, unit: rad}
            theta: {value: 0, unit: deg}
            psi: {value: 0, unit: deg}
        name: test
        X: phi(t)
        Y: theta(t)
        Z: psi(t)
        K: qr(t)
        M: qi(t)
        N: qj(t)+qk(t)
        """
        data = ManeuveringForceModel.parse(yaml)
        env = get_env_with_default_rotation_convention()
        t = 0
        states = get_body("TestShip").get_states()
        states.x.record(t, 0.1)
        states.y.record(t, 2.04)
        states.z.record(t, 6.28)
        states.u.record(t, 0.45)
        states.v.record(t, 0.01)
        states.w.record(t, 5.869)
        states.p.record(t, 0.23)
        states.q.record(t, 0)
        states.r.record(t, 0.38)
        states.qr.record(t, 0.47978680878362095)
        states.qi.record(t, 0.4931142201387216)
        states.qj.record(t, 0.5064416314938222)
        states.qk.record(t, 0.5197690428489228)
        angles = states.get_angles()
        command_listener = DataSource()
        force = ManeuveringForceModel(data, "TestShip", env)
        F = force(states, t, env, command_listener)
        self.assertEqual(angles.phi, F.X())
        self.assertEqual(angles.theta, F.Y())
        self.assertEqual(angles.psi, F.Z())
        self.assertEqual(states.qr(), F.K())
        self.assertEqual(states.qi(), F.M())
        self.assertEqual(states.qj() + states.qk(), F.N())

    def test_can_use_euler_angles_in_maneuvering(self):
        yaml = """
        reference frame:
            frame: TestShip
            x: {value: 0.696, unit: m}
            y: {value: 0, unit: m}
            z: {value: 1.418, unit: m}
            phi: {value: 0.7, unit: rad}
            theta: {value: -166, unit: deg}
            psi: {value: 125, unit: deg}
        name: test
        X: phi(t)
        Y: theta(t)
        Z: psi(t)
        K: qr(t)
        M: qi(t)
        N: qj(t)+qk(t)
        """
        data = ManeuveringForceModel.parse(yaml)
        env = get_env_with_default_rotation_convention()
        t = 0
        states = get_body("TestShip").get_states()
        states.x.record(t, 0.1)
        states.y.record(t, 2.04)
        states.z.record(t, 6.28)
        states.u.record(t, 0.45)
        states.v.record(t, 0.01)
        states.w.record(t, 5.869)
        states.p.record(t, 0.23)
        states.q.record(t, 0)
        states.r.record(t, 0.38)
        states.qr.record(t, 0.47978680878362095)
        states.qi.record(t, 0.4931142201387216)
        states.qj.record(t, 0.5064416314938222)
        states.qk.record(t, 0.5197690428489228)

        command_listener = DataSource()

        force = ManeuveringForceModel(data, "TestShip", env)
        F = force(states, t, env, command_listener)
        assert_equal = lambda x, y: self.assertAlmostEqual(x, y, delta=1e-14)
        assert_equal(+1.917565962059328, F.X())
        assert_equal(-0.8788836899394064, F.Y())
        assert_equal(-0.8088077018359574, F.Z())
        assert_equal(+1.8988663216207913, F.K())
        assert_equal(+2.8450652134487004, F.M())
        assert_equal(-1.5654434261669063, F.N())

    def test_can_get_Tmax(self):
        data = ManeuveringForceModel.parse(man_with_delay())
        env = get_env_with_default_rotation_convention()
        force = ManeuveringForceModel(data, "ball", env)
        self.assertEqual(10, force.get_Tmax())


if __name__ == "__main__":

    unittest.main()
