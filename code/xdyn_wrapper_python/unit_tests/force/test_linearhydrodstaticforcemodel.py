"""
Unit test for LinearHydrostaticForceModel
"""
import unittest

import numpy as np
from xdyn.data.yaml import linear_hydrostatics
from xdyn.force import LinearHydrostaticForceModel


class LinearHydrostaticForceModelTest(unittest.TestCase):
    """Test class for LinearHydrostaticForceModel"""

    def test_parser(self):
        data = LinearHydrostaticForceModel.parse(linear_hydrostatics())
        self.assertEqual(-2, data.z_eq)
        self.assertAlmostEqual(1 * np.pi / 180.0, data.theta_eq, delta=1e-12)
        self.assertAlmostEqual(-3 * np.pi / 180.0, data.phi_eq, delta=1e-12)
        self.assertTrue(np.allclose(data.K1, [1, 0, 0]))
        self.assertTrue(np.allclose(data.K2, [0, 1, 0]))
        self.assertTrue(np.allclose(data.K3, [0, 0, 1]))


if __name__ == "__main__":

    unittest.main()
