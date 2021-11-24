"""
Unit test for DiracDirectionalSpreading
"""
import unittest

from xdyn.env import DiracDirectionalSpreading
from xdyn.ssc.random import DataGenerator

NB_TRIALS: int = 10


class DiracDirectionalSpreadingTest(unittest.TestCase):
    """Test class for DiracDirectionalSpreading"""

    def setUp(self) -> None:
        self.rng = DataGenerator(666)

    def test_example(self):
        theta0 = self.rng.random_double()()
        D = DiracDirectionalSpreading(theta0)
        self.assertEqual(1, D(theta0))
        for _ in range(1000):
            theta = self.rng.random_double().but_not(theta0)()
            self.assertEqual(0, D(theta))


if __name__ == "__main__":

    unittest.main()
