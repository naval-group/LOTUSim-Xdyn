"""
Unit test for RadiationDampingForceModel
"""
import io
import os
import unittest
from contextlib import redirect_stderr
from typing import Callable

import numpy as np

from xdyn.core import BodyStates, EnvironmentAndFrames
from xdyn.core.io import TypeOfQuadrature, YamlRadiationDamping
from xdyn.data.test import analytical_Br, analytical_K, bug_3210
from xdyn.data.test.precal import precal
from xdyn.data.yaml import radiation_damping
from xdyn.exceptions import InvalidInputException
from xdyn.force import RadiationDampingForceModel, RadiationDampingForceModelInput
from xdyn.hdb import RadiationDampingBuilder
from xdyn.ssc.integrate import ClenshawCurtisCosine, Simpson
from xdyn.ssc.random import DataGenerator


def get_yaml_data(show_debug: bool = False) -> YamlRadiationDamping:
    ret = YamlRadiationDamping()
    ret.nb_of_points_for_retardation_function_discretization = 50
    ret.omega_min = 0
    ret.omega_max = 30
    ret.output_Br_and_K = show_debug
    ret.tau_min = 0.2094395
    ret.tau_max = 10
    ret.type_of_quadrature_for_convolution = TypeOfQuadrature.SIMPSON
    ret.type_of_quadrature_for_cos_transform = TypeOfQuadrature.SIMPSON
    return ret


def compute_Ma_from_K(
    omega: float, A: float, K: Callable[[float], float], tau_min: float, tau_max: float
) -> float:
    g = lambda tau: K(tau) * np.sin(omega * tau)
    return A - (1 / omega) * Simpson(g).integrate_f(tau_min, tau_max, 1e-2)


def get_hdb_data(yaml: YamlRadiationDamping, only_diagonal_terms: bool = True):
    omega_min = 0.01
    omega_max = 40
    N = 460
    A = 1000
    omegas = RadiationDampingBuilder(
        TypeOfQuadrature.FILON, TypeOfQuadrature.GAUSS_KRONROD
    ).build_exponential_intervals(omega_min, omega_max, N)
    Br = [analytical_Br(omega) for omega in omegas]
    Ma = [
        compute_Ma_from_K(omega, A, analytical_K, yaml.tau_min, yaml.tau_max)
        for omega in omegas
    ]
    return omegas, Ma, Br, only_diagonal_terms


def record(states: BodyStates, t: float, value: float):
    states.u.record(t, value)
    states.v.record(t, value)
    states.w.record(t, value)
    states.p.record(t, value)
    states.q.record(t, value)
    states.r.record(t, value)


def record_uvwpqr(
    states: BodyStates,
    t: float,
    u: float,
    v: float,
    w: float,
    p: float,
    q: float,
    r: float,
):
    states.u.record(t, u)
    states.v.record(t, v)
    states.w.record(t, w)
    states.p.record(t, p)
    states.q.record(t, q)
    states.r.record(t, r)


def record_sine(
    states: BodyStates, t_start: float, T: float, n_periods: int, n_steps: int
) -> float:
    t_end = t_start + T * n_periods
    dt = (t_end - t_start) / n_steps
    for i in range(n_steps + 1):
        t = t_start + i * dt
        record(states, t, np.sin(2 * np.pi * (t - t_start) / T))
    return t_end


def record_offset_sine(
    states: BodyStates,
    t_start: float,
    T: float,
    n_periods: int,
    n_steps: int,
    offset: float,
) -> float:
    tend = t_start + T * n_periods
    dt = (tend - t_start) / n_steps
    for i in range(n_steps + 1):
        t = t_start + i * dt
        record(states, t, offset + np.sin(2 * np.pi * (t - t_start) / T))
    return tend


