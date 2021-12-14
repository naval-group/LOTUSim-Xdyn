"""
Unit test for XDyn
"""
import os
import unittest

from xdyn.data.yaml import falling_ball_example

import xdyn
from xdyn import ErrorReporter, XdynCommandLineArguments, run


class XDynTest(unittest.TestCase):
    """Test class for XDyn"""

    def test_xdyn_version(self):
        self.assertEqual(xdyn.__version__, "5.0.1")

    def test_xdyn_no_argument(self):
        input_data = XdynCommandLineArguments()
        error_outputter = ErrorReporter()
        res = run(input_data, error_outputter)
        self.assertTrue(res)

    def test_xdyn_falling_ball_example(self):
        yaml_filename = "falling_ball_example.yaml"
        with open(yaml_filename, "w", encoding="utf-8") as fid:
            fid.write(falling_ball_example())
        input_data = XdynCommandLineArguments()
        input_data.solver = "rk4"
        input_data.yaml_filenames = [yaml_filename]
        input_data.initial_timestep = 0.1
        input_data.tstart = 0.0
        input_data.tend = 1.0
        error_outputter = ErrorReporter()
        res = run(input_data, error_outputter)
        self.assertTrue(res)
        with open("falling_ball.csv", "r", encoding="utf-8") as fid:
            lines = fid.readlines()
        self.assertEqual(len(lines), 1 + 11)
        for extension in (".csv", ".json", ".h5"):
            res_filename = f"falling_ball{extension}"
            self.assertTrue(os.path.exists(res_filename))
            os.remove(res_filename)
        os.remove(yaml_filename)


if __name__ == "__main__":

    unittest.main()
