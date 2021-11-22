"""
Unit test for WageningenControlledForceModel
"""
import unittest

import numpy as np

from xdyn import (
    EnvironmentAndFrames,
    WageningenControlledForceModel,
    WageningenControlledForceModelInput,
    YamlRotation,
)
from xdyn.data.yaml import wageningen
from xdyn.exceptions import InvalidInputException

EPS: float = 1e-2
NB_TRIALS: int = 100


def get_env() -> EnvironmentAndFrames:
    env = EnvironmentAndFrames()
    env.rho = 1024
    env.rot = YamlRotation("angle", ["z", "y'", "x''"])
    return env


class WageningenControlledForceModelTest(unittest.TestCase):
    """Test class for WageningenControlledForceModel"""

    def setUp(self) -> None:
        self.rng = np.random.default_rng(666)

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

    def test_should_throw_if_blade_area_ratio_is_outside_bounds(self):
        data = WageningenControlledForceModel.parse(wageningen())
        expected_msg = "Invalid number of blade area ratio AE_A0 received: expected 0.3 <= AE_A0 <= 1.05 but got AE_A0=0"

        def check_raises(data: WageningenControlledForceModelInput):
            data.blade_area_ratio = self.rng.uniform(low=0.0, high=0.3)
            with self.assertRaises(InvalidInputException) as pcm:
                WageningenControlledForceModel(data, "", get_env())
            self.assertTrue(expected_msg in str(pcm.exception))

        for _ in range(NB_TRIALS):
            data.blade_area_ratio = self.rng.uniform(low=0.0, high=0.3)
            check_raises(data)
            data.blade_area_ratio = self.rng.uniform(low=1.05, high=10)
            check_raises(data)
            data.blade_area_ratio = self.rng.uniform(low=-10, high=0.3)
            check_raises(data)
            data.blade_area_ratio = self.rng.uniform(low=0.3, high=1.05)
            WageningenControlledForceModel(data, "", get_env())


if __name__ == "__main__":

    unittest.main()
