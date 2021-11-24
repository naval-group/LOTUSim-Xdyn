"""
Unit test for MMGManeuveringForceModel
"""
import unittest

from xdyn.core import BodyStates, EnvironmentAndFrames
from xdyn.core.io import YamlRotation
from xdyn.force import MMGManeuveringForceModel


def get_input() -> str:
    """Create a YAML input model string"""
    return """
    calculation point in body frame:
        x: {value: -11.1, unit: m}
        y: {value: 0, unit: m}
        z: {value: 0, unit: m}
    Lpp: {value: 320, unit: m}
    T: {value: 20.8, unit: m}
    Xvv: -0.04
    Xrr: 0.011
    Xvr: 0.002
    Xvvvv: 0.771
    Yv: -0.315
    Yr: 0.083
    Yvvv: -1.607
    Yrvv: 0.379
    Yvrr: -0.391
    Yrrr: 0.008
    Nv: -0.137
    Nr: -0.049
    Nvvv: -0.03
    Nrvv: -0.294
    Nvrr: 0.055
    Nrrr: -0.013
    """


def get_states() -> BodyStates:
    """Create a body state variable with one record"""
    states = BodyStates(0)
    states.convention = YamlRotation("angle", ["z", "y'", "x''"])
    states.x.record(0, 0)
    states.y.record(0, 0)
    states.z.record(0, 0)
    states.u.record(0, 1)
    states.v.record(0, 2)
    states.w.record(0, 0)
    states.p.record(0, 0)
    states.q.record(0, 0)
    states.r.record(0, 3)
    states.qr.record(0, 1)
    states.qi.record(0, 0)
    states.qj.record(0, 0)
    states.qr.record(0, 0)
    return states


class MMGManeuveringForceModelTest(unittest.TestCase):
    """Test class for MMGManeuveringForceModel"""

    def test_can_parse(self):
        """Check that parse function produces a valid MMGManeuveringForceModelInput object"""
        data = MMGManeuveringForceModel.parse(get_input())
        self.assertEqual(data.application_point.x, -11.1)
        self.assertEqual(data.application_point.y, 0)
        self.assertEqual(data.application_point.z, 0)
        self.assertEqual(data.Lpp, 320)
        self.assertEqual(data.T, 20.8)
        self.assertEqual(data.Xvv, -0.04)
        self.assertEqual(data.Xrr, 0.011)
        self.assertEqual(data.Xvr, 0.002)
        self.assertEqual(data.Xvvvv, 0.771)
        self.assertEqual(data.Yv, -0.315)
        self.assertEqual(data.Yr, 0.083)
        self.assertEqual(data.Yvvv, -1.607)
        self.assertEqual(data.Yrvv, 0.379)
        self.assertEqual(data.Yvrr, -0.391)
        self.assertEqual(data.Yrrr, 0.008)
        self.assertEqual(data.Nv, -0.137)
        self.assertEqual(data.Nr, -0.049)
        self.assertEqual(data.Nvvv, -0.03)
        self.assertEqual(data.Nrvv, -0.294)
        self.assertEqual(data.Nvrr, 0.055)
        self.assertEqual(data.Nrrr, -0.013)

    def test_example(self):
        """Check the model works fine on a happy test case"""
        env = EnvironmentAndFrames()
        env.rho = 1000
        env.rot = YamlRotation("angle", ["z", "y'", "x''"])
        force_model = MMGManeuveringForceModel(
            MMGManeuveringForceModel.parse(get_input()), "body", env
        )
        states = get_states()
        wrench = force_model.get_force(states, 0, env, {})
        self.assertEqual(wrench.X(), 35918728522.330902)
        self.assertEqual(wrench.Y(), 2003246596640.8945)
        self.assertEqual(wrench.Z(), 0)
        self.assertEqual(wrench.K(), 0)
        self.assertEqual(wrench.M(), 0)
        self.assertEqual(wrench.N(), -455883858001617.88)


if __name__ == "__main__":

    unittest.main()