class RadiationDampingForceModelTest(unittest.TestCase):
    """Test class for RadiationDampingForceModel"""

    def setUp(self):
        self.rng = DataGenerator(666)
        hdb_filename = "test_ship.hdb"
        with open(hdb_filename, "w", encoding="utf-8") as fid:
            fid.write(bug_3210())

    def tearDown(self) -> None:
        hdb_filename = "test_ship.hdb"
        os.remove(hdb_filename)

    def test_can_parse(self):
        """Check that parse function produces a valid RadiationDampingForceModelTest object"""
        data = RadiationDampingForceModel.parse(radiation_damping()).yaml
        self.assertEqual("test_ship.hdb", data.hdb_filename)
        self.assertEqual(50, data.nb_of_points_for_retardation_function_discretization)
        self.assertEqual(30, data.omega_max)
        self.assertEqual(0, data.omega_min)
        self.assertTrue(data.output_Br_and_K)
        self.assertEqual(10, data.tau_max)
        self.assertEqual(0.2094395, data.tau_min)
        self.assertEqual(
            TypeOfQuadrature.CLENSHAW_CURTIS, data.type_of_quadrature_for_convolution
        )
        self.assertEqual(
            TypeOfQuadrature.SIMPSON, data.type_of_quadrature_for_cos_transform
        )
        self.assertEqual(0.696, data.calculation_point_in_body_frame.x)
        self.assertEqual(0, data.calculation_point_in_body_frame.y)
        self.assertEqual(1.418, data.calculation_point_in_body_frame.z)
        self.assertTrue(data.forward_speed_correction)

    def test_example(self):
        yaml = get_yaml_data(False)
        data = RadiationDampingForceModelInput()
        data.set_hdb_parser_for_tests(*get_hdb_data(data.yaml))
        data.yaml = yaml
        env = EnvironmentAndFrames()
        body_name = self.rng.random_string()()
        F = RadiationDampingForceModel(data, body_name, env)
        self.assertEqual("radiation damping", F.model_name())
        states = BodyStates(100)
        states.name = body_name
        T = 10
        t = record_sine(states, 0, T, 10, 100)
        g = lambda t: analytical_K(t) * np.sin(-2 * np.pi * t / T)
        Fexpected = -ClenshawCurtisCosine(g, 0).integrate_f(yaml.tau_min, yaml.tau_max)
        EPS = 7e-2
        Frad = F.get_force(states, 100, env)
        self.assertEqual(Frad.X(), Frad.Y())
        self.assertEqual(Frad.X(), Frad.Z())
        self.assertEqual(Frad.K(), Frad.M())
        self.assertEqual(Frad.M(), Frad.N())
        self.assertAlmostEqual(Fexpected, Frad.X(), delta=EPS)
        self.assertAlmostEqual(Fexpected, Frad.Y(), delta=EPS)
        self.assertAlmostEqual(Fexpected, Frad.Z(), delta=EPS)
        self.assertAlmostEqual(Fexpected, Frad.K(), delta=EPS)
        self.assertAlmostEqual(Fexpected, Frad.M(), delta=EPS)
        self.assertAlmostEqual(Fexpected, Frad.N(), delta=EPS)

    def test_results_are_zero_for_constant_velocity(self):
        yaml = get_yaml_data(False)
        data = RadiationDampingForceModelInput()
        data.set_hdb_parser_for_tests(*get_hdb_data(data.yaml))
        data.yaml = yaml
        data.yaml.remove_constant_speed = True
        env = EnvironmentAndFrames()
        body_name = self.rng.random_string()()
        F = RadiationDampingForceModel(data, body_name, env)
        self.assertEqual("radiation damping", F.model_name())
        states = BodyStates(100)
        states.name = body_name
        velocity = self.rng.random_double()()
        record(states, 0, velocity)
        record(states, 100, velocity)
        EPS = 5e-2
        Frad = F.get_force(states, 100, env)
        self.assertEqual(Frad.X(), Frad.Y())
        self.assertEqual(Frad.X(), Frad.Z())
        self.assertEqual(Frad.K(), Frad.M())
        self.assertEqual(Frad.M(), Frad.N())
        self.assertAlmostEqual(0, Frad.X(), delta=EPS)
        self.assertAlmostEqual(0, Frad.Y(), delta=EPS)
        self.assertAlmostEqual(0, Frad.Z(), delta=EPS)
        self.assertAlmostEqual(0, Frad.K(), delta=EPS)
        self.assertAlmostEqual(0, Frad.M(), delta=EPS)
        self.assertAlmostEqual(0, Frad.N(), delta=EPS)

    def test_velocity_offset_should_not_change_the_result(self):
        yaml = get_yaml_data(False)
        data = RadiationDampingForceModelInput()
        data.set_hdb_parser_for_tests(*get_hdb_data(data.yaml))
        data.yaml = yaml
        data.yaml.remove_constant_speed = True
        env = EnvironmentAndFrames()
        body_name = self.rng.random_string()()
        F = RadiationDampingForceModel(data, body_name, env)
        self.assertEqual("radiation damping", F.model_name())
        states1 = BodyStates(100)
        states1.name = body_name
        T_sine = 10
        record_sine(states1, 0, T_sine, 10, 100)
        states2 = BodyStates(100)
        states2.name = body_name
        offset = self.rng.random_double()()
        record_offset_sine(states2, 0, T_sine, 10, 100, offset)
        EPS = 5e-2
        Frad1 = F.get_force(states1, 100, env)
        Frad2 = F.get_force(states2, 100, env)
        self.assertAlmostEqual(Frad1.X(), Frad2.X(), delta=EPS)
        self.assertAlmostEqual(Frad1.Y(), Frad2.Y(), delta=EPS)
        self.assertAlmostEqual(Frad1.Z(), Frad2.Z(), delta=EPS)
        self.assertAlmostEqual(Frad1.K(), Frad2.K(), delta=EPS)
        self.assertAlmostEqual(Frad1.M(), Frad2.M(), delta=EPS)
        self.assertAlmostEqual(Frad1.N(), Frad2.N(), delta=EPS)

    def test_should_print_debugging_information_if_required_by_yaml_data(self):
        data = RadiationDampingForceModelInput()
        data.yaml = get_yaml_data(show_debug=True)
        data.set_hdb_parser_for_tests(*get_hdb_data(data.yaml))
        env = EnvironmentAndFrames()
        buf = io.StringIO()
        with redirect_stderr(buf):
            RadiationDampingForceModel(data, "", env)
        expected_regex = "Debugging information for damping functions Br"
        self.assertTrue(expected_regex in buf.getvalue(), buf.getvalue())

    def test_should_not_print_debugging_information_if_not_required_by_yaml_data(self):
        data = RadiationDampingForceModelInput()
        data.yaml = get_yaml_data(show_debug=False)
        data.set_hdb_parser_for_tests(*get_hdb_data(data.yaml))
        env = EnvironmentAndFrames()
        buf = io.StringIO()
        with redirect_stderr(buf):
            RadiationDampingForceModel(data, "", env)
        expected_regex = "Debugging information for damping functions Br"
        self.assertFalse(expected_regex in buf.getvalue(), buf.getvalue())

    def test_force_model_knows_history_length(self):
        data = RadiationDampingForceModelInput()
        data.yaml = get_yaml_data(False)
        data.set_hdb_parser_for_tests(*get_hdb_data(data.yaml))
        F = RadiationDampingForceModel(data, "", EnvironmentAndFrames())
        self.assertEqual(data.yaml.tau_max, F.get_Tmax())

    def test_matrix_product_should_be_done_properly(self):
        data = RadiationDampingForceModelInput()
        data.yaml = get_yaml_data(False)
        only_diagonal_terms = True
        data.set_hdb_parser_for_tests(*get_hdb_data(data.yaml, not only_diagonal_terms))
        data.yaml.type_of_quadrature_for_convolution = TypeOfQuadrature.RECTANGLE
        env = EnvironmentAndFrames()
        F = RadiationDampingForceModel(data, "", env)
        states = BodyStates(100)
        tmin = 0.20943950000000000067
        tmax = 10
        eps = 1e-6
        t0 = 5.1047197500000001114

        record(states, 0, 0)
        record(states, tmax - t0 - eps, 0)
        u0 = 1
        v0 = 2
        w0 = 3
        p0 = 4
        q0 = 5
        r0 = 6
        record_uvwpqr(states, tmax - t0 - eps / 2, u0, v0, w0, p0, q0, r0)
        record_uvwpqr(states, tmax - t0 + eps / 2, u0, v0, w0, p0, q0, r0)
        record(states, tmax - t0 + eps, 0)
        record(states, tmax, 0)
        Frad = F.get_force(states, 0, env)
        # This is the result of the discrete Fourier transform: it does not
        # exactly match the value given by the (analytical) continuous Fourier
        # transform.
        k = -0.50135576185179109299
        self.assertAlmostEqual(analytical_K(t0), k, delta=3e-3)
        conv = -(tmax - tmin) / 100 * k
        EPS = 5e-2
        self.assertAlmostEqual(
            conv * (11 * u0 + 12 * v0 + 13 * w0 + 14 * p0 + 15 * q0 + 16 * r0),
            Frad.X(),
            delta=EPS,
        )
        self.assertAlmostEqual(
            conv * (21 * u0 + 22 * v0 + 23 * w0 + 24 * p0 + 25 * q0 + 26 * r0),
            Frad.Y(),
            delta=EPS,
        )
        self.assertAlmostEqual(
            conv * (31 * u0 + 32 * v0 + 33 * w0 + 34 * p0 + 35 * q0 + 36 * r0),
            Frad.Z(),
            delta=EPS,
        )
        self.assertAlmostEqual(
            conv * (41 * u0 + 42 * v0 + 43 * w0 + 44 * p0 + 45 * q0 + 46 * r0),
            Frad.K(),
            delta=10 * EPS,
        )
        self.assertAlmostEqual(
            conv * (51 * u0 + 52 * v0 + 53 * w0 + 54 * p0 + 55 * q0 + 56 * r0),
            Frad.M(),
            delta=10 * EPS,
        )
        self.assertAlmostEqual(
            conv * (61 * u0 + 62 * v0 + 63 * w0 + 64 * p0 + 65 * q0 + 66 * r0),
            Frad.N(),
            delta=10 * EPS,
        )

    def test_cannot_specify_both_hdb_and_precal_r_files(self):
        invalid_yaml = """
        model: radiation damping
        hdb: test_ship.hdb
        raodb: test_ship.ini
        type of quadrature for cos transform: simpson
        type of quadrature for convolution: clenshaw-curtis
        nb of points for retardation function discretization: 50
        omega min: {value: 0, unit: rad/s}
        omega max: {value: 30, unit: rad/s}
        tau min: {value: 0.2094395, unit: s}
        tau max: {value: 10, unit: s}
        output Br and K: true
        forward speed correction: true
        calculation point in body frame:
            x: {value: 0.696, unit: m}
            y: {value: 0, unit: m}
            z: {value: 1.418, unit: m}
        """

        expected_msg = (
            "When using the radiation force model, you cannot specify "
            "both the 'hdb' and 'raodb' in the YAML, as xdyn would not know which "
            "one to use to retrieve radiation damping coefficients: you should remove"
            " either 'hdb' or 'raodb' from the YAML file."
        )
        with self.assertRaises(InvalidInputException) as pcm:
            RadiationDampingForceModel.parse(invalid_yaml, False)
        self.assertTrue(expected_msg in str(pcm.exception), str(pcm.exception))

    def test_should_be_able_to_use_precal_r_files_only(self):
        valid_yaml = """
        model: radiation damping
        raodb: test_ship.ini
        type of quadrature for cos transform: simpson
        type of quadrature for convolution: clenshaw-curtis
        nb of points for retardation function discretization: 50
        omega min: {value: 0, unit: rad/s}
        omega max: {value: 30, unit: rad/s}
        tau min: {value: 0.2094395, unit: s}
        tau max: {value: 10, unit: s}
        output Br and K: true
        forward speed correction: true
        calculation point in body frame:
            x: {value: 0.696, unit: m}
            y: {value: 0, unit: m}
            z: {value: 1.418, unit: m}
        """
        RadiationDampingForceModel.parse(valid_yaml, False)

    def test_need_one_of_precal_r_or_hdb(self):
        invalid_yaml = """
        model: radiation damping
        type of quadrature for cos transform: simpson
        type of quadrature for convolution: clenshaw-curtis
        nb of points for retardation function discretization: 50
        omega min: {value: 0, unit: rad/s}
        omega max: {value: 30, unit: rad/s}
        tau min: {value: 0.2094395, unit: s}
        tau max: {value: 10, unit: s}
        output Br and K: true
        forward speed correction: true
        calculation point in body frame:
            x: {value: 0.696, unit: m}
            y: {value: 0, unit: m}
            z: {value: 1.418, unit: m}
        """
        expected_msg = (
            "When using the radiation force model, you must use "
            "*either* the 'hdb' key in the YAML file (to read the radiation "
            "damping matrix coefficients from an HDB file) *or* 'raodb' "
            "(if you wish to use the outputs of PRECAL_R): xdyn couldn't "
            "find either in the YAML file"
        )
        with self.assertRaises(InvalidInputException) as pcm:
            RadiationDampingForceModel.parse(invalid_yaml, False)
        self.assertTrue(expected_msg in str(pcm.exception), str(pcm.exception))

    def test_precal_r_filename_is_read_properly(self):
        valid_yaml = """
        model: radiation damping
        raodb: test_ship.ini
        type of quadrature for cos transform: simpson
        type of quadrature for convolution: clenshaw-curtis
        nb of points for retardation function discretization: 50
        omega min: {value: 0, unit: rad/s}
        omega max: {value: 30, unit: rad/s}
        tau min: {value: 0.2094395, unit: s}
        tau max: {value: 10, unit: s}
        output Br and K: true
        forward speed correction: true
        calculation point in body frame:
            x: {value: 0.696, unit: m}
            y: {value: 0, unit: m}
            z: {value: 1.418, unit: m}
        """
        precal_r_filename = RadiationDampingForceModel.parse(
            valid_yaml, False
        ).yaml.precal_r_filename
        self.assertEqual("test_ship.ini", precal_r_filename)

    def test_can_use_data_from_precal_r(self):
        yaml = """
        model: radiation damping
        raodb: data.raodb.ini
        type of quadrature for cos transform: simpson
        type of quadrature for convolution: trapezoidal
        nb of points for retardation function discretization: 50
        omega min: {value: 0, unit: rad/s}
        omega max: {value: 30, unit: rad/s}
        tau min: {value: 0.2094395, unit: s}
        tau max: {value: 10, unit: s}
        output Br and K: false
        forward speed correction: false
        calculation point in body frame:
            x: {value: 0.696, unit: m}
            y: {value: 0, unit: m}
            z: {value: 1.418, unit: m}
        """
        precalr_file = "data.raodb.ini"
        with open(precalr_file, "w", encoding="utf-8") as fid:
            fid.write(precal())
        data = RadiationDampingForceModel.parse(yaml)
        env = EnvironmentAndFrames()
        body_name = self.rng.random_string()()
        F = RadiationDampingForceModel(data, body_name, env)
        states = BodyStates(100)
        states.name = body_name
        T = 10
        record_sine(states, t_start=0, T=T, n_periods=10, n_steps=100)
        Frad = F.get_force(states, 0, env)
        self.assertNotEqual(0, Frad.X())
        os.remove(precalr_file)


if __name__ == "__main__":

    unittest.main()
