"""
Unit test for WindMeanVelocityProfile, UniformWindVelocityProfile
"""
import unittest

import numpy as np
from xdyn.env.wind import (
    LogWindVelocityProfile,
    LogWindVelocityProfileInput,
    PowerLawWindVelocityProfile,
    PowerLawWindVelocityProfileInput,
    UniformWindVelocityProfile,
    WindMeanVelocityProfileInput,
)
from xdyn.exceptions import InvalidInputException
from xdyn.ssc.random import DataGenerator


def get_data() -> str:
    """Create a YAML data model string"""
    return """
    {velocity: {unit: m/s, value: 10},
     direction: {unit: rad, value: 1}}
    """


class UniformWindVelocityProfileTest(unittest.TestCase):
    """Test class for UniformWindVelocityProfile"""

    def setUp(self) -> None:
        self.rng = DataGenerator(666)

    def test_can_parse(self):
        """Check that parse function produces a valid UniformWindVelocityProfiledata object"""
        data = UniformWindVelocityProfile.parse(get_data())
        self.assertEqual(data.velocity, 10)
        self.assertEqual(data.direction, 1)

    def test_UniformWindVelocityProfile_returns_right_value(self):
        """Check the model works fine on a happy test case"""
        data = WindMeanVelocityProfileInput()
        data.velocity = 10
        data.direction = 135 * np.pi / 180
        wind_model = UniformWindVelocityProfile(data)
        position = self.rng.random_vector_of_double().of_size(3)()
        time = self.rng.random_double()()
        wind_vector = wind_model.get_wind(position, time)
        self.assertAlmostEqual(data.velocity * np.cos(data.direction), wind_vector[0], delta=1e-14)
        self.assertAlmostEqual(data.velocity * np.sin(data.direction), wind_vector[1], delta=1e-14)
        self.assertEqual(0.0, wind_vector[2])

    def test_can_parse_PowerLawWindVelocityProfile_data(self):
        yaml_data = """
        {velocity: {unit: m/s, value: 8},
         direction: {unit: rad, value: 1},
         alpha: 0.11,
         reference height: {unit: m, value: 10}}
         """
        data = PowerLawWindVelocityProfile.parse(yaml_data)
        self.assertEqual(data.velocity, 8)
        self.assertEqual(data.direction, 1)
        self.assertEqual(data.z_ref, 10)
        self.assertEqual(data.alpha, 0.11)

    def test_PowerLawWindVelocityProfile_returns_right_value(self):
        data = PowerLawWindVelocityProfileInput()
        data.velocity = 8
        data.direction = 135 * np.pi / 180
        data.alpha = 0.11
        data.z_ref = 10
        wind_model = PowerLawWindVelocityProfile(data)
        position = self.rng.random_vector_of_double().of_size(3)()
        position[2] = -abs(position[2])
        time = self.rng.random_double()()
        wind_vector = wind_model.get_wind(position, time)
        wind_velocity = data.velocity * np.power(-position[2] / data.z_ref, data.alpha)
        self.assertAlmostEqual(wind_velocity * np.cos(data.direction), wind_vector[0], delta=1e-14)
        self.assertAlmostEqual(wind_velocity * np.sin(data.direction), wind_vector[1], delta=1e-14)
        self.assertEqual(0.0, wind_vector[2])

    def test_PowerLawWindVelocityProfile_constructor_should_throw_if_invalid_data(self):
        data = PowerLawWindVelocityProfileInput()
        data.velocity = 8
        data.direction = 135 * np.pi / 180
        data.alpha = 0.11
        data.z_ref = -1
        expected_msg = "the reference height must be strictly positive"
        with self.assertRaises(InvalidInputException) as pcm:
            PowerLawWindVelocityProfile(data)
        self.assertTrue(expected_msg in str(pcm.exception), str(pcm.exception))

    def test_can_parse_LogWindVelocityProfile_data(self):
        yaml_data = """
        {velocity: {unit: m/s, value: 8},
         direction: {unit: rad, value: 1},
         roughness length: {unit: m, value: 0.005},
         reference height: {unit: m, value: 10}}
        """
        data = LogWindVelocityProfile.parse(yaml_data)
        self.assertEqual(data.velocity, 8)
        self.assertEqual(data.direction, 1)
        self.assertEqual(data.z_ref, 10)
        self.assertEqual(data.z0, 0.005)

    def test_LogWindVelocityProfile_returns_right_value(self):
        data = LogWindVelocityProfileInput()
        data.velocity = 8
        data.direction = 135 * np.pi / 180
        data.z0 = 0.005
        data.z_ref = 10
        wind_model = LogWindVelocityProfile(data)
        position = self.rng.random_vector_of_double().of_size(3)()
        position[2] = -position[2]
        time = self.rng.random_double()()
        wind_vector = wind_model.get_wind(position, time)
        wind_velocity = (
            data.velocity * np.log(-position[2] / data.z0) / np.log(data.z_ref / data.z0)
        )
        self.assertAlmostEqual(wind_velocity * np.cos(data.direction), wind_vector[0], delta=1e-14)
        self.assertAlmostEqual(wind_velocity * np.sin(data.direction), wind_vector[1], delta=1e-14)
        self.assertEqual(0.0, wind_vector[2])

    def test_LogWindVelocityProfile_constructor_should_throw_if_invalid_data(self):
        data = LogWindVelocityProfileInput()
        data.velocity = 8
        data.direction = 135 * np.pi / 180
        data.z0 = 0.005
        data.z_ref = -1
        expected_msg = "the reference height must be strictly positive"
        with self.assertRaises(InvalidInputException) as pcm:
            LogWindVelocityProfile(data)
        self.assertTrue(expected_msg in str(pcm.exception), str(pcm.exception))
        data.z0 = -1
        data.z_ref = 10
        expected_msg = "the roughness length must be strictly positive"
        with self.assertRaises(InvalidInputException) as pcm:
            LogWindVelocityProfile(data)
        self.assertTrue(expected_msg in str(pcm.exception), str(pcm.exception))


if __name__ == "__main__":

    unittest.main()
