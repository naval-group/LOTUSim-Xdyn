"""
Unit test for BretschneiderSpectrum
"""
import unittest

import numpy as np
from xdyn.env import BretschneiderSpectrum
from xdyn.exceptions import InvalidInputException
from xdyn.ssc.random import DataGenerator

NB_TRIALS: int = 10


class BretschneiderSpectrumTest(unittest.TestCase):
    """Test class for BretschneiderSpectrum"""

    def setUp(self) -> None:
        self.rng = np.random.default_rng(666)
        self.a = DataGenerator(666)
        print(self.a)
        print(dir(self.a))

    def test_example(self):
        EPS = 1e-15
        assert_equal = lambda x, y: self.assertAlmostEqual(x, y, delta=EPS)
        assert_equal(0.034510725921325, BretschneiderSpectrum(1.0, 5.0)(1.0))
        assert_equal(0.138042903685298, BretschneiderSpectrum(2.0, 5.0)(1.0))
        assert_equal(0.000031161194389, BretschneiderSpectrum(2.0, 5.0)(10.0))
        assert_equal(0.276085807370596, BretschneiderSpectrum(2.0, 10.0)(0.5))

#   def test_should_throw_if_Hs_is_negative(self):
#       for _ in range(NB_TRIALS):
#           Hs = a.random < double > ().no().greater_than(0)
#           Tp = a.random < double > ().greater_than(0)
#           expected_msg = "the reference height must be strictly positive"
#           with self.assertRaises(InvalidInputException) as pcm:
#               BretschneiderSpectrum(Hs, Tp)
#           self.assertTrue(expected_msg in str(pcm.exception), str(pcm.exception))
#
#   def test_should_throw_if_Tp_is_negative(self):
#       for _ in range(NB_TRIALS):
#           Hs = a.random < double > ().greater_than(0)
#           Tp = a.random < double > ().no().greater_than(0)
#           expected_msg = "the reference height must be strictly positive"
#           with self.assertRaises(InvalidInputException) as pcm:
#               BretschneiderSpectrum(Hs, Tp)
#           self.assertTrue(expected_msg in str(pcm.exception), str(pcm.exception))
#
#   def test_should_throw_if_Tp_is_zero(self):
#       for _ in range(NB_TRIALS):
#           Hs = a.random < double > ().greater_than(0)
#           Tp = 0
#           expected_msg = "the reference height must be strictly positive"
#           with self.assertRaises(InvalidInputException) as pcm:
#               BretschneiderSpectrum(Hs, Tp)
#           self.assertTrue(expected_msg in str(pcm.exception), str(pcm.exception))
#
#   def test_should_not_throw_if_Hs_is_zero(self):
#       for _ in range(NB_TRIALS):
#           Hs = 0
#           Tp = a.random < double > ().greater_than(0)
#           BretschneiderSpectrum(Hs, Tp)
#
#   def test_should_throw_if_omega_is_negative(self):
#       Hs = a.random < double > ().greater_than(0)
#       Tp = a.random < double > ().greater_than(0)
#       S = BretschneiderSpectrum(Hs, Tp)
#       for _ in range(NB_TRIALS):
#           omega = a.random < double > ().no().greater_than(0)
#           expected_msg = "the reference height must be strictly positive"
#           with self.assertRaises(InvalidInputException) as pcm:
#               S(omega)
#           self.assertTrue(expected_msg in str(pcm.exception), str(pcm.exception))
#
#   def test_should_throw_if_omega_is_zero(self):
#       Hs = a.random < double > ().greater_than(0)
#       Tp = a.random < double > ().greater_than(0)
#       S = BretschneiderSpectrum(Hs, Tp)
#       expected_msg = "the reference height must be strictly positive"
#       with self.assertRaises(InvalidInputException) as pcm:
#           S(0)
#       self.assertTrue(expected_msg in str(pcm.exception), str(pcm.exception))


if __name__ == "__main__":

    unittest.main()
