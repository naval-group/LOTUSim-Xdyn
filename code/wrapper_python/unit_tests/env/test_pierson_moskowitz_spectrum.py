"""
Unit test for PiersonMoskowitzSpectrum
"""
import unittest

import numpy as np

from xdyn.env import JonswapSpectrum, PiersonMoskowitzSpectrum
from xdyn.exceptions import InvalidInputException
from xdyn.ssc.random import DataGenerator

NB_TRIALS: int = 10


class PiersonMoskowitzSpectrumTest(unittest.TestCase):
    """Test class for PiersonMoskowitzSpectrum"""

    def setUp(self) -> None:
        self.rng = DataGenerator(666)

    def test_example(self):
        S = PiersonMoskowitzSpectrum(10, 5.0 * np.pi / 8.0)
        eps = 1e-10
        self.assertAlmostEqual(0.004600020635184, S(1.8888888888), delta=eps)
        self.assertAlmostEqual(0.179258995153943, S(2.166666666666667), delta=eps)
        self.assertAlmostEqual(0.955512136348661, S(2.444444444444445), delta=eps)
        self.assertAlmostEqual(2.014925308286375, S(2.722222222222222), delta=eps)
        self.assertAlmostEqual(2.673499618095267, S(3), delta=1e-3)

    def test_Jonswap_reduces_to_Pierson_Moskowitz_for_gamma_equals_1_(self):
        Sjonswap = JonswapSpectrum(4, 8, 1)
        Spierson_moskowitz = PiersonMoskowitzSpectrum(4, 8)
        for _ in range(NB_TRIALS):
            omega = self.rng.random_double().between(0.01, 3)()
            self.assertEqual(Sjonswap(omega), Spierson_moskowitz(omega))

    def test_should_throw_if_Hs_is_negative(self):
        for _ in range(NB_TRIALS):
            Hs = self.rng.random_double().no().greater_than(0)()
            Tp = self.rng.random_double().greater_than(0)()
            expected_msg = "Hs should be greater than or equal to 0: got"
            with self.assertRaises(InvalidInputException) as pcm:
                PiersonMoskowitzSpectrum(Hs, Tp)
            self.assertTrue(expected_msg in str(pcm.exception), str(pcm.exception))

    def test_should_throw_if_Tp_is_negative(self):
        for _ in range(NB_TRIALS):
            Hs = self.rng.random_double().greater_than(0)()
            Tp = self.rng.random_double().no().greater_than(0)()
            expected_msg = "Tp should be greater than 0: got"
            with self.assertRaises(InvalidInputException) as pcm:
                PiersonMoskowitzSpectrum(Hs, Tp)
            self.assertTrue(expected_msg in str(pcm.exception), str(pcm.exception))

    def test_should_throw_if_Tp_is_zero(self):
        for _ in range(NB_TRIALS):
            Hs = self.rng.random_double().greater_than(0)()
            Tp = 0
            expected_msg = "Tp should be greater than 0: got"
            with self.assertRaises(InvalidInputException) as pcm:
                PiersonMoskowitzSpectrum(Hs, Tp)
            self.assertTrue(expected_msg in str(pcm.exception), str(pcm.exception))

    def test_should_not_throw_if_Hs_is_zero(self):
        for _ in range(NB_TRIALS):
            Hs = 0
            Tp = self.rng.random_double().greater_than(0)()
            PiersonMoskowitzSpectrum(Hs, Tp)

    def test_should_throw_if_omega_is_negative(self):
        Hs = self.rng.random_double().greater_than(0)()
        Tp = self.rng.random_double().greater_than(0)()
        S = PiersonMoskowitzSpectrum(Hs, Tp)
        expected_msg = "omega should be greater than 0: got"
        for _ in range(NB_TRIALS):
            omega = self.rng.random_double().no().greater_than(0)()
            with self.assertRaises(InvalidInputException) as pcm:
                S(omega)
            self.assertTrue(expected_msg in str(pcm.exception), str(pcm.exception))

    def test_should_throw_if_omega_is_zero(self):
        Hs = self.rng.random_double().greater_than(0)()
        Tp = self.rng.random_double().greater_than(0)()
        S = PiersonMoskowitzSpectrum(Hs, Tp)
        expected_msg = "omega should be greater than 0: got 0"
        with self.assertRaises(InvalidInputException) as pcm:
            S(0)
        self.assertTrue(expected_msg in str(pcm.exception), str(pcm.exception))


if __name__ == "__main__":

    unittest.main()
