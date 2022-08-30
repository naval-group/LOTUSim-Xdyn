"""
Unit test for RudderForceModel
"""
import unittest

import numpy as np
from xdyn.core import (
    BlockedDOF,
    BodyStates,
    BodyWithoutSurfaceForces,
    EnvironmentAndFrames,
)
from xdyn.core.io import YamlFilteredStates, YamlRotation
from xdyn.data.yaml import rudder
from xdyn.env.wave import (
    Airy,
    DiracDirectionalSpreading,
    DiracSpectralDensity,
    Stretching,
    discretize,
)
from xdyn.force import (
    RudderForceModel,
    RudderForceModelInOutWakeSscPoint,
    RudderForceModelInput,
    RudderModel,
)
from xdyn.ssc.kinematics import Point as SscPoint
from xdyn.ssc.kinematics import Transform as SscTransform
from xdyn.ssc.random import DataGenerator

EPS: float = 1e-2
NB_TRIALS: int = 100
BODY: str = "body 1"


def get_env() -> EnvironmentAndFrames:
    env = EnvironmentAndFrames()
    env.rho = 1024
    env.rot = YamlRotation("angle", ["z", "y'", "x''"])
    return env


def get_environment_and_frames(A, phi: float) -> EnvironmentAndFrames:
    # See how to use a wave model instead of (A, phi)
    env = EnvironmentAndFrames()
    env.g = 9.81
    env.rho = 1024
    env.rot = YamlRotation("angle", ["z", "y'", "x''"])
    env.k_add(SscTransform(SscPoint("NED"), "mesh(" + BODY + ")"))
    env.k_add(SscTransform(SscPoint("NED"), BODY))
    env.set_w_from_discrete_directional_wave_spectrum(A, phi)
    return env


