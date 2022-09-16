"""
Unit test for DefaultWindModel
"""
import unittest

import numpy as np
from xdyn.env.wind import DefaultWindModel


class DefaultWindModelTest(unittest.TestCase):
    """Test class for DefaultWindModel"""

    def test_returns_no_wind(self):
        wind_model = DefaultWindModel(0)
        position = np.random.uniform(size=3)
        time = np.random.uniform()
        wind_vector = wind_model.get_wind(position, time)
        self.assertEqual(0.0, wind_vector[0])
        self.assertEqual(0.0, wind_vector[1])
        self.assertEqual(0.0, wind_vector[2])


if __name__ == "__main__":

    unittest.main()
