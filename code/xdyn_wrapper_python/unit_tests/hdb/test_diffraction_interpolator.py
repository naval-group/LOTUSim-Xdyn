"""
Unit test for RaoInterpolator
"""
import unittest

import numpy as np
from xdyn.core.io import YamlRAO
from xdyn.data.test import bug_3210
from xdyn.hdb import HDBParser, RaoInterpolator


class RaoInterpolatorTest(unittest.TestCase):
    """Test class for RaoInterpolator"""

    def test_example(self):
        parser = HDBParser.from_string(bug_3210())
        yaml = YamlRAO()
        yaml.mirror = True
        radiation = RaoInterpolator(parser, yaml)
        k = 0  #  X-axis
        module = radiation.interpolate_module(k, 64.0, 0.0)
        self.assertAlmostEqual(3.378373e03, module, delta=1e-9)
        module = radiation.interpolate_module(k, 125.0, 30.0 / 180.0 * np.pi)
        self.assertAlmostEqual(9.067188e02, module, delta=1e-9)
        phase = radiation.interpolate_phase(k, 64.0, 0.0)
        self.assertAlmostEqual(2.088816e00, phase, delta=1e-9)
        phase = radiation.interpolate_phase(k, 125.0, 30.0 / 180.0 * np.pi)
        self.assertAlmostEqual(2.123063e00, phase, delta=1e-9)


if __name__ == "__main__":

    unittest.main()
