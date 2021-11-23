"""
Unit test for Airy
"""
import unittest

import numpy as np

from xdyn import YamlStretching
from xdyn.env import (
    Airy,
    DiracDirectionalSpreading,
    DiracSpectralDensity,
    DiscreteDirectionalWaveSpectrum,
    Stretching,
    discretize,
)


class AiryTest(unittest.TestCase):
    """Test class for Airy"""

    def setUp(self) -> None:
        self.rng = np.random.default_rng(666)

    def test_single_frequency_single_direction_at_one_point(self):
        psi0 = np.pi / 4
        Hs = 3
        Tp = 10
        omega0 = 2 * np.pi / Tp
        omega_min = self.rng.uniform(low=0.0, high=1e6)
        omega_max = self.rng.uniform(low=omega_min, high=2e6)
        nfreq = self.rng.integers(low=2, high=100)
        ndir = self.rng.integers(low=2, high=100)
        ys = YamlStretching()
        ys.delta = 1
        ys.h = 0
        s = Stretching(ys)
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

        x = [self.rng.uniform()]
        y = [self.rng.uniform()]
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
        omega_min = self.rng.uniform(low=0.0, high=1e6)
        omega_max = self.rng.uniform(low=omega_min, high=2e6)
        nfreq = self.rng.integers(low=2, high=100)
        ndir = self.rng.integers(low=2, high=100)
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
        pass

    def test_one_frequency_two_directions_at_one_point(self):
        pass

    def test_bug(self):
        pass

    def test_dynamic_pressure(self):
        pass

    def test_validate_elevation_formula_against_results_from_sos(self):
        pass

    def test_should_be_able_to_reproduce_results_from_sos_stab(self):
        pass

    def test_dynamic_pressure_compare_with_sos_stab(self):
        pass

    def test_norm_of_orbital_velocity_should_only_depend_on_z(self):
        pass

    def test_orbital_velocity_non_regression_test(self):
        pass

    def test_orbital_velocity_sanity_check(self):
        pass

    def test_orbital_velocity_and_elevation_should_have_opposite_signs(self):
        pass

    def test_should_get_different_results_when_using_two_different_spectra(self):
        pass

    def test_RAO_non_regression_test(self):
        pass

    def test_should_respect_dirac_inputs(self):
        pass

    def test_orbital_velocities_and_dynamic_pressure_should_decrease_with_depth_in_finite_depth(
        self,
    ):
        pass

    def test_orbital_velocities_and_dynamic_pressure_should_decrease_with_depth_in_infinite_depth(
        self,
    ):
        pass

    def test_total_pressure_should_always_be_positive_in_finite_depth(self):
        pass

    def test_total_pressure_should_always_be_positive_in_infinite_depth(self):
        pass

    def test_dynamic_pressure_and_orbital_velocities_should_be_0_outside_water_in_finite_depth(
        self,
    ):
        pass

    def test_get_dynamic_pressure_and_orbital_velocities_should_be_0_above_water_in_infinite_depth(
        self,
    ):
        pass
