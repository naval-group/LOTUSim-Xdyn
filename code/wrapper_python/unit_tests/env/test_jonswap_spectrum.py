"""
Unit test for JonswapSpectrum
"""
import unittest

import numpy as np
from xdyn.env.wave import JonswapSpectrum
from xdyn.exceptions import InvalidInputException
from xdyn.ssc.random import DataGenerator

NB_TRIALS: int = 10


class JonswapSpectrumTest(unittest.TestCase):
    """Test class for JonswapSpectrum"""

    def setUp(self) -> None:
        self.rng = DataGenerator(666)

    def test_example(self):
        EPS = 1e-15
        Hs = 1.0
        Tp = 10.0
        gamma = 3.3
        S = JonswapSpectrum(Hs, Tp, gamma)
        self.assertAlmostEqual(0.001548977510098, S(0.4), delta=EPS)
        self.assertAlmostEqual(0.046145520023689, S(0.5), delta=EPS)
        self.assertAlmostEqual(0.241661465488933, S(0.6), delta=EPS)
        self.assertAlmostEqual(0.144435240716521, S(0.7), delta=EPS)
        self.assertAlmostEqual(0.061449169843241, S(0.8), delta=EPS)

    def test_spectrum_looks_like_the_one_in_DNV_RPC_C205_fig_3_9(self):
        Tp = 8
        Hs = 4
        S_gamma_1 = JonswapSpectrum(Hs, Tp, 1)
        S_gamma_2 = JonswapSpectrum(Hs, Tp, 2)
        S_gamma_5 = JonswapSpectrum(Hs, Tp, 5)
        self.assertLess(1.5, S_gamma_1(0.8))
        self.assertLess(S_gamma_1(0.8), 2)
        self.assertLess(2.5, S_gamma_2(0.8))
        self.assertLess(S_gamma_2(0.8), 3)
        self.assertLess(4.5, S_gamma_5(0.8))
        self.assertLess(S_gamma_5(0.8), 5)
        for _ in range(100):
            omega = self.rng.random_double().between(0, 3).but().outside(0.6, 1.1)()
            self.assertLessEqual(S_gamma_1(omega), 1)
            self.assertLessEqual(S_gamma_2(omega), 1)
            self.assertLessEqual(S_gamma_5(omega), 1)
            self.assertLessEqual(S_gamma_2(omega), S_gamma_1(omega))
            self.assertLessEqual(S_gamma_5(omega), S_gamma_2(omega))

    def test_can_compute_the_wave_number_for_infinite_depth(self):
        Tp = self.rng.random_double().greater_than(0)()
        Hs = self.rng.random_double().greater_than(0)()
        gamma = self.rng.random_double().greater_than(0)()
        S = JonswapSpectrum(Hs, Tp, gamma)
        for i in range(10):
            omega = self.rng.random_double().greater_than(0)()
            self.assertEqual(omega * omega / 9.81, S.get_wave_number(omega))

    def test_can_compute_the_wave_number_for_finite_depth(self):
        Tp = self.rng.random_double().between(1, 100)()
        Hs = self.rng.random_double().between(1, 80)()
        gamma = self.rng.random_double().greater_than(0)()
        S = JonswapSpectrum(Hs, Tp, gamma)
        for h in range(1, 1001):
            omega = self.rng.random_double().between(1, 2)()
            k = S.get_wave_number(omega, h)
            self.assertAlmostEqual(omega * omega, 9.81 * k * np.tanh(k * h), delta=1e-15)

    def test_should_throw_if_gamma_is_negative(self):
        expected_msg = "gamma should be greater than 0: got"
        for _ in range(NB_TRIALS):
            Hs = self.rng.random_double().greater_than(0)()
            Tp = self.rng.random_double().greater_than(0)()
            gamma = self.rng.random_double().no().greater_than(0)()
            with self.assertRaises(InvalidInputException) as pcm:
                JonswapSpectrum(Hs, Tp, gamma)
            self.assertTrue(expected_msg in str(pcm.exception), str(pcm.exception))

    def test_should_throw_if_gamma_is_zero(self):
        expected_msg = "gamma should be greater than 0: got"
        gamma = 0.0
        for _ in range(NB_TRIALS):
            Hs = self.rng.random_double().greater_than(0)()
            Tp = self.rng.random_double().greater_than(0)()
            with self.assertRaises(InvalidInputException) as pcm:
                JonswapSpectrum(Hs, Tp, gamma)
            self.assertTrue(expected_msg in str(pcm.exception), str(pcm.exception))

    def test_should_throw_if_Hs_is_negative(self):
        expected_msg = "Hs should be greater than or equal to 0: got"
        for _ in range(NB_TRIALS):
            Hs = self.rng.random_double().no().greater_than(0)()
            Tp = self.rng.random_double().greater_than(0)()
            gamma = self.rng.random_double().greater_than(0)()
            with self.assertRaises(InvalidInputException) as pcm:
                JonswapSpectrum(Hs, Tp, gamma)
            self.assertTrue(expected_msg in str(pcm.exception), str(pcm.exception))

    def test_should_throw_if_Tp_is_negative(self):
        expected_msg = "Tp should be greater than 0: got"
        for _ in range(NB_TRIALS):
            Hs = self.rng.random_double().greater_than(0)()
            Tp = self.rng.random_double().no().greater_than(0)()
            gamma = self.rng.random_double().greater_than(0)()
            with self.assertRaises(InvalidInputException) as pcm:
                JonswapSpectrum(Hs, Tp, gamma)
            self.assertTrue(expected_msg in str(pcm.exception), str(pcm.exception))

    def test_should_throw_if_Tp_is_zero(self):
        Tp = 0
        expected_msg = "Tp should be greater than 0: got"
        for _ in range(NB_TRIALS):
            Hs = self.rng.random_double().greater_than(0)()
            gamma = self.rng.random_double().greater_than(0)()
            with self.assertRaises(InvalidInputException) as pcm:
                JonswapSpectrum(Hs, Tp, gamma)
            self.assertTrue(expected_msg in str(pcm.exception), str(pcm.exception))

    def test_should_not_throw_if_Hs_is_zero(self):
        Hs = 0
        for _ in range(NB_TRIALS):
            Tp = self.rng.random_double().greater_than(0)()
            gamma = self.rng.random_double().greater_than(0)()
            JonswapSpectrum(Hs, Tp, gamma)

    def test_should_throw_if_omega_is_negative(self):
        Hs = self.rng.random_double().greater_than(0)()
        Tp = self.rng.random_double().greater_than(0)()
        gamma = self.rng.random_double().greater_than(0)()
        S = JonswapSpectrum(Hs, Tp, gamma)
        expected_msg = "omega should be greater than 0: got"
        for _ in range(NB_TRIALS):
            omega = self.rng.random_double().no().greater_than(0)()
            with self.assertRaises(InvalidInputException) as pcm:
                S(omega)
            self.assertTrue(expected_msg in str(pcm.exception), str(pcm.exception))

    def test_should_throw_if_omega_is_zero(self):
        Hs = self.rng.random_double().greater_than(0)()
        Tp = self.rng.random_double().greater_than(0)()
        gamma = self.rng.random_double().greater_than(0)()
        S = JonswapSpectrum(Hs, Tp, gamma)
        expected_msg = "omega should be greater than 0: got 0"
        with self.assertRaises(InvalidInputException) as pcm:
            S(0)
        self.assertTrue(expected_msg in str(pcm.exception), str(pcm.exception))


if __name__ == "__main__":

    unittest.main()
