"""
Unit test for Stretching
"""
import unittest

from xdyn.env.wave import Stretching


class StretchingTest(unittest.TestCase):
    """Test class for Stretching"""

    def test_example(self):
        stretching = Stretching(delta=0.4, h=123)
        self.assertEqual(124, stretching.rescaled_z(124, -10))
        self.assertEqual(124, stretching.rescaled_z(124, 10))
        self.assertEqual(124, stretching.rescaled_z(124, 0))
        self.assertEqual(125, stretching.rescaled_z(125, -10))
        self.assertEqual(125, stretching.rescaled_z(125, 10))
        self.assertEqual(125, stretching.rescaled_z(125, 0))
        self.assertEqual(123, stretching.rescaled_z(123, -10))
        self.assertEqual(123, stretching.rescaled_z(123, 10))
        self.assertEqual(123, stretching.rescaled_z(123, 0))
        self.assertEqual(122.04511278195488, stretching.rescaled_z(122, -10))
        self.assertEqual(121.94690265486726, stretching.rescaled_z(122, 10))
        self.assertEqual(122, stretching.rescaled_z(122, 0))
        self.assertEqual(5.5488721804511272, stretching.rescaled_z(0, -10))
        self.assertEqual(-6.5309734513274407, stretching.rescaled_z(0, 10))
        self.assertEqual(0, stretching.rescaled_z(0, 0))
        self.assertEqual(-89.939849624060145, stretching.rescaled_z(-100, -10))
        self.assertEqual(-111.84070796460176, stretching.rescaled_z(-100, 10))
        self.assertEqual(-100, stretching.rescaled_z(-100, 0))


if __name__ == "__main__":

    unittest.main()
