"""
Unit test for DiffractionForceModel
"""
import io
import os
import unittest
from contextlib import redirect_stderr
from textwrap import dedent

import numpy as np
from xdyn.core import BodyStates, EnvironmentAndFrames
from xdyn.core.io import YamlCoordinates, YamlRAO, YamlRotation
from xdyn.data.test import bug_3210
from xdyn.data.test.precal import precal
from xdyn.data.yaml import diffraction, diffraction_precalr
from xdyn.env.wave import (
    Airy,
    DiracDirectionalSpreading,
    DiracSpectralDensity,
    DiscreteDirectionalWaveSpectrum,
    Stretching,
    WaveModel,
    discretize,
)
from xdyn.force import DiffractionForceModel
from xdyn.ssc.kinematics import Point as SscPoint
from xdyn.ssc.kinematics import Transform as SscTransform

BODY_NAME: str = "body 1"


def get_states_with_forward_speed(u: float) -> BodyStates:
    states = BodyStates()
    states.name = BODY_NAME
    states.convention = YamlRotation("angle", ["z", "y'", "x''"])
    states.x.record(0, 0)
    states.y.record(0, 0)
    states.z.record(0, 0)
    states.u.record(0, u)
    states.v.record(0, 0)
    states.w.record(0, 0)
    states.p.record(0, 0)
    states.q.record(0, 0)
    states.r.record(0, 0)
    states.qr.record(0, 1)
    states.qi.record(0, 0)
    states.qj.record(0, 0)
    states.qr.record(0, 0)
    return states


def get_wave_model(
    period: float, direction: float = 0.0, height: float = 1.0
) -> WaveModel:
    omega = 2 * np.pi / period
    ss = Stretching(h=0, delta=1)
    A = discretize(
        DiracSpectralDensity(omega, 2 * height),
        DiracDirectionalSpreading(direction),
        omega,
        omega,
        1,
        1,
        ss,
        False,
    )
    assert type(A) == DiscreteDirectionalWaveSpectrum
    return A, Airy(A, 0.0)


def get_waves_env(
    period: float, direction: float = 0.0, amplitude: float = 1.0
) -> EnvironmentAndFrames:
    env = EnvironmentAndFrames()
    env.g = 9.81
    env.rho = 1000
    env.rot = YamlRotation("angle", ["z", "y'", "x''"])
    env.k_add(SscTransform(SscPoint("NED"), BODY_NAME))
    A, wave_model = get_wave_model(period, direction, amplitude)
    # TODO: Use wave_model instead of A
    # For time being, the C++ wrapper code crashes
    env.set_w_from_discrete_directional_wave_spectrum(A, 0.0)
    return env


def small_relative_error(val: float) -> float:
    eps = 1e-4
    return max(eps, abs(val * eps))


