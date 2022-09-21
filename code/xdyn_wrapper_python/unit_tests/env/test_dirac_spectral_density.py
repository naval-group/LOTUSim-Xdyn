"""
Unit test for DiracSpectralDensityTest
"""
import unittest

from xdyn.env.wave import DiracSpectralDensity
from xdyn.exceptions import InvalidInputException
from xdyn.ssc.random import DataGenerator

NB_TRIALS: int = 10


class DiracSpectralDensityTest(unittest.TestCase):
    """Test class for DiracSpectralDensity"""

    def setUp(self) -> None:
        self.rng = DataGenerator(666)

    def test_example(self):
        omega0 = self.rng.random_double().between(0, 200)()
        Hs = self.rng.random_double().between(0, 100)()
        D = DiracSpectralDensity(omega0, Hs)
        self.assertEqual(Hs * Hs / 8, D(omega0))
        for _ in range(NB_TRIALS):
            omega = self.rng.random_double().greater_than(0).but_not(omega0)()
            self.assertEqual(0, D(omega))

    def test_should_throw_if_Hs_is_negative(self):
        for _ in range(NB_TRIALS):
            Hs = self.rng.random_double().no().greater_than(0)()
            omega0 = self.rng.random_double().greater_than(0)()
            expected_msg = "Hs should be greater than or equal to 0: got"
            with self.assertRaises(InvalidInputException) as pcm:
                DiracSpectralDensity(omega0, Hs)
            self.assertTrue(expected_msg in str(pcm.exception), str(pcm.exception))

    def test_should_throw_if_omega0_is_negative(self):
        for _ in range(NB_TRIALS):
            Hs = self.rng.random_double().greater_than(0)()
            omega0 = self.rng.random_double().no().greater_than(0)()
            expected_msg = "omega0 should be greater than 0: got"
            with self.assertRaises(InvalidInputException) as pcm:
                DiracSpectralDensity(omega0, Hs)
            self.assertTrue(expected_msg in str(pcm.exception), str(pcm.exception))

    def test_should_throw_if_omega0_is_zero(self):
        for _ in range(NB_TRIALS):
            Hs = self.rng.random_double().greater_than(0)()
            omega0 = 0
            expected_msg = "omega0 should be greater than 0: got 0"
            with self.assertRaises(InvalidInputException) as pcm:
                DiracSpectralDensity(omega0, Hs)
            self.assertTrue(expected_msg in str(pcm.exception), str(pcm.exception))

    def test_should_not_throw_if_Hs_is_zero(self):
        for _ in range(NB_TRIALS):
            Hs = 0
            omega0 = self.rng.random_double().greater_than(0)()
            DiracSpectralDensity(omega0, Hs)

    def test_should_throw_if_omega_is_negative(self):
        Hs = self.rng.random_double().greater_than(0)()
        omega0 = self.rng.random_double().greater_than(0)()
        S = DiracSpectralDensity(omega0, Hs)
        expected_msg = "omega should be greater than 0: got"
        for _ in range(NB_TRIALS):
            omega = self.rng.random_double().no().greater_than(0)()
            with self.assertRaises(InvalidInputException) as pcm:
                S(omega)
            self.assertTrue(expected_msg in str(pcm.exception), str(pcm.exception))

    def test_should_throw_if_omega_is_zero(self):
        Hs = self.rng.random_double().greater_than(0)()
        omega0 = self.rng.random_double().greater_than(0)()
        S = DiracSpectralDensity(omega0, Hs)
        expected_msg = "omega should be greater than 0: got 0"
        with self.assertRaises(InvalidInputException) as pcm:
            S(0)
        self.assertTrue(expected_msg in str(pcm.exception), str(pcm.exception))


if __name__ == "__main__":

    unittest.main()
