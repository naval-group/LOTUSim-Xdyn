"""
Unit test for discretize
"""
import re
import unittest
from typing import List

import numpy as np
from xdyn.env import (
    Cos2sDirectionalSpreading,
    DiracDirectionalSpreading,
    DiracSpectralDensity,
    DiscreteDirectionalWaveSpectrum,
    JonswapSpectrum,
    Stretching,
    discretize,
)
from xdyn.exceptions import InvalidInputException
from xdyn.ssc.random import DataGenerator

NB_TRIALS: int = 10


class DiscretizeTest(unittest.TestCase):
    """Test class for discretize"""

    def setUp(self) -> None:
        self.rng = DataGenerator(666)

    def random_increasing_vector_of_size(self, n: int) -> List[float]:
        if n <= 0:
            return []
        ret = [0] * n
        ret[0] = self.rng.random_double().between(-1e6, 1e6)()
        for i in range(n):
            increment = self.rng.random_double().between(0, 1e3).but_not(0)()
            ret[i] = ret[i - 1] + increment
        return ret

    def test_example(self):
        Hs = 3
        Tp = 4
        gamma = 1.4
        S = JonswapSpectrum(Hs, Tp, gamma)
        D = Cos2sDirectionalSpreading(np.pi / 4, 2)
        s = Stretching(delta=0, h=0)
        A = discretize(S, D, 0.01, 3, 1000, 1000, s, False)
        self.assertTrue(isinstance(A, DiscreteDirectionalWaveSpectrum))
        self.assertEqual(1000, len(A.Dj))
        self.assertEqual(1000, len(A.Si))

    def test_Dirac_in_frequency(self):
        omega0 = 0.2
        Hs = 3
        S = DiracSpectralDensity(omega0, Hs)
        D = Cos2sDirectionalSpreading(np.pi / 4, 2)
        s = Stretching(delta=0, h=0)
        A = discretize(S, D, 0.01, 3, 1000, 1000, s, False)
        self.assertTrue(isinstance(A, DiscreteDirectionalWaveSpectrum))
        self.assertEqual(1000, len(A.Dj))
        self.assertEqual(1, len(A.Si))
        self.assertEqual(Hs * Hs / 8, A.Si[0])

    def test_Dirac_in_direction(self):
        Hs = 3
        Tp = 4
        gamma = 1.4
        S = JonswapSpectrum(Hs, Tp, gamma)
        D = DiracDirectionalSpreading(np.pi / 4)
        s = Stretching(delta=0, h=0)
        A = discretize(S, D, 0.01, 3, 1000, 1000, s, False)
        self.assertTrue(isinstance(A, DiscreteDirectionalWaveSpectrum))
        self.assertEqual(1, len(A.Dj))
        self.assertEqual(1000, len(A.Si))
        self.assertEqual(1, A.Dj[0])

    def test_should_throw_if_omega_min_is_negative(self):
        Hs = 3
        Tp = 4
        gamma = 1.4
        S = JonswapSpectrum(Hs, Tp, gamma)
        D = DiracDirectionalSpreading(np.pi / 4)
        omega_min = self.rng.random_double().no().greater_than(0)()
        omega_max = self.rng.random_double().greater_than(0)()
        nfreq = self.rng.random_size_t()()
        ndir = self.rng.random_size_t()()
        s = Stretching(delta=0, h=0)
        expected_msg = "should be positive."
        with self.assertRaises(InvalidInputException) as pcm:
            discretize(S, D, omega_min, omega_max, nfreq, ndir, s, False)
        self.assertTrue(expected_msg in str(pcm.exception), str(pcm.exception))

    def test_should_throw_if_omega_min_is_zero(self):
        Hs = 3
        Tp = 4
        gamma = 1.4
        S = JonswapSpectrum(Hs, Tp, gamma)
        D = DiracDirectionalSpreading(np.pi / 4)
        omega_min = 0
        omega_max = self.rng.random_double().greater_than(0)()
        nfreq = self.rng.random_size_t()()
        ndir = self.rng.random_size_t()()
        s = Stretching(delta=0, h=0)
        expected_msg = "should be positive."
        with self.assertRaises(InvalidInputException) as pcm:
            discretize(S, D, omega_min, omega_max, nfreq, ndir, s, False)
        self.assertTrue(expected_msg in str(pcm.exception), str(pcm.exception))

    def test_should_throw_if_omega_max_is_negative(self):
        Hs = 3
        Tp = 4
        gamma = 1.4
        S = JonswapSpectrum(Hs, Tp, gamma)
        D = DiracDirectionalSpreading(np.pi / 4)
        omega_min = self.rng.random_double().greater_than(0)()
        omega_max = self.rng.random_double().no().greater_than(0)()
        nfreq = self.rng.random_size_t()()
        ndir = self.rng.random_size_t()()
        s = Stretching(delta=0, h=0)
        expected_msg = "should be positive."
        with self.assertRaises(InvalidInputException) as pcm:
            discretize(S, D, omega_min, omega_max, nfreq, ndir, s, False)
        self.assertTrue(expected_msg in str(pcm.exception), str(pcm.exception))

    def test_should_throw_if_omega_max_is_zero(self):
        Hs = 3
        Tp = 4
        gamma = 1.4
        S = JonswapSpectrum(Hs, Tp, gamma)
        D = DiracDirectionalSpreading(np.pi / 4)
        omega_min = self.rng.random_double().greater_than(0)()
        omega_max = 0
        nfreq = self.rng.random_size_t()()
        ndir = self.rng.random_size_t()()
        s = Stretching(delta=0, h=0)
        expected_msg = "should be positive."
        with self.assertRaises(InvalidInputException) as pcm:
            discretize(S, D, omega_min, omega_max, nfreq, ndir, s, False)
        self.assertTrue(expected_msg in str(pcm.exception), str(pcm.exception))

    def test_should_throw_if_omega_max_is_lower_than_omega_min(self):
        Hs = 3
        Tp = 4
        gamma = 1.4
        S = JonswapSpectrum(Hs, Tp, gamma)
        D = DiracDirectionalSpreading(np.pi / 4)
        omega_min = self.rng.random_double().greater_than(0)()
        omega_max = self.rng.random_double().no().greater_than(omega_min)()
        nfreq = self.rng.random_size_t()()
        ndir = self.rng.random_size_t()()
        s = Stretching(delta=0, h=0)
        expected_regex = "omega_max.*<.*omega_min"
        with self.assertRaises(InvalidInputException) as pcm:
            discretize(S, D, omega_min, omega_max, nfreq, ndir, s, False)
        self.assertTrue(
            re.search(expected_regex, str(pcm.exception)), str(pcm.exception)
        )

    def test_should_throw_if_nfreq_is_zero(self):
        Hs = 3
        Tp = 4
        gamma = 1.4
        S = JonswapSpectrum(Hs, Tp, gamma)
        D = DiracDirectionalSpreading(np.pi / 4)
        omega_min = self.rng.random_double().greater_than(0)()
        omega_max = self.rng.random_double().greater_than(0)()
        nfreq = 0
        ndir = self.rng.random_size_t()()
        s = Stretching(delta=0, h=0)
        expected_msg = "nfreq == 0"
        with self.assertRaises(InvalidInputException) as pcm:
            discretize(S, D, omega_min, omega_max, nfreq, ndir, s, False)
        self.assertTrue(expected_msg in str(pcm.exception), str(pcm.exception))

    def test_should_throw_if_ndir_is_zero(self):
        Hs = 3
        Tp = 4
        gamma = 1.4
        S = JonswapSpectrum(Hs, Tp, gamma)
        D = DiracDirectionalSpreading(np.pi / 4)
        omega_min = self.rng.random_double().greater_than(0)()
        omega_max = self.rng.random_double().greater_than(omega_min)()
        ndir = 0
        nfreq = self.rng.random_size_t().between(2, 1000)()
        s = Stretching(delta=0, h=0)
        expected_msg = "ndir == 0"
        with self.assertRaises(InvalidInputException) as pcm:
            discretize(S, D, omega_min, omega_max, nfreq, ndir, s, False)
        self.assertTrue(expected_msg in str(pcm.exception), str(pcm.exception))

    def test_should_throw_if_nfreq_is_one_but_omega_min_is_not_omega_max(self):
        Hs = 3
        Tp = 4
        gamma = 1.4
        S = JonswapSpectrum(Hs, Tp, gamma)
        D = DiracDirectionalSpreading(np.pi / 4)
        omega_min = self.rng.random_double().greater_than(0)()
        omega_max = self.rng.random_double().greater_than(0)()
        nfreq = 1
        ndir = self.rng.random_size_t()()
        s = Stretching(delta=0, h=0)
        expected_regex = "Asked for a single frequency \(nfreq = 1\), but omega_min \(=.*\) != omega_max \(=.*\)"
        with self.assertRaises(InvalidInputException) as pcm:
            discretize(S, D, omega_min, omega_max, nfreq, ndir, s, False)
        self.assertTrue(
            re.search(expected_regex, str(pcm.exception)), str(pcm.exception)
        )

    def test_should_throw_if_omega_min_equals_omega_max_but_nfreq_is_not_one(self):
        Hs = 3
        Tp = 4
        gamma = 1.4
        S = JonswapSpectrum(Hs, Tp, gamma)
        D = DiracDirectionalSpreading(np.pi / 4)
        omega_min = self.rng.random_double().greater_than(0)()
        omega_max = omega_min
        nfreq = self.rng.random_size_t().but_not(1)()
        ndir = self.rng.random_size_t()()
        s = Stretching(delta=0, h=0)
        expected_regex = "omega_min = omega_max \(=.*\) but nfreq != 1"
        with self.assertRaises(InvalidInputException) as pcm:
            discretize(S, D, omega_min, omega_max, nfreq, ndir, s, False)
        self.assertTrue(
            re.search(expected_regex, str(pcm.exception)), str(pcm.exception)
        )


if __name__ == "__main__":

    unittest.main()
