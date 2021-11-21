"""
Unit test for WageningenControlledForceModel
"""
import unittest

import numpy as np

from xdyn import WageningenControlledForceModel
from xdyn.data.yaml import wageningen

EPS: float = 1e-2
NB_TRIALS: int = 100


class WageningenControlledForceModelTest(unittest.TestCase):
    """Test class for WageningenControlledForceModel"""

    def test_can_parse(self):
        """Check that parse function produces a valid WageningenControlledForceModelInput data object"""
        data = WageningenControlledForceModel.parse(wageningen())
        self.assertEqual(0.5, data.blade_area_ratio)
        self.assertEqual("port side propeller", data.name)
        self.assertEqual(3, data.number_of_blades)
        self.assertEqual(0, data.position_of_propeller_frame.angle.phi)
        self.assertEqual(
            -10 * np.pi / 180.0, data.position_of_propeller_frame.angle.theta
        )
        self.assertEqual(-1 * np.pi / 180.0, data.position_of_propeller_frame.angle.psi)
        self.assertEqual(-4, data.position_of_propeller_frame.coordinates.x)
        self.assertEqual(-2, data.position_of_propeller_frame.coordinates.y)
        self.assertEqual(2, data.position_of_propeller_frame.coordinates.z)
        self.assertEqual("mesh(body 1)", data.position_of_propeller_frame.frame)
        self.assertEqual(1, data.relative_rotative_efficiency)
        self.assertTrue(data.rotating_clockwise)
        self.assertEqual(0.7, data.thrust_deduction_factor)
        self.assertEqual(0.9, data.wake_coefficient)
        self.assertEqual(2, data.diameter)

    # def test_should_throw_if_blade_area_ratio_is_outside_bounds(self):
    #    data = WageningenControlledForceModel.parse(wageningen())
    #    for i in range(NB_TRIALS):
    #        data.blade_area_ratio = a.random<double>().between(0,0.3)
    #        ASSERT_THROW(WageningenControlledForceModel w(data, "", get_env()), InvalidInputException)
    #        data.blade_area_ratio = a.random<double>().between(1.05,10)
    #        ASSERT_THROW(WageningenControlledForceModel w(data, "", get_env()), InvalidInputException)
    #        data.blade_area_ratio = a.random<double>().outside(0.3,1.05)
    #        ASSERT_THROW(WageningenControlledForceModel w(data, "", get_env()), InvalidInputException)
    #        data.blade_area_ratio = a.random<double>().between(0.3,1.05)
    #        ASSERT_NO_THROW(WageningenControlledForceModel w(data, "", get_env()))


if __name__ == "__main__":

    unittest.main()
