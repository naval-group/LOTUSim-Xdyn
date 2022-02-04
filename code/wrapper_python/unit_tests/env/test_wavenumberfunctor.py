"""
Unit test for WaveNumberFunctor
"""
import unittest

from numpy import tanh
from xdyn.env.wave import WaveNumberFunctor


class WaveNumberFunctorTest(unittest.TestCase):
    """Test class for WaveNumberFunctor"""

    def test_example(self):
        assert_near = lambda x, y: self.assertAlmostEqual(x, y, delta=1e-10)
        F1 = WaveNumberFunctor(10, 0.3)
        T1 = F1(10)
        assert_near(98.10 * tanh(100) - 0.9e-1, T1[0])
        assert_near(9.81 * tanh(100) + 981.00 - 981.00 * tanh(100) * tanh(100), T1[1])
        assert_near(
            196.20
            - 196.20 * tanh(100) * tanh(100)
            - 19620.00 * tanh(100) * (1 - tanh(100) * tanh(100)),
            T1[2],
        )
        F2 = WaveNumberFunctor(45, 7)
        T2 = F2(3)
        assert_near(0.2943e2 * tanh(0.135e3) - 0.49e2, T2[0])
        assert_near(
            0.981e1 * tanh(0.135e3) + 0.132435e4 - 0.132435e4 * pow(tanh(0.135e3), 0.2e1),
            T2[1],
        )
        assert_near(
            0.88290e3
            - 0.88290e3 * pow(tanh(0.135e3), 0.2e1)
            - 0.1191915000e6 * tanh(0.135e3) * (0.1e1 - pow(tanh(0.135e3), 0.2e1)),
            T2[2],
        )


if __name__ == "__main__":

    unittest.main()
