"""
Unit test for DiffractionInterpolator
"""
import unittest

import numpy as np
from xdyn.core.io import YamlRAO
from xdyn.data.test import bug_3210
from xdyn.hdb import HDBParser, RaoInterpolator


class DiffractionInterpolatorTest(unittest.TestCase):
    """Test class for DiffractionInterpolator"""

    def test_example(self):
        parser = HDBParser.from_string(bug_3210())
        omegas = [4, 64, 125]
        psis = [0, 30]
        omegas = list(reversed([2 * np.pi / omega for omega in omegas]))
        psis = [psi * np.pi / 180.0 for psi in psis]
        yaml = YamlRAO()
        yaml.mirror = True
        radiation = RaoInterpolator(parser, omegas, psis, yaml)
        k = 0  #  X-axis
        modules = radiation.get_modules_cartesian(k)
        phases = radiation.get_phases_cartesian(k)
        self.assertEqual(len(omegas), len(modules))
        self.assertEqual(len(omegas), len(phases))
        for phase in phases:
            self.assertEqual(len(psis), len(phase))
        for module in modules:
            self.assertEqual(len(psis), len(module))
        # First column (because X-axis) of the first group
        # (because incidence index is 0), second line (because period index is 1)
        self.assertAlmostEqual(3.378373e03, modules[1][0], delta=1e-9)
        # First column (because X-axis) of the second group
        # (because incidence index is 1), third line (because period index is 2)
        self.assertAlmostEqual(9.067188e02, modules[2][1], delta=1e-9)
        # First column (because X-axis) of the first group
        # (because incidence index is 0), second line (because period index is 1)
        self.assertAlmostEqual(2.088816e00, phases[1][0], delta=1e-9)
        # First column (because X-axis) of the second group
        # (because incidence index is 1), third line (because period index is 2)
        self.assertAlmostEqual(2.123063e00, phases[2][1], delta=1e-9)


if __name__ == "__main__":

    unittest.main()
