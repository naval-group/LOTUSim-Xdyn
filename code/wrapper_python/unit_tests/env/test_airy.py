"""
Unit test for Airy
"""
import unittest

import numpy as np
from xdyn.env.wave import (
    Airy,
    BretschneiderSpectrum,
    Cos2sDirectionalSpreading,
    DiracDirectionalSpreading,
    DiracSpectralDensity,
    DiscreteDirectionalWaveSpectrum,
    FlatDiscreteDirectionalWaveSpectrum,
    Stretching,
    SumOfWaveSpectralDensities,
    discretize,
)
from xdyn.ssc.kinematics import PointMatrix as SscPointMatrix
from xdyn.ssc.random import DataGenerator

EPS: float = 1e-10
BIG_EPS: float = 1e-4


class AiryTest(unittest.TestCase):
    """Test class for Airy"""

    def setUp(self) -> None:
        self.rng = DataGenerator(666)

    def test_single_frequency_single_direction_at_one_point(self):
        psi0 = np.pi / 4
        Hs = 3
        Tp = 10
        omega0 = 2 * np.pi / Tp
        omega_min = self.rng.random_double().greater_than(0)()
        omega_max = self.rng.random_double().greater_than(omega_min)()
        nfreq = self.rng.random_size_t().between(2, 100)()
        ndir = self.rng.random_size_t().between(2, 100)()
        s = Stretching(delta=1, h=0)
        A = discretize(
            DiracSpectralDensity(omega0, Hs),
            DiracDirectionalSpreading(psi0),
            omega_min,
            omega_max,
            nfreq,
            ndir,
            s,
            False,
        )
        random_seed = 0
        wave = Airy(A, random_seed)
        x = [self.rng.random_double()()]
        y = [self.rng.random_double()()]
        phi = 3.4482969340598712549
        k = 4.0 * np.pi * np.pi / Tp / Tp / 9.81
        for t in np.arange(0, 3 * Tp, 0.1):
            self.assertAlmostEqual(
                -Hs
                / 2
                * np.sin(
                    -2 * np.pi / Tp * t
                    + k * (x[0] * np.cos(psi0) + y[0] * np.sin(psi0))
                    + phi
                ),
                wave.get_elevation(x, y, t)[0],
                delta=1e-6,
            )

    def test_serialized_spectrum_should_have_all_the_information_we_need(self):
        psi0 = np.pi / 4
        Hs = 3
        Tp = 10
        omega0 = 2 * np.pi / Tp
        omega_min = self.rng.random_double().greater_than(0)()
        omega_max = self.rng.random_double().greater_than(omega_min)()
        nfreq = self.rng.random_size_t().between(2, 100)()
        ndir = self.rng.random_size_t().between(2, 100)()
        s = Stretching(delta=1.0, h=0.0)
        A = discretize(
            DiracSpectralDensity(omega0, Hs),
            DiracDirectionalSpreading(psi0),
            omega_min,
            omega_max,
            nfreq,
            ndir,
            s,
            False,
        )
        self.assertEqual(type(A), DiscreteDirectionalWaveSpectrum)
        random_seed = 0
        wave = Airy(A, random_seed)
        serialized_spectrum = wave.get_spectrum()
        self.assertEqual(1, len(serialized_spectrum.phase))
        for phase in serialized_spectrum.phase:
            self.assertEqual(1, len(phase))
            self.assertNotEqual(0, phase[0])

    def test_two_frequencies_single_direction_at_one_point(self):
        psi0 = np.pi / 4
        Hs = 3
        Tp = 10
        omega0 = 2 * np.pi / Tp
        omega_min = 0.01
        omega_max = 10
        nfreq = 50
        ndir = self.rng.random_size_t().between(2, 100)()
        s = Stretching(h=0, delta=1)
        S = DiracSpectralDensity(omega0, Hs) + DiracSpectralDensity(omega0, Hs)
        D = DiracDirectionalSpreading(psi0)
        A = discretize(S, D, omega_min, omega_max, nfreq, ndir, s)
        self.assertTrue(type(A), DiscreteDirectionalWaveSpectrum)
        wave = Airy(spectrum=A, random_number_generator_seed=0)
        x = [self.rng.random_double()()]
        y = [self.rng.random_double()()]
        phi = 3.4482969340598712549
        k = 4.0 * np.pi * np.pi / Tp / Tp / 9.81
        kx_plus_phi = k * (x[0] * np.cos(psi0) + y[0] * np.sin(psi0)) + phi
        ref_eta = lambda t: -Hs / np.sqrt(2) * np.sin(-2 * np.pi / Tp * t + kx_plus_phi)
        assert_equal = lambda x, y: self.assertAlmostEqual(x, y, delta=1e-6)
        for t in np.arange(0.0, 3 * Tp, 0.1):
            assert_equal(ref_eta(t), wave.get_elevation(x, y, t)[0])

    def test_one_frequency_two_directions_at_one_point(self):
        psi0 = np.pi / 4
        Hs = 3
        Tp = 10
        omega0 = 2 * np.pi / Tp
        omega_min = 0.01
        omega_max = 10
        nfreq = 50
        ndir = 50
        s = Stretching(h=0, delta=1)
        S = DiracSpectralDensity(omega0, Hs)
        D = DiracDirectionalSpreading(psi0) + DiracDirectionalSpreading(psi0)
        A = discretize(S, D, omega_min, omega_max, nfreq, ndir, s)
        self.assertEqual(type(A), DiscreteDirectionalWaveSpectrum)
        wave = Airy(spectrum=A, random_number_generator_seed=0)
        x = [self.rng.random_double()()]
        y = [self.rng.random_double()()]
        phi = 3.4482969340598712549
        k = 4.0 * np.pi * np.pi / Tp / Tp / 9.81
        ref_eta = (
            lambda t: -Hs
            / np.sqrt(2)
            * np.sin(
                -2 * np.pi / Tp * t
                + k * (x[0] * np.cos(psi0) + y[0] * np.sin(psi0))
                + phi
            )
        )
        assert_equal = lambda x, y: self.assertAlmostEqual(x, y, delta=1e-6)
        for t in np.arange(0.0, 3 * Tp, 0.1):
            assert_equal(ref_eta(t), wave.get_elevation(x, y, t)[0])

    def test_bug(self):
        psi0 = np.pi
        Hs = 2
        Tp = 7
        s = 1
        omega_min = 0.1
        omega_max = 6
        nfreq = 101
        ndir = 101
        ss = Stretching(h=0, delta=1)
        S = BretschneiderSpectrum(Hs, Tp)
        D = Cos2sDirectionalSpreading(psi0, s)
        A = discretize(S, D, omega_min, omega_max, nfreq, ndir, ss)
        self.assertEqual(type(A), DiscreteDirectionalWaveSpectrum)
        wave = Airy(spectrum=A, random_number_generator_seed=0)
        x = [0.0]
        y = [0.0]
        t = 0
        self.assertLess(abs(wave.get_elevation(x, y, t)[0]), 5)

    def test_dynamic_pressure(self):
        psi0 = np.pi / 4
        Hs = 3
        Tp = 10
        omega0 = 2 * np.pi / Tp
        omega_min = self.rng.random_double().greater_than(0)()
        omega_max = self.rng.random_double().greater_than(omega_min)()
        nfreq = self.rng.random_size_t().between(2, 100)()
        ndir = self.rng.random_size_t().between(2, 100)()
        ss = Stretching(h=0, delta=1)
        S = DiracSpectralDensity(omega0, Hs)
        D = DiracDirectionalSpreading(psi0)
        A = discretize(S, D, omega_min, omega_max, nfreq, ndir, ss)
        self.assertEqual(type(A), DiscreteDirectionalWaveSpectrum)
        wave = Airy(spectrum=A, random_number_generator_seed=0)

        x = [self.rng.random_double().between(-100, 100)()]
        y = [self.rng.random_double().between(-100, 100)()]
        z = [self.rng.random_double().between(0, 100)()]
        eta = [0]
        phi = 3.4482969340598712549
        rho = 1024
        g = 9.81
        k = 4.0 * np.pi * np.pi / Tp / Tp / g
        ref_dynamic_pressure = (
            lambda t: Hs
            / 2
            * rho
            * g
            * np.exp(-k * z[0])
            * np.sin(
                -2 * np.pi / Tp * t
                + k * (x[0] * np.cos(psi0) + y[0] * np.sin(psi0))
                + phi
            )
        )
        assert_equal = lambda x, y: self.assertAlmostEqual(x, y, delta=1e-6)
        for t in np.arange(0.0, 3 * Tp, 0.1):
            assert_equal(
                ref_dynamic_pressure(t),
                wave.get_dynamic_pressure(rho, g, x, y, z, eta, t)[0],
            )

    def test_validate_elevation_formula_against_results_from_sos(self):
        Hs = 0.1
        Tp = 5
        omega0 = 2 * np.pi / Tp
        psi = 0
        phi = 5.8268
        t = 0
        g = 9.81
        k = omega0 * omega0 / g
        get_eta = (
            lambda x, y: Hs
            / 2
            * np.cos(omega0 * t - k * (x * np.cos(psi) + y * np.sin(psi)) + phi)
        )
        assert_equal = lambda x, y: self.assertAlmostEqual(x, y, delta=BIG_EPS)
        assert_equal(0.045232, get_eta(x=-0.1, y=+0.0))
        assert_equal(0.044522, get_eta(x=+0.1, y=+0.0))
        assert_equal(0.044883, get_eta(x=+0.0, y=-0.1))
        assert_equal(0.044883, get_eta(x=+0.0, y=+0.1))
        assert_equal(0.044883, get_eta(x=+0.0, y=+0.0))

    def test_should_be_able_to_reproduce_results_from_sos_stab(self):
        Hs = 0.1
        Tp = 5
        omega0 = 2 * np.pi / Tp
        psi = 0
        phi = 5.8268
        t = 0
        omega_min = self.rng.random_double().greater_than(0)()
        omega_max = self.rng.random_double().greater_than(omega_min)()
        nfreq = self.rng.random_size_t().between(2, 100)()
        ndir = self.rng.random_size_t().between(2, 100)()
        ss = Stretching(h=0, delta=1)
        S = DiracSpectralDensity(omega0, Hs)
        D = DiracDirectionalSpreading(psi)
        A = discretize(S, D, omega_min, omega_max, nfreq, ndir, ss)
        self.assertEqual(type(A), DiscreteDirectionalWaveSpectrum)
        wave = Airy(spectrum=A, constant_random_phase=phi)
        get_eta = lambda x, y: wave.get_elevation([x], [y], t)[0]
        assert_eta = lambda x, y, expected_eta: self.assertAlmostEqual(
            expected_eta, get_eta(x, y), delta=BIG_EPS
        )
        assert_eta(x=-0.1, y=+0.0, expected_eta=+0.022754911240680714)
        assert_eta(x=+0.1, y=+0.0, expected_eta=+0.021310005003521912)
        assert_eta(x=+0.0, y=-0.1, expected_eta=+0.022035312958930367)
        assert_eta(x=+0.0, y=+0.1, expected_eta=+0.022035312958930367)
        assert_eta(x=+0.0, y=+0.0, expected_eta=+0.022035312958930367)

    def test_dynamic_pressure_compare_with_sos_stab(self):
        Hs = 0.1
        Tp = 5
        omega0 = 2 * np.pi / Tp
        psi = 0
        phi = 5.8268
        t = 0
        rho = 1025
        g = 9.81
        omega_min = self.rng.random_double().greater_than(0)()
        omega_max = self.rng.random_double().greater_than(omega_min)()
        nfreq = self.rng.random_size_t().between(2, 100)()
        ndir = self.rng.random_size_t().between(2, 100)()
        ss = Stretching(h=0, delta=1)
        S = DiracSpectralDensity(omega0, Hs)
        D = DiracDirectionalSpreading(psi)
        A = discretize(S, D, omega_min, omega_max, nfreq, ndir, ss)
        self.assertEqual(type(A), DiscreteDirectionalWaveSpectrum)
        wave = Airy(spectrum=A, constant_random_phase=phi)

        x = [-0.1, 0.1, 0, 0, 0, 0]
        y = [0, 0, -0.1, 0.1, 0, 0]
        z = [0.2, 0.2, 0.2, 0.2, 0.1, 0.3]

        eta = wave.get_elevation(x, y, t)
        pdyn = wave.get_dynamic_pressure(rho, g, x, y, z, eta, t)
        assert_equal = lambda x, y: self.assertAlmostEqual(x, y, delta=BIG_EPS)
        assert_equal(-0.022033996863949721, pdyn[0] / rho / g)
        assert_equal(-0.020634867719409668, pdyn[1] / rho / g)
        assert_equal(-0.021337196682411571, pdyn[2] / rho / g)
        assert_equal(-0.021337196682411571, pdyn[3] / rho / g)
        assert_equal(-0.021683445449540293, pdyn[4] / rho / g)
        assert_equal(-0.020996476935521684, pdyn[5] / rho / g)

    def test_norm_of_orbital_velocity_should_only_depend_on_z(self):
        Hs = 0.1
        Tp = 5
        omega0 = 2 * np.pi / Tp
        psi = 0
        phi = 5.8268
        t = 0
        g = 9.81

        omega_min = self.rng.random_double().greater_than(0)()
        omega_max = self.rng.random_double().greater_than(omega_min)()
        nfreq = self.rng.random_size_t().between(2, 100)()
        ndir = self.rng.random_size_t().between(2, 100)()
        ss = Stretching(h=0, delta=1)
        S = DiracSpectralDensity(omega0, Hs)
        D = DiracDirectionalSpreading(psi)
        A = discretize(S, D, omega_min, omega_max, nfreq, ndir, ss)
        self.assertEqual(type(A), DiscreteDirectionalWaveSpectrum)
        wave = Airy(spectrum=A, constant_random_phase=phi)
        x = [0]
        y = [0]
        z = [100]
        eta = [0]

        V = wave.get_orbital_velocity(g, x, y, z, t, eta)
        self.assertEqual(type(V), SscPointMatrix)
        self.assertEqual(V.get_frame(), "NED")
        ref = np.linalg.norm(V.m)

        for _ in range(100):
            x[0] = self.rng.random_double().between(-100, 100)()
            y[0] = self.rng.random_double().between(-100, 100)()
            V = wave.get_orbital_velocity(g, x, y, z, t, eta)
            norm = np.linalg.norm(V.m)
            self.assertAlmostEqual(ref, norm, delta=EPS)

    def test_orbital_velocity_non_regression_test(self):
        Hs = 0.1
        Tp = 5
        omega0 = 2 * np.pi / Tp
        psi = np.pi / 3
        phi = 5.8268
        t = 0
        g = 9.81

        omega_min = self.rng.random_double().greater_than(0)()
        omega_max = self.rng.random_double().greater_than(omega_min)()
        nfreq = self.rng.random_size_t().between(2, 100)()
        ndir = self.rng.random_size_t().between(2, 100)()
        ss = Stretching(h=0, delta=1)
        S = DiracSpectralDensity(omega0, Hs)
        D = DiracDirectionalSpreading(psi)
        A = discretize(S, D, omega_min, omega_max, nfreq, ndir, ss)
        self.assertEqual(type(A), DiscreteDirectionalWaveSpectrum)
        wave = Airy(spectrum=A, constant_random_phase=phi)
        x = [-0.1, 0.1, 0, 0, 0, 0]
        y = [0, 0, -0.1, 0.1, 0, 0]
        z = [0.2, 0.2, 0.2, 0.2, 0.1, 0.3]
        eta = [0, 0, 0, 0, 0, 0]

        V = wave.get_orbital_velocity(g, x, y, z, t, eta)
        self.assertEqual(type(V), SscPointMatrix)
        self.assertEqual("NED", V.get_frame())
        assert_equal = lambda x, y: self.assertAlmostEqual(x, y, delta=EPS)
        assert_equal(-0.013625903643418017, V.m[0, 0])
        assert_equal(-0.023600757409437876, V.m[1, 0])
        assert_equal(+0.054396641584981066, V.m[2, 0])
        assert_equal(-0.013186340021821508, V.m[0, 1])
        assert_equal(-0.022839410883673743, V.m[1, 1])
        assert_equal(+0.054828253332012813, V.m[2, 1])
        assert_equal(-0.013785918416543868, V.m[0, 2])
        assert_equal(-0.02387791112645346, V.m[1, 2])
        assert_equal(+0.054235130955620707, V.m[2, 2])
        assert_equal(-0.013024588330772734, V.m[0, 3])
        assert_equal(-0.022559248736567082, V.m[1, 3])
        assert_equal(+0.054982688288036041, V.m[2, 3])
        assert_equal(-0.013624110585758166, V.m[0, 4])
        assert_equal(-0.023597651742470114, V.m[1, 4])
        assert_equal(+0.055500467086839214, V.m[2, 4])
        assert_equal(-0.013192475538380495, V.m[0, 5])
        assert_equal(-0.022850037910084588, V.m[1, 5])
        assert_equal(+0.053742117681957483, V.m[2, 5])

    def test_orbital_velocity_sanity_check(self):
        Hs = 0.1
        Tp = 5
        omega0 = 2 * np.pi / Tp
        t = 0
        g = 9.81
        omega_min = self.rng.random_double().greater_than(0)()
        omega_max = self.rng.random_double().greater_than(omega_min)()
        nfreq = self.rng.random_size_t().between(2, 100)()
        ndir = self.rng.random_size_t().between(2, 100)()
        psi = self.rng.random_double().between(-np.pi, np.pi)()
        ss = Stretching(h=0, delta=1)
        for _ in range(100):
            S = DiracSpectralDensity(omega0, Hs)
            D = DiracDirectionalSpreading(psi)
            A = discretize(S, D, omega_min, omega_max, nfreq, ndir, ss)
            self.assertEqual(type(A), DiscreteDirectionalWaveSpectrum)
            constant_random_phase = self.rng.random_double().between(-np.pi, np.pi)()
            wave = Airy(spectrum=A, constant_random_phase=constant_random_phase)
            x = [self.rng.random_double().between(-100, 100)()]
            y = [self.rng.random_double().between(-100, 100)()]
            z = [self.rng.random_double().between(2, 5)()]
            eta = [0]
            V = wave.get_orbital_velocity(g, x, y, z, t, eta)
            self.assertAlmostEqual(
                abs(np.cos(psi)),
                abs(V.m[0, 0] / np.hypot(V.m[0, 0], V.m[1, 0])),
                delta=EPS,
            )
            self.assertAlmostEqual(
                abs(np.sin(psi)),
                abs(V.m[1, 0] / np.hypot(V.m[0, 0], V.m[1, 0])),
                delta=EPS,
            )

    def test_orbital_velocity_and_elevation_should_have_opposite_signs(self):
        Hs = 0.1
        Tp = 5
        omega0 = 2 * np.pi / Tp
        t = 0
        g = 9.81
        omega_min = self.rng.random_double().greater_than(0)()
        omega_max = self.rng.random_double().greater_than(omega_min)()
        nfreq = self.rng.random_size_t().between(2, 100)()
        ndir = self.rng.random_size_t().between(2, 100)()
        psi = self.rng.random_double().between(0, np.pi / 4)()
        ss = Stretching(h=0, delta=1)
        for _ in range(100):
            S = DiracSpectralDensity(omega0, Hs)
            D = DiracDirectionalSpreading(psi)
            A = discretize(S, D, omega_min, omega_max, nfreq, ndir, ss)
            self.assertEqual(type(A), DiscreteDirectionalWaveSpectrum)
            constant_random_phase = self.rng.random_double().between(-np.pi, np.pi)()
            wave = Airy(spectrum=A, constant_random_phase=constant_random_phase)
            x = [self.rng.random_double().between(-100, 100)()]
            y = [self.rng.random_double().between(-100, 100)()]
            z = [self.rng.random_double().between(2, 5)()]
            V = wave.get_orbital_velocity(g, x, y, z, t, [0.0])
            eta = wave.get_elevation(x, y, t)
            self.assertLessEqual(V.m[0, 0] * eta[0], 0)
            self.assertLessEqual(V.m[1, 0] * eta[0], 0)

    def test_should_get_different_results_when_using_two_different_spectra(self):
        Hs = 0.1
        Tp = 5
        omega0 = 2 * np.pi / Tp
        psi = 2 * np.pi / 3
        omega_min = 0
        omega_max = 10
        nfreq = 11
        ndir = 11
        s = Stretching(h=0, delta=1)
        S1 = DiracSpectralDensity(omega0, Hs)
        S2 = DiracSpectralDensity(omega0, 10 * Hs)
        D = DiracDirectionalSpreading(psi)
        A1 = discretize(S1, D, omega_min, omega_max, nfreq, ndir, s)
        A2 = discretize(S2, D, omega_min, omega_max, nfreq, ndir, s)
        wave1 = Airy(A1, 0)
        wave2 = Airy(A2, 0)
        rao_module = [
            1
        ]  # {{1,2,3},{4,5,6},{7,8,9},{0,1,2},{3,4,5},{6,7,8},{9,0,1},{2,3,4},{5,6,7},{8,9,0}}
        rao_phase = [
            9
        ]  # {{9,8,7},{6,5,4},{1,4,7},{8,5,2},{7,5,3},{1,5,9},{4,5,6},{7,8,9},{6,5,4},{4,8,6}}

        self.assertGreater(
            abs(
                wave1.evaluate_rao(4, 5, 6, rao_module, rao_phase)
                - wave2.evaluate_rao(4, 5, 6, rao_module, rao_phase)
            ),
            1e-6,
        )

    def test_RAO_non_regression_test(self):
        """
        from math import np.pi, np.cos, np.sin
        phase = 0.0
        Hs = 0.1
        Tp = 5
        omega = 2 * np.pi / Tp
        psi = 2 * np.pi / 3.0
        x = 4
        y = 5
        t = 6
        rao_module = 1
        rao_phase = 9
        np.cos_psi = np.cos(psi)
        np.sin_psi = np.sin(psi)
        k = omega * omega / 9.81
        a = Hs / 2.0
        result = rao_module * a * np.cos(-omega * t + k * (x * np.cos_psi + y * np.sin_psi) + rao_phase + phase)
        """
        Hs = 0.1
        Tp = 5
        omega0 = 2.0 * np.pi / Tp
        psi = 2.0 * np.pi / 3.0
        omega_min = 0
        omega_max = 10
        nfreq = 11
        ndir = 11
        ss = Stretching(h=0, delta=1)
        S = DiracSpectralDensity(omega0, Hs)
        D = DiracDirectionalSpreading(psi)
        A = discretize(S, D, omega_min, omega_max, nfreq, ndir, ss)
        self.assertEqual(type(A), DiscreteDirectionalWaveSpectrum)
        wave = Airy(spectrum=A, constant_random_phase=0.0)
        rao_module = [
            1
        ]  # {{1,2,3},{4,5,6},{7,8,9},{0,1,2},{3,4,5},{6,7,8},{9,0,1},{2,3,4},{5,6,7},{8,9,0}}
        rao_phase = [
            9
        ]  # {{9,8,7},{6,5,4},{1,4,7},{8,5,2},{7,5,3},{1,5,9},{4,5,6},{7,8,9},{6,5,4},{4,8,6}}
        self.assertAlmostEqual(
            -0.048261600164461897,
            wave.evaluate_rao(4, 5, 6, rao_module, rao_phase),
            delta=1e-6,
        )

    def test_should_respect_dirac_inputs(self):
        Hs = 0.1
        Tp = 5
        omega0 = 2.0 * np.pi / Tp
        psi = 2.0 * np.pi / 3.0
        omega_min = 0
        omega_max = 10
        nfreq = 11
        ndir = 11
        ss = Stretching()
        S = DiracSpectralDensity(omega0, Hs)
        D = DiracDirectionalSpreading(psi)
        A = discretize(S, D, omega_min, omega_max, nfreq, ndir, ss)
        self.assertEqual(type(A), DiscreteDirectionalWaveSpectrum)
        wave = Airy(A, 0.0)
        s = wave.get_flat_spectrum()
        self.assertEqual(type(s), FlatDiscreteDirectionalWaveSpectrum)
        self.assertEqual(1, len(s.a))
        self.assertEqual(1, len(s.omega))
        self.assertEqual(1, len(s.psi))
        self.assertEqual(1, len(s.cos_psi))
        self.assertEqual(1, len(s.sin_psi))
        self.assertEqual(1, len(s.k))
        self.assertEqual(1, len(s.phase))
        self.assertAlmostEqual(Hs / 2.0, s.a[0], delta=1e-10)
        self.assertAlmostEqual(omega0, s.omega[0], delta=1e-10)
        self.assertAlmostEqual(psi, s.psi[0], delta=1e-10)
        self.assertAlmostEqual(np.cos(psi), s.cos_psi[0], delta=1e-10)
        self.assertAlmostEqual(np.sin(psi), s.sin_psi[0], delta=1e-10)
        self.assertAlmostEqual(omega0 * omega0 / 9.81, s.k[0], delta=1e-10)
        self.assertAlmostEqual(0.0, s.phase[0], delta=1e-10)

    def test_orbital_velocities_and_dynamic_pressure_should_decrease_with_depth_in_finite_depth(
        self,
    ):
        Hs = 1
        Tp = 5
        psi = 0
        omega0 = 2 * np.pi / Tp
        g = 9.81
        rho = 1000
        omega_min = 0.1
        omega_max = 10
        nfreq = 100
        ndir = 100
        stretching = Stretching(h=0, delta=1)
        h = 40
        t = 0
        S = DiracSpectralDensity(omega0, Hs)
        D = DiracDirectionalSpreading(psi)
        A = discretize(
            S,
            D,
            omega_min,
            omega_max,
            nfreq,
            ndir,
            stretching,
            equal_energy_bins=False,
            h=h,
        )
        self.assertEqual(type(A), DiscreteDirectionalWaveSpectrum)
        random_phase = self.rng.random_double().between(-np.pi, np.pi)()
        wave = Airy(spectrum=A, constant_random_phase=random_phase)
        k = S.get_wave_number(omega0, h)

        x = [self.rng.random_double().between(-10, 10)()] * 2
        y = [self.rng.random_double().between(-10, 10)()] * 2
        z = [1, 30]

        eta = wave.get_elevation(x, y, t)
        V = wave.get_orbital_velocity(g, x, y, z, t, eta)
        pdyn = wave.get_dynamic_pressure(rho, g, x, y, z, eta, t)

        eps = 1e-6

        self.assertAlmostEqual(
            np.cosh(k * (h - z[0])) / np.cosh(k * (h - z[1])),
            (V.m[0, 0]) / (V.m[0, 1]),
            delta=eps,
        )
        self.assertAlmostEqual(
            np.cosh(k * (h - z[0])) / np.cosh(k * (h - z[1])),
            pdyn[0] / pdyn[1],
            delta=eps,
        )
        self.assertLess(0, rho * g * z[0] + pdyn[0])
        self.assertLess(0, rho * g * z[1] + pdyn[1])
        self.assertLess(abs(pdyn[1]), abs(pdyn[0]))
        self.assertLessEqual(abs(V.m[0, 1]), abs(V.m[0, 0]))
        self.assertLessEqual(abs(V.m[1, 1]), abs(V.m[1, 0]))
        self.assertLessEqual(abs(V.m[2, 1]), abs(V.m[2, 0]))
        self.assertAlmostEqual(
            np.sinh(k * (h - z[0])) / np.sinh(k * (h - z[1])),
            (V.m[2, 0]) / (V.m[2, 1]),
            delta=eps,
        )

    def test_orbital_velocities_and_dynamic_pressure_should_decrease_with_depth_in_infinite_depth(
        self,
    ):
        Hs = 3
        Tp = 5
        psi = 0
        omega0 = 2 * np.pi / Tp
        g = 9.81
        rho = 1000
        omega_min = 0.1
        omega_max = 10
        nfreq = 100
        ndir = 100
        stretching = Stretching(h=0, delta=1)
        t = 0
        S = DiracSpectralDensity(omega0, Hs)
        D = DiracDirectionalSpreading(psi)
        A = discretize(S, D, omega_min, omega_max, nfreq, ndir, stretching)
        self.assertEqual(type(A), DiscreteDirectionalWaveSpectrum)
        random_phase = self.rng.random_double().between(-np.pi, np.pi)()
        wave = Airy(spectrum=A, constant_random_phase=random_phase)
        k = S.get_wave_number(omega0)
        x = [self.rng.random_double().between(-10, 10)()] * 2
        y = [self.rng.random_double().between(-10, 10)()] * 2
        z = [4, 30]
        eta = wave.get_elevation(x, y, t)
        V = wave.get_orbital_velocity(g, x, y, z, t, eta)
        pdyn = wave.get_dynamic_pressure(rho, g, x, y, z, eta, t)
        ptot1 = rho * g * z[0] + pdyn[0]
        ptot2 = rho * g * z[1] + pdyn[1]

        eps = 1e-6

        self.assertLess(ptot1, ptot2)
        self.assertLess(0, ptot1)
        self.assertLess(0, ptot2)
        self.assertLess(abs(pdyn[1]), abs(pdyn[0]))
        self.assertAlmostEqual(
            np.exp(-k * z[0]) / np.exp(-k * z[1]), (V.m[0, 0]) / (V.m[0, 1]), delta=eps
        )
        self.assertAlmostEqual(
            np.exp(-k * z[0]) / np.exp(-k * z[1]), pdyn[0] / pdyn[1], delta=eps
        )
        self.assertLessEqual(abs(V.m[0, 1]), abs(V.m[0, 0]))
        self.assertLessEqual(abs(V.m[1, 1]), abs(V.m[1, 0]))
        self.assertLessEqual(abs(V.m[2, 1]), abs(V.m[2, 0]))
        self.assertAlmostEqual(
            np.exp(-k * z[0]) / np.exp(-k * z[1]), (V.m[2, 0]) / (V.m[2, 1]), delta=eps
        )

    def test_total_pressure_should_always_be_positive_in_finite_depth(self):
        Hs = 3
        Tp = 5
        psi = 0
        omega0 = 2 * np.pi / Tp
        g = 9.81
        rho = 1000

        omega_min = 0.1
        omega_max = 10.0
        nfreq = 100
        ndir = 100
        stretching = Stretching(h=0, delta=1)
        h = 40
        t = 0
        S = DiracSpectralDensity(omega0, Hs)
        D = DiracDirectionalSpreading(psi)
        A = discretize(S, D, omega_min, omega_max, nfreq, ndir, stretching, False, h)
        self.assertEqual(type(A), DiscreteDirectionalWaveSpectrum)
        random_phase = self.rng.random_double().between(-np.pi, np.pi)()
        wave = Airy(spectrum=A, constant_random_phase=random_phase)
        n = 100
        for i in range(n):
            x = [self.rng.random_double().between(-100, 100)()]
            y = [self.rng.random_double().between(-100, 100)()]
            eta = wave.get_elevation(x, y, t)
            z = [eta[0] + (h - eta[0]) * i / (n - 1)]
            self.assertLessEqual(
                0,
                rho * g * z[0] + wave.get_dynamic_pressure(rho, g, x, y, z, eta, t)[0],
            )

    def test_total_pressure_should_always_be_positive_in_infinite_depth(self):
        Hs = 3
        Tp = 5
        psi = 0
        omega0 = 2 * np.pi / Tp
        g = 9.81
        rho = 1000

        omega_min = 0.1
        omega_max = 10
        nfreq = 100
        ndir = 100
        stretching = Stretching(h=0, delta=1)
        S = DiracSpectralDensity(omega0, Hs)
        D = DiracDirectionalSpreading(psi)
        A = discretize(S, D, omega_min, omega_max, nfreq, ndir, stretching)
        self.assertEqual(type(A), DiscreteDirectionalWaveSpectrum)
        random_phase = self.rng.random_double().between(-np.pi, np.pi)()
        wave = Airy(spectrum=A, constant_random_phase=random_phase)

        t = 0
        n = 100
        for i in range(n):
            x = [self.rng.random_double().between(-100, 100)()]
            y = [self.rng.random_double().between(-100, 100)()]
            eta = wave.get_elevation(x, y, t)
            z = [eta[0] + (10000 - eta[0]) * i / (n - 1)]
            self.assertLessEqual(
                0,
                rho * g * z[0] + wave.get_dynamic_pressure(rho, g, x, y, z, eta, t)[0],
            )

    def test_dynamic_pressure_and_orbital_velocities_should_be_0_outside_water_in_finite_depth(
        self,
    ):
        Hs = 3
        Tp = 5
        psi = 0
        omega0 = 2 * np.pi / Tp
        g = 9.81
        rho = 1000
        omega_min = 0.1
        omega_max = 10
        nfreq = 100
        ndir = 100
        stretching = Stretching(h=0, delta=1)
        t = 0
        S = DiracSpectralDensity(omega0, Hs)
        D = DiracDirectionalSpreading(psi)
        h = 40
        A = discretize(S, D, omega_min, omega_max, nfreq, ndir, stretching, False, h)
        self.assertEqual(type(A), DiscreteDirectionalWaveSpectrum)
        random_phase = self.rng.random_double().between(-np.pi, np.pi)()
        wave = Airy(spectrum=A, constant_random_phase=random_phase)

        for _ in range(100):
            x = [self.rng.random_double().between(-100, 100)()] * 3
            y = [self.rng.random_double().between(-100, 100)()] * 3
            eta = wave.get_elevation(x, y, t)
            z = [eta[0] - 0.1, h + 0.1, h]
            pdyn = wave.get_dynamic_pressure(rho, g, x, y, z, eta, t)
            V = wave.get_orbital_velocity(g, x, y, z, t, eta)
            self.assertTrue(type(V), SscPointMatrix)
            self.assertEqual(0, pdyn[0])
            self.assertEqual(0, pdyn[1])
            self.assertEqual(0, np.linalg.norm(V.m[:, 0]))
            self.assertEqual(0, np.linalg.norm(V.m[:, 1]))
            self.assertEqual(0, V.m[2, 2])  # Sea bed is impervious

    def test_get_dynamic_pressure_and_orbital_velocities_should_be_0_above_water_in_infinite_depth(
        self,
    ):
        Hs = 3
        Tp = 5
        psi = 0
        omega0 = 2 * np.pi / Tp
        g = 9.81
        rho = 1000
        omega_min = 0.1
        omega_max = 10
        nfreq = 100
        ndir = 100
        stretching = Stretching(h=0, delta=1)
        t = 0
        S = DiracSpectralDensity(omega0, Hs)
        D = DiracDirectionalSpreading(psi)
        A = discretize(S, D, omega_min, omega_max, nfreq, ndir, stretching)
        self.assertEqual(type(A), DiscreteDirectionalWaveSpectrum)
        random_phase = self.rng.random_double().between(-np.pi, np.pi)()
        wave = Airy(spectrum=A, constant_random_phase=random_phase)

        for _ in range(100):
            x = [self.rng.random_double().between(-100, 100)()]
            y = [self.rng.random_double().between(-100, 100)()]
            eta = wave.get_elevation(x, y, t)
            z = [eta[0] - 0.1]
            self.assertEqual(0, wave.get_dynamic_pressure(rho, g, x, y, z, eta, t)[0])
            self.assertEqual(
                0, np.linalg.norm(wave.get_orbital_velocity(g, x, y, z, t, eta).m[:, 0])
            )

    def test_additional_should_be_able_to_add_various_spectrum(self):
        Hs0 = 3
        Tp0 = 10
        omega0 = 2 * np.pi / Tp0
        Hs = 2
        Tp = 7
        res = BretschneiderSpectrum(Hs, Tp) + BretschneiderSpectrum(Hs, Tp)
        self.assertEqual(type(res), SumOfWaveSpectralDensities)
        res = DiracSpectralDensity(omega0, Hs0) + DiracSpectralDensity(omega0, Hs0)
        self.assertEqual(type(res), SumOfWaveSpectralDensities)
        # NOT WORKING
        # res =  BretschneiderSpectrum(Hs, Tp) + DiracSpectralDensity(omega0, Hs0)


if __name__ == "__main__":

    unittest.main()
