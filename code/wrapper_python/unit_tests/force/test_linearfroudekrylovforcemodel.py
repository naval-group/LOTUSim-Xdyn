"""
Unit test for LinearFroudeKrylovForceModel
"""
import os
import unittest

import numpy as np
from xdyn.core import BodyStates, EnvironmentAndFrames
from xdyn.core.io import TypeOfRao, YamlCoordinates, YamlRAO, YamlRotation
from xdyn.data.test import big_hdb
from xdyn.data.test.precal import precal
from xdyn.env.wave import (
    Airy,
    DiracDirectionalSpreading,
    DiracSpectralDensity,
    DiscreteDirectionalWaveSpectrum,
    Stretching,
    WaveModel,
    discretize,
)
from xdyn.force import LinearFroudeKrylovForceModel
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


class LinearFroudeKrylovForceModelTest(unittest.TestCase):
    """Test class for LinearFroudeKrylovForceModel"""

    def test_encounter_frequency_example(self):
        env = get_waves_env(64.0, 0.0)
        states = BodyStates()
        hdb_file = "data.hdb"
        with open(hdb_file, "w", encoding="utf-8") as fid:
            fid.write(big_hdb())
        data = YamlRAO()
        data.calculation_point = YamlCoordinates(0.696, 0, 1.418)
        data.hdb_filename = hdb_file
        data.mirror = True
        data.use_encounter_period = True
        data.type_of_rao = TypeOfRao.FROUDE_KRYLOV_RAO
        force_model = LinearFroudeKrylovForceModel(data, BODY_NAME, env)
        T = 64

        # Ship heading is always 0 (due North), encounter period varies with U and wave heading
        # Values from the force model are compared to values from the HDB data (amplitude and phase)

        # Wave direction=0° (due North)
        env = get_waves_env(T, 0.0)

        # U=48.763, wave direction=0°, wave period = 64s -> encounter period = 125s, incidence = 0°
        states = get_states_with_forward_speed(48.762833661758)
        F = force_model.get_force(states, 0.0, env)
        kx = 0.000683817
        assert_equal = lambda x, y, delta=10: self.assertAlmostEqual(x, y, delta=delta)
        assert_equal(+F.X(), 2.164523e04 * np.sin(-(1.570774e00 - kx)), delta=10)
        assert_equal(-F.Y(), 0.000000e00 * np.sin(-(-3.141593e00 - kx)), delta=10)
        assert_equal(-F.Z(), 2.106490e07 * np.sin(-(1.577854e-03 - kx)), delta=10)
        assert_equal(+F.K(), 0.000000e00 * np.sin(-(-3.141593e00 - kx)), delta=10)
        assert_equal(-F.M(), 1.291984e08 * np.sin(-(5.123568e-02 - kx)), delta=110)
        assert_equal(-F.N(), 0.000000e00 * np.sin(-(-3.141593e00 - kx)), delta=10)
        os.remove(hdb_file)

    def test_precal_r_example(self):
        env = get_waves_env(10.47198, 0.0)
        states = BodyStates()
        precal_filename = "data.raodb.ini"
        with open(precal_filename, "w", encoding="utf-8") as fid:
            fid.write(precal())
        data = YamlRAO()
        data.calculation_point = YamlCoordinates(0, 0, 0)
        data.precal_filename = "data.raodb.ini"
        data.mirror = True
        data.use_encounter_period = True
        data.type_of_rao = TypeOfRao.FROUDE_KRYLOV_RAO
        force_model = LinearFroudeKrylovForceModel(data, BODY_NAME, env)
        T = 10.47198

        # Ship heading is always 0 (due North), encounter period varies with U and wave heading
        # Values from the force model are compared to values from the PRECAL_R data (amplitude and phase)

        # First battery of tests: wave direction=-180° (due South)
        env = get_waves_env(T, -np.pi)

        # U=0, wave direction=-180° -> encounter period = wave period = 10.47198s (frequency = 0.6 rad/s => third line), incidence = 180°
        states = get_states_with_forward_speed(0.0)
        F = force_model.get_force(states, 0.0, env)
        sin_deg = lambda x: np.sin(x * np.pi / 180.0)
        assert_equal = lambda x, y, delta: self.assertAlmostEqual(x, y, delta=delta)
        tol_fx = small_relative_error(F.X())
        tol_fy = small_relative_error(F.Y())
        tol_fz = small_relative_error(F.Z())
        tol_mx = small_relative_error(F.K())
        tol_my = small_relative_error(F.M())
        tol_mz = small_relative_error(F.N())
        assert_equal(+F.X(), 0.137233e4 * 1e3 * sin_deg(-(-91.060349)), tol_fx)
        assert_equal(-F.Y(), 0.185689e-2 * 1e3 * sin_deg(-(99.457497)), tol_fy)
        assert_equal(-F.Z(), 0.672921e04 * 1e3 * sin_deg(-(-16.163645)), tol_fz)
        assert_equal(+F.K(), 0.122369e-01 * 1e3 * sin_deg(-(129.387329)), tol_mx)
        assert_equal(-F.M(), 0.459008e06 * 1e3 * sin_deg(-(-97.283081)), tol_my)
        assert_equal(-F.N(), 0.120006e00 * 1e3 * sin_deg(-(-96.079956)), tol_mz)
        os.remove(precal_filename)


if __name__ == "__main__":

    unittest.main()