class RudderForceModelTest(unittest.TestCase):
    """Test class for RudderForceModel"""

    def setUp(self) -> None:
        self.rng = DataGenerator(666)

    def random_string(self) -> str:
        return self.rng.random_string()()

    def random_double(self) -> float:
        return self.rng.random_double()()

    def random_double_between(self, low: float = 0.0, high: float = 1.0) -> float:
        return self.rng.random_double().between(low, high)()

    def random_int_between(self, low: int = 0, high: int = 1) -> int:
        return self.rng.random_size_t().between(low, high)()

    def random_rudder_force_model_input(self) -> RudderForceModelInput:
        ret = RudderForceModelInput()
        ret.Ar = self.random_double_between(0, 1e5)
        ret.b = self.random_double()
        ret.blade_area_ratio = self.random_double()
        ret.diameter = self.random_double()
        ret.drag_coeff = self.random_double()
        ret.effective_aspect_ratio_factor = self.random_double()
        ret.lift_coeff = self.random_double()
        return ret

    def get_wave_model(self, phi: float = 5.8268):
        Hs = 0.1
        Tp = 5
        omega0 = 2 * np.pi / Tp
        psi = 0
        omega_min = self.rng.random_double().greater_than(0)()
        omega_max = self.rng.random_double().greater_than(omega_min)()
        nfreq = self.random_int_between(2, 100)
        ndir = self.random_int_between(2, 100)
        ss = Stretching(h=0, delta=1)
        S = DiracSpectralDensity(omega0, Hs)
        D = DiracDirectionalSpreading(psi)
        A = discretize(S, D, omega_min, omega_max, nfreq, ndir, ss, False)
        return A, phi, Airy(A, phi)

    def test_angle_of_attack(self):
        riw = RudderModel(
            self.random_rudder_force_model_input(),
            self.random_double(),
            self.random_double(),
        )
        self.assertEqual(-1, riw.get_angle_of_attack(1, 2))
        self.assertEqual(1, riw.get_angle_of_attack(2, 1))

    def test_get_Cd(self):
        parameters = self.random_rudder_force_model_input()
        parameters.Ar = 3
        parameters.b = 4
        parameters.effective_aspect_ratio_factor = 0.5
        riw = RudderModel(parameters, 1024, 2)
        Vs = 6
        Cl = 7
        self.assertEqual(6.5028916553907399, riw.get_Cd(Vs, Cl))

    def test_get_Cl(self):
        parameters = self.random_rudder_force_model_input()
        parameters.Ar = 3
        parameters.b = 4
        parameters.effective_aspect_ratio_factor = 0.5
        riw = RudderModel(parameters, 1024, 2)
        alpha = 0.4
        self.assertEqual(1.0985577009852809, riw.get_Cl(alpha))

    def test_get_lift(self):
        parameters = self.random_rudder_force_model_input()
        parameters.Ar = 10
        parameters.lift_coeff = 2
        riw = RudderModel(parameters, 1024, self.random_double())
        Vs = 12
        Cl = 1.3
        alpha = np.pi / 4
        Ar = 1.4
        self.assertEqual(189766.19029849127, riw.get_lift(Vs, Cl, alpha, Ar))

    def test_get_drag(self):
        parameters = self.random_rudder_force_model_input()
        parameters.Ar = 10
        parameters.drag_coeff = 2
        riw = RudderModel(parameters, 1024, self.random_double())
        Vs = 12
        Cl = 1.3
        alpha = np.pi / 4
        Ar = 1.4
        self.assertEqual(268369.91999999998, riw.get_drag(Vs, Cl, alpha, Ar))

    def test_get_force(self):
        riw = RudderModel(
            self.random_rudder_force_model_input(),
            self.random_double(),
            self.random_double(),
        )
        drag = 10
        lift = 200
        angle = -np.pi / 3
        v = riw.get_force(drag, lift, angle)
        self.assertEqual(-91.339745962155646, v[0])
        self.assertEqual(178.20508075688772, v[1])
        self.assertEqual(0, v[2])
        self.assertEqual(0, v[3])
        self.assertEqual(0, v[4])
        self.assertEqual(0, v[5])

    def test_get_wrench(self):
        parameters = RudderForceModelInput()
        parameters.Ar = 10
        parameters.drag_coeff = 2
        parameters.lift_coeff = 5
        parameters.b = 99
        parameters.effective_aspect_ratio_factor = 2.3
        riw = RudderModel(parameters, 1024, 0.75)
        area = 1.467
        v = riw.get_wrench(3, 4, 0.5, area)
        self.assertEqual(-2021.4412785509464, v[0])
        self.assertEqual(1757.2988992064641, v[1])
        self.assertEqual(0, v[2])
        self.assertEqual(0, v[3])
        self.assertEqual(0, v[4])
        self.assertEqual(0, v[5])

    def test_get_Ar(self):
        parameters = self.random_rudder_force_model_input()
        riw = RudderModel(parameters, self.random_double(), self.random_double())
        CTh = self.random_double_between(-1, 1000)
        ar = riw.get_Ar(CTh)
        # ar.outside_wake = parameters.Ar-ar.in_wake
        self.assertAlmostEqual(parameters.Ar, ar.in_wake + ar.outside_wake, delta=1e-10)

    def test_get_Ar2(self):
        parameters = self.random_rudder_force_model_input()
        parameters.Ar = 10
        parameters.b = 4
        parameters.position_of_propeller_frame.coordinates.x = 0
        parameters.position_of_propeller_frame.coordinates.y = 0
        parameters.position_of_propeller_frame.coordinates.z = 0
        parameters.position_of_the_rudder_frame_in_the_body_frame.x = 2.47
        parameters.position_of_the_rudder_frame_in_the_body_frame.y = 0
        parameters.position_of_the_rudder_frame_in_the_body_frame.z = 0
        parameters.diameter = 1.67
        CTh = 5.3
        riw = RudderModel(parameters, self.random_double(), self.random_double())
        ar = riw.get_Ar(CTh)
        self.assertEqual(3.5404447215261827, ar.in_wake)
        self.assertEqual(6.4595552784738173, ar.outside_wake)

    def test_get_Vs(self):
        parameters = self.random_rudder_force_model_input()
        parameters.diameter = 3.6
        riw = RudderModel(parameters, 1024, self.random_double())
        vs = riw.get_vs(1.5, 12, 6, 12e4)
        self.assertEqual(12.007932248435861, np.linalg.norm(vs.in_wake.v))
        self.assertEqual(13.416407864998739, np.linalg.norm(vs.outside_wake.v))

    def test_get_fluid_angle(self):
        riw = RudderModel(
            self.random_rudder_force_model_input(),
            self.random_double(),
            self.random_double(),
        )
        V = RudderForceModelInOutWakeSscPoint()
        V.in_wake.v = [1, 2, self.random_double()]
        V.outside_wake.v = [-4, -4, self.random_double()]
        vs = riw.get_fluid_angle(V)
        self.assertEqual(1.1071487177940904, vs.in_wake)
        self.assertEqual(-3 * np.pi / 4, vs.outside_wake)

    def test_parser(self):
        w = RudderForceModel.parse(rudder())
        self.assertEqual(0.5, w.blade_area_ratio)
        self.assertEqual("port side propeller", w.name)
        self.assertEqual(3, w.number_of_blades)
        self.assertEqual(0, w.position_of_propeller_frame.angle.phi)
        self.assertEqual(-10 / 180 * np.pi, w.position_of_propeller_frame.angle.theta)
        self.assertEqual(-1 / 180 * np.pi, w.position_of_propeller_frame.angle.psi)
        self.assertEqual(-4, w.position_of_propeller_frame.coordinates.x)
        self.assertEqual(-2, w.position_of_propeller_frame.coordinates.y)
        self.assertEqual(2, w.position_of_propeller_frame.coordinates.z)
        self.assertEqual("mesh(body 1)", w.position_of_propeller_frame.frame)
        self.assertEqual(1, w.relative_rotative_efficiency)
        self.assertTrue(w.rotating_clockwise)
        self.assertEqual(0.7, w.thrust_deduction_factor)
        self.assertEqual(0.9, w.wake_coefficient)
        self.assertEqual(2, w.diameter)
        self.assertEqual(2.2, w.Ar)
        self.assertEqual(2, w.b)
        self.assertEqual(1, w.drag_coeff)
        self.assertEqual(2.1, w.lift_coeff)
        self.assertEqual(1.7, w.effective_aspect_ratio_factor)
        self.assertEqual(-5.1, w.position_of_the_rudder_frame_in_the_body_frame.x)
        self.assertEqual(-2, w.position_of_the_rudder_frame_in_the_body_frame.y)
        self.assertEqual(2, w.position_of_the_rudder_frame_in_the_body_frame.z)

    def test_force_and_torque(self):
        A, phi, _ = self.get_wave_model()
        env = get_environment_and_frames(A, phi)
        model = RudderForceModel(RudderForceModel.parse(rudder()), self.random_string(), env)
        self.assertEqual(type(model), RudderForceModel)
        self.assertEqual("propeller+rudder", model.model_name())
        states = BodyStates()
        s = [1, 2, 3, 4, 5, 6, 0, 0, 0, 1, 0, 0, 0]
        t = 24
        states.u.record(t, s[2])
        states.v.record(t, s[3])
        states.w.record(t, s[4])
        states.name = "body"
        b = BodyWithoutSurfaceForces(states, 0, BlockedDOF(""), YamlFilteredStates())
        b.update_kinematics(s, env.k)
        commands = {"rpm": 200, "P/D": 1.2, "beta": np.pi / 6}
        F = model.get_force(states, t, env, commands)
        self.assertEqual(2208573.9553180891, F.X())
        self.assertEqual(777997.67996840423, F.Y())
        self.assertEqual(0, F.Z())
        self.assertEqual(-2793416.1021430148, F.K())
        self.assertEqual(0, F.M())
        self.assertEqual(-855797.44796524453, F.N())


if __name__ == "__main__":

    unittest.main()
