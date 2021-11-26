"""
Unit test for ResistanceCurveForceModel
"""
import unittest
import io
import re
import unittest
from contextlib import redirect_stderr

from xdyn.core import BodyStates, EnvironmentAndFrames
from xdyn.core.io import YamlFilteredStates, YamlRotation
from xdyn.data.yaml import resistance_curve
from xdyn.force import ResistanceCurveForceModel
from xdyn.ssc.random import DataGenerator

EPS: float = 1e-6


def get_states() -> BodyStates:
    """Create a body state variable with one record"""
    states = BodyStates(0)
    states.convention = YamlRotation("angle", ["z", "y'", "x''"])
    states.x.record(0, 0)
    states.y.record(0, 0)
    states.z.record(0, 0)
    states.u.record(0, 0)
    states.v.record(0, 0)
    states.w.record(0, 0)
    states.p.record(0, 0)
    states.q.record(0, 0)
    states.r.record(0, 0)
    states.qr.record(0, 1)
    states.qi.record(0, 0)
    states.qj.record(0, 0)
    states.qr.record(0, 0)
    return states


class ResistanceCurveForceModelTest(unittest.TestCase):
    """Test class for ResistanceCurveForceModel"""

    def setUp(self) -> None:
        self.rng = DataGenerator(666)

    def test_parser(self):
        r = ResistanceCurveForceModel.parse(resistance_curve())
        self.assertEqual(8, len(r.R))
        self.assertEqual(8, len(r.Va))

        knot = 1852 / 3600
        self.assertEqual(0, r.Va[0])
        self.assertEqual(1 * knot, r.Va[1])
        self.assertEqual(2 * knot, r.Va[2])
        self.assertEqual(3 * knot, r.Va[3])
        self.assertEqual(4 * knot, r.Va[4])
        self.assertEqual(5 * knot, r.Va[5])
        self.assertEqual(15 * knot, r.Va[6])
        self.assertEqual(20 * knot, r.Va[7])

        self.assertEqual(0, r.R[0])
        self.assertEqual(1e6, r.R[1])
        self.assertEqual(4e6, r.R[2])
        self.assertEqual(9e6, r.R[3])
        self.assertEqual(16e6, r.R[4])
        self.assertEqual(25e6, r.R[5])
        self.assertEqual(225e6, r.R[6])
        self.assertEqual(400e6, r.R[7])

    def test_example(self):
        env = EnvironmentAndFrames()
        model = ResistanceCurveForceModel(
            ResistanceCurveForceModel.parse(resistance_curve()), "", env
        )
        self.assertEqual("resistance curve", model.model_name())
        states = get_states()
        states.u.record(0, 0)
        force = model.get_force(states, self.rng.random_double()(), env, {})
        self.assertEqual(0, force.X())
        self.assertEqual(0, force.Y())
        self.assertEqual(0, force.Z())
        self.assertEqual(0, force.K())
        self.assertEqual(0, force.M())
        self.assertEqual(0, force.N())

        states.u.record(1, 1852 / 3600)
        force = model.get_force(states, self.rng.random_double()(), env, {})
        self.assertAlmostEqual(-1e6, force.X(), delta=EPS)
        self.assertEqual(0, force.Y())
        self.assertEqual(0, force.Z())
        self.assertEqual(0, force.K())
        self.assertEqual(0, force.M())
        self.assertEqual(0, force.N())

        states.u.record(2, 1852 / 3600 * 15)
        force = model.get_force(states, self.rng.random_double()(), env, {})
        self.assertAlmostEqual(-225e6, force.X(), delta=EPS)
        self.assertEqual(0, force.Y())
        self.assertEqual(0, force.Z())
        self.assertEqual(0, force.K())
        self.assertEqual(0, force.M())
        self.assertEqual(0, force.N())

    def test_should_issue_a_warning_when_speed_is_lower_than_min_speed_specified_in_resistance_curve_table(self):
        env = EnvironmentAndFrames()
        model= ResistanceCurveForceModel(ResistanceCurveForceModel.parse(resistance_curve()), "", env)
        states = get_states()
        states.u.record(0, -1)
        buf = io.StringIO()
        with redirect_stderr(buf):
            model.get_force(states, self.rng.random_double()(), env, {})
        expected_regex = "Warning: resistance curve is tabulated from .* m/s, but received Va = .* m/s"
        self.assertTrue(re.search(expected_regex, buf.getvalue()), buf.getvalue())

    def test_should_issue_a_warning_when_speed_is_greater_than_max_speed_specified_in_resistance_curve_table(self):
        env = EnvironmentAndFrames()
        model = ResistanceCurveForceModel(ResistanceCurveForceModel.parse(resistance_curve()), "", env)
        states = get_states()
        states.u.record(0, 20*1852/3600 + 1E-10)
        buf = io.StringIO()
        with redirect_stderr(buf):
            model.get_force(states, self.rng.random_double()(), env, {})
        expected_regex = "Warning: resistance curve is tabulated up to .* m/s, but received Va = .* m/s"
        self.assertTrue(re.search(expected_regex, buf.getvalue()), buf.getvalue())

    def test_with_filtered_states(self):
        env = EnvironmentAndFrames()
        model = ResistanceCurveForceModel(
            ResistanceCurveForceModel.parse(resistance_curve()), "", env
        )
        filters = YamlFilteredStates()
        filters.u = "type of filter: moving average\n" "duration in seconds : 9"
        states = BodyStates(filters, 9)
        states.convention = YamlRotation("angle", ["z", "y'", "x''"])
        knot = 1852.0 / 3600.0
        for i in range(10):
            states.u.record(i, i * knot)
        force = model.get_force(states, self.rng.random_double()(), env, {})
        average_speed_over_nine_seconds_in_knots = 4.5
        interpolated_resistance = average_speed_over_nine_seconds_in_knots**2
        MN = 1e6  # 1 mega Newton
        self.assertAlmostEqual(-interpolated_resistance * MN, force.X(), delta=1e-8)
        self.assertEqual(0, force.Y())
        self.assertEqual(0, force.Z())
        self.assertEqual(0, force.K())
        self.assertEqual(0, force.M())
        self.assertEqual(0, force.N())


if __name__ == "__main__":

    unittest.main()
