"""
Unit test for Cos2sDirectionalSpreading
"""
import unittest

import numpy as np
from xdyn.env.wave import Cos2sDirectionalSpreading
from xdyn.exceptions import InvalidInputException

NB_TRIALS: int = 10


class Cos2sDirectionalSpreadingTest(unittest.TestCase):
    """Test class for Cos2sDirectionalSpreading"""

    def setUp(self) -> None:
        self.rng = np.random.default_rng(666)

    def test_example(self):
        """
        Generated with Maple:
        G:=(theta0,theta,s)->2^(2*s-1)/Pi*GAMMA(s+1)^2/GAMMA(2*s+1)*np.cos((theta-theta0)/2)^(2*s);
        G(Pi/4, Pi/3, 3);
        G(Pi/4, Pi/3, 4);
        G(Pi/4, Pi/3, 5);
        G(Pi/5, Pi/6, 1);
        G(Pi/5, Pi/6, 2);
        """
        EPS = 1e-15
        assert_equal = lambda x, y: self.assertAlmostEqual(x, y, delta=EPS)
        g_1 = Cos2sDirectionalSpreading(np.pi / 4.0, 3)
        g_2 = Cos2sDirectionalSpreading(np.pi / 4.0, 4)
        g_3 = Cos2sDirectionalSpreading(np.pi / 4.0, 5)
        g_4 = Cos2sDirectionalSpreading(np.pi / 5.0, 1)
        g_5 = Cos2sDirectionalSpreading(np.pi / 5.0, 2)
        assert_equal(
            8 * np.power(np.cos(np.pi / 24.0), 6) / (5.0 * np.pi), g_1(np.pi / 3.0)
        )
        assert_equal(
            64 * np.power(np.cos(np.pi / 24.0), 8) / (35.0 * np.pi), g_2(np.pi / 3.0)
        )
        assert_equal(
            128 * np.power(np.cos(np.pi / 24.0), 10) / (63.0 * np.pi), g_3(np.pi / 3.0)
        )
        assert_equal(np.power(np.cos(np.pi / 60.0), 2) / (np.pi), g_4(np.pi / 6.0))
        assert_equal(
            4 * np.power(np.cos(np.pi / 60.0), 4) / (3.0 * np.pi), g_5(np.pi / 6.0)
        )

    def test_should_throw_if_s_is_negative(self):
        expected_msg = "should be non-negative"
        for _ in range(NB_TRIALS):
            with self.assertRaises(InvalidInputException) as pcm:
                Cos2sDirectionalSpreading(
                    self.rng.uniform(), self.rng.uniform(low=-1e6, high=0.0)
                )
            self.assertTrue(expected_msg in str(pcm.exception), str(pcm.exception))

    def test_should_not_throw_if_s_is_zero(self):
        for _ in range(NB_TRIALS):
            Cos2sDirectionalSpreading(self.rng.uniform(), 0)


if __name__ == "__main__":

    unittest.main()