class DiffractionForceModelTest(unittest.TestCase):
    """Test class for DiffractionForceModel"""

    def test_parser_hdb(self):
        data = DiffractionForceModel.parse(diffraction())
        self.assertEqual(type(data), YamlRAO)
        self.assertEqual("test_ship.hdb", data.hdb_filename)
        self.assertEqual(0, len(data.precal_filename))
        self.assertEqual(0.696, data.calculation_point.x)
        self.assertEqual(0, data.calculation_point.y)
        self.assertEqual(1.418, data.calculation_point.z)
        self.assertTrue(data.mirror)
        self.assertTrue(data.use_encounter_period)

    def test_parser_precalr(self):
        data = DiffractionForceModel.parse(diffraction_precalr())
        self.assertEqual(type(data), YamlRAO)
        self.assertEqual("test_ship.raodb.ini", data.precal_filename)
        self.assertEqual(0, len(data.hdb_filename))
        self.assertEqual(0, data.calculation_point.x)
        self.assertEqual(0, data.calculation_point.y)
        self.assertEqual(0, data.calculation_point.z)
        self.assertTrue(data.mirror)
        self.assertTrue(data.use_encounter_period)

    def test_parser_precalr_ignores_calculation_point(self):
        test_data = diffraction_precalr()
        additional_test_data = """
        calculation point in body frame:
            x: {value: 0.696, unit: m}
            y: {value: 0, unit: m}
            z: {value: 1.418, unit: m}
        """
        test_data += dedent(additional_test_data)
        data = DiffractionForceModel.parse(test_data)
        self.assertEqual(0, data.calculation_point.x)
        self.assertEqual(0, data.calculation_point.y)
        self.assertEqual(0, data.calculation_point.z)

    def test_encounter_frequency_example(self):
        env = get_waves_env(64.0, 0.0)
        hdb_filename = "data.hdb"
        with open(hdb_filename, "w", encoding="utf-8") as fid:
            fid.write(bug_3210())
        data = YamlRAO()
        data.calculation_point = YamlCoordinates(0.696, 0, 1.418)
        data.hdb_filename = hdb_filename
        data.mirror = True
        data.use_encounter_period = True
        force_model = DiffractionForceModel(data, BODY_NAME, env)
        T = 64.0

        # Ship heading is always 0 (due North), encounter period varies with U and wave heading
        # Values from the force model are compared to values from the HDB data (amplitude and phase)

        # First battery of tests: wave direction=0° (due North)
        env = get_waves_env(T, 0.0)

        # U=0, wave direction=0° -> encounter period = wave period = 64s, incidence = 0°
        states = get_states_with_forward_speed(0.0)
        F = force_model.get_force(states, 0.0, env)
        assert_equal = lambda x, y, delta: self.assertAlmostEqual(x, y, delta=delta)
        assert_equal(F.X(), 3.378373e3 * np.sin(-2.088816), 10)
        assert_equal(F.Y(), 0, 10)
        assert_equal(F.Z(), -3.282703e5 * np.sin(-1.636404), 20)
        assert_equal(F.K(), 0, 10)
        assert_equal(F.M(), -2.267054e6 * np.sin(-1.789263), 400)
        assert_equal(F.N(), 0, 10)

        # U=48.763, wave direction=0°, wave period = 64s -> encounter period = 125s, incidence = 0°
        states = get_states_with_forward_speed(48.762833661758)
        F = force_model.get_force(states, 0.0, env)
        assert_equal(F.X(), 9.472648e2 * np.sin(-2.189364), 10)
        assert_equal(F.Y(), 0, 10)
        assert_equal(F.Z(), -8.336677e4 * np.sin(-1.596137), 10)
        assert_equal(F.K(), 0, 10)
        assert_equal(F.M(), -5.817311e5 * np.sin(-1.825686), 110)
        assert_equal(F.N(), 0, 10)

        # U=-1498.86, wave direction=0°, wave period = 64s -> encounter period = 4s, incidence = 0°
        states = get_states_with_forward_speed(-1498.8575920622336)
        F = force_model.get_force(states, 0.0, env)
        assert_equal(F.X(), 5.084407e4 * np.sin(1.135123), 20)
        assert_equal(F.Y(), 0, 10)
        assert_equal(F.Z(), -3.997774e5 * np.sin(8.389206e-1), 200)
        assert_equal(F.K(), 0, 10)
        assert_equal(
            F.M(), -3.209051e7 * np.sin(8.356066e-1), 15000
        )  # Still a small relative error
        assert_equal(F.N(), 0, 10)

        # Second battery of tests: wave direction=-30° (North-West)
        env = get_waves_env(T, -30.0 * np.pi / 180)

        # U=0, wave direction=-30° -> encounter period = wave period = 64s, incidence = 30°
        states = get_states_with_forward_speed(0.0)
        F = force_model.get_force(states, 0.0, env)
        assert_equal(F.X(), 3.275670e3 * np.sin(-2.028876), 10)
        assert_equal(F.Y(), -3.409648e4 * np.sin(3.130008), 30)
        assert_equal(F.Z(), -3.283568e5 * np.sin(-1.636047), 20)
        assert_equal(F.K(), 1.685674e4 * np.sin(1.350179e-1), 10)
        assert_equal(F.M(), -2.255748e6 * np.sin(-1.768062), 300)
        assert_equal(F.N(), -4.594584e5 * np.sin(3.053413), 300)

        # U=56.31, wave direction=-30°, wave period = 64s -> encounter period = 125s, incidence = 30°
        states = get_states_with_forward_speed(56.306470282129844)
        F = force_model.get_force(states, 0.0, env)
        assert_equal(F.X(), 9.067188e2 * np.sin(-2.123063), 10)
        assert_equal(F.Y(), -8.953133e3 * np.sin(3.138567), 10)
        assert_equal(F.Z(), -8.339660e4 * np.sin(-1.595086), 10)
        assert_equal(F.K(), 4.420171e3 * np.sin(3.533363e-2), 10)
        assert_equal(F.M(), -5.772251e5 * np.sin(-1.794767), 100)
        assert_equal(F.N(), -1.199319e5 * np.sin(3.118377), 100)

        # U=-1730.73, wave direction=-30°, wave period = 64s -> encounter period = 4s, incidence = 30°
        states = get_states_with_forward_speed(-1730.7316685080896)
        F = force_model.get_force(states, 0.0, env)
        assert_equal(F.X(), 8.567009e4 * np.sin(-2.077326), 30)
        assert_equal(F.Y(), -3.284304e5 * np.sin(5.459499e-1), 200)
        assert_equal(F.Z(), -5.820626e5 * np.sin(-1.525810), 20)
        assert_equal(F.K(), 1.460688e6 * np.sin(6.670656e-1), 700)
        assert_equal(
            F.M(), -3.140660e7 * np.sin(-1.375271), 4000
        )  # Still a small relative error
        assert_equal(
            F.N(), -2.301511e7 * np.sin(5.846877e-1), 12000
        )  # Still a small relative error
        os.remove(hdb_filename)

    def test_precal_r_example(self):
        env = get_waves_env(10.47198, 0.0)
        precal_filename = "data.raodb.ini"
        with open(precal_filename, "w", encoding="utf-8") as fid:
            fid.write(precal())
        data = YamlRAO()
        data.calculation_point = YamlCoordinates(0, 0, 0)
        data.precal_filename = precal_filename
        data.mirror = True
        data.use_encounter_period = True
        force_model = DiffractionForceModel(data, BODY_NAME, env)
        T = 10.47198

        # Ship heading is always 0 (due North), encounter period varies with U and wave heading
        # Values from the force model are compared to values from the PRECAL_R data (amplitude and phase)

        # First battery of tests: wave direction=-180° (due South)
        env = get_waves_env(T, -np.pi)

        # U=0, wave direction=-180° -> encounter period = wave period = 10.47198s (frequency = 0.6 rad/s => third line), incidence = 180°
        states = get_states_with_forward_speed(0.0)
        F = force_model.get_force(states, 0.0, env)
        assert_equal = lambda x, y, delta: self.assertAlmostEqual(x, y, delta=delta)
        assert_equal(
            F.X(),
            0.549354e03 * 1e3 * np.sin(-(51.238937) * np.pi / 180.0),
            small_relative_error(F.X()),
        )
        assert_equal(
            -F.Y(),
            0.187922e-02 * 1e3 * np.sin(-(-47.399357) * np.pi / 180.0),
            small_relative_error(F.Y()),
        )
        assert_equal(
            -F.Z(),
            0.414623e04 * 1e3 * np.sin(-(117.171936) * np.pi / 180.0),
            small_relative_error(F.Z()),
        )
        assert_equal(
            F.K(),
            0.688938e-02 * 1e3 * np.sin(-(-73.747025) * np.pi / 180.0),
            small_relative_error(F.K()),
        )
        assert_equal(
            -F.M(),
            0.197622e06 * 1e3 * np.sin(-(55.770741) * np.pi / 180.0),
            small_relative_error(F.M()),
        )
        assert_equal(
            -F.N(),
            0.418582e-01 * 1e3 * np.sin(-(66.399605) * np.pi / 180.0),
            small_relative_error(F.N()),
        )

        # U=-5.45, wave direction=-180°, wave period = 10.47198s -> encounter frequency = 0.4 rad/s => first line, incidence = 180°
        states = get_states_with_forward_speed(-5.45)
        buf = io.StringIO()
        with redirect_stderr(buf):
            F = force_model.get_force(states, 0.0, env)
        assert_equal(
            F.X(),
            0.138050e03 * 1e3 * np.sin(-(90.317017) * np.pi / 180.0),
            small_relative_error(F.X()),
        )
        assert_equal(
            -F.Y(),
            0.117473e-02 * 1e3 * np.sin(-(-48.947906) * np.pi / 180.0),
            small_relative_error(F.Y()),
        )
        assert_equal(
            -F.Z(),
            0.444391e04 * 1e3 * np.sin(-(140.987823) * np.pi / 180.0),
            small_relative_error(F.Z()),
        )
        assert_equal(
            F.K(),
            0.121499e-02 * 1e3 * np.sin(-(-77.528954) * np.pi / 180.0),
            small_relative_error(F.K()),
        )
        assert_equal(
            -F.M(),
            0.692932e05 * 1e3 * np.sin(-(100.971458) * np.pi / 180.0),
            small_relative_error(F.M()),
        )
        assert_equal(
            -F.N(),
            0.883748e-02 * 1e3 * np.sin(-(-61.118809) * np.pi / 180.0),
            small_relative_error(F.N()),
        )

        # U=10.9, wave direction=-180°, wave period = 10.47198s -> encounter frequency = 1 rad/s => seventh line, incidence = 180°
        states = get_states_with_forward_speed(10.9)
        F = force_model.get_force(states, 0.0, env)
        assert_equal(
            F.X(),
            0.248856e03 * 1e3 * np.sin(-(-146.170074) * np.pi / 180.0),
            small_relative_error(F.X()),
        )
        assert_equal(
            -F.Y(),
            0.205086e-03 * 1e3 * np.sin(-(-169.398438) * np.pi / 180.0),
            small_relative_error(F.Y()),
        )
        assert_equal(
            -F.Z(),
            0.127826e04 * 1e3 * np.sin(-(-114.645767) * np.pi / 180.0),
            small_relative_error(F.Z()),
        )
        assert_equal(
            F.K(),
            0.191906e-02 * 1e3 * np.sin(-(63.095062) * np.pi / 180.0),
            small_relative_error(F.K()),
        )
        assert_equal(
            -F.M(),
            0.911647e05 * 1e3 * np.sin(-(-164.636292) * np.pi / 180.0),
            small_relative_error(F.M()),
        )
        assert_equal(
            -F.N(),
            0.291656e-01 * 1e3 * np.sin(-(-50.890854) * np.pi / 180.0),
            small_relative_error(F.N()),
        )

        # Second battery of tests: wave direction=-90° (North-West)
        env = get_waves_env(T, -90.0 * np.pi / 180)

        # U=0, wave direction=-90° -> encounter period = wave period = 10.47198s (frequency = 0.6 rad/s => third line), incidence = 90°
        states = get_states_with_forward_speed(0.0)
        F = force_model.get_force(states, 0.0, env)
        # 0.114735E+03     164.462891
        assert_equal(
            F.X(),
            0.114735e03 * 1e3 * np.sin(-(164.462891) * np.pi / 180.0),
            small_relative_error(F.X()),
        )
        assert_equal(
            -F.Y(),
            0.250271e04 * 1e3 * np.sin(-(78.760712) * np.pi / 180.0),
            small_relative_error(F.Y()),
        )
        assert_equal(
            -F.Z(),
            0.847017e04 * 1e3 * np.sin(-(131.552856) * np.pi / 180.0),
            small_relative_error(F.Z()),
        )
        assert_equal(
            F.K(),
            0.967723e03 * 1e3 * np.sin(-(-128.995148) * np.pi / 180.0),
            small_relative_error(F.K()),
        )
        assert_equal(
            -F.M(),
            0.611019e05 * 1e3 * np.sin(-(138.192886) * np.pi / 180.0),
            small_relative_error(F.M()),
        )
        assert_equal(
            -F.N(),
            0.400660e05 * 1e3 * np.sin(-(84.480057) * np.pi / 180.0),
            small_relative_error(F.N()),
        )

        # U=10, wave direction=-90° -> encounter period = wave period = 15.70796s (frequency = 0.4 rad/s => first line), incidence = 90°
        env = get_waves_env(15.70796, -90.0 * np.pi / 180)
        states = get_states_with_forward_speed(10)
        F = force_model.get_force(states, 0.0, env)
        assert_equal(
            F.X(),
            0.517219e02 * 1e3 * np.sin(-(-175.715546) * np.pi / 180.0),
            small_relative_error(F.X()),
        )
        assert_equal(
            -F.Y(),
            0.116960e04 * 1e3 * np.sin(-(88.327232) * np.pi / 180.0),
            small_relative_error(F.Y()),
        )
        assert_equal(
            -F.Z(),
            0.516786e04 * 1e3 * np.sin(-(146.821213) * np.pi / 180.0),
            small_relative_error(F.Z()),
        )
        assert_equal(
            F.K(),
            0.554469e03 * 1e3 * np.sin(-(-94.531265) * np.pi / 180.0),
            small_relative_error(F.K()),
        )
        assert_equal(
            -F.M(),
            0.355934e05 * 1e3 * np.sin(-(150.882507) * np.pi / 180.0),
            small_relative_error(F.M()),
        )
        assert_equal(
            -F.N(),
            0.167614e05 * 1e3 * np.sin(-(89.216774) * np.pi / 180.0),
            small_relative_error(F.N()),
        )

        # U=-10, wave direction=-90° -> encounter period = wave period = 6.28319s (frequency = 1 rad/s => seventh line), incidence = 90°
        env = get_waves_env(6.28319, -90.0 * np.pi / 180)
        states = get_states_with_forward_speed(-12.586)
        F = force_model.get_force(states, 0.0, env)
        assert_equal(
            F.X(),
            0.175932e03 * 1e3 * np.sin(-(163.390305) * np.pi / 180.0),
            small_relative_error(F.X()),
        )
        assert_equal(
            -F.Y(),
            0.261594e04 * 1e3 * np.sin(-(53.043697) * np.pi / 180.0),
            small_relative_error(F.Y()),
        )
        assert_equal(
            -F.Z(),
            0.117765e05 * 1e3 * np.sin(-(128.725204) * np.pi / 180.0),
            small_relative_error(F.Z()),
        )
        assert_equal(
            F.K(),
            0.587610e04 * 1e3 * np.sin(-(72.772514) * np.pi / 180.0),
            small_relative_error(F.K()),
        )
        assert_equal(
            -F.M(),
            0.977144e05 * 1e3 * np.sin(-(138.608902) * np.pi / 180.0),
            small_relative_error(F.M()),
        )
        assert_equal(
            -F.N(),
            0.108441e06 * 1e3 * np.sin(-(59.445541) * np.pi / 180.0),
            small_relative_error(F.N()),
        )
        os.remove(precal_filename)


if __name__ == "__main__":

    unittest.main()
