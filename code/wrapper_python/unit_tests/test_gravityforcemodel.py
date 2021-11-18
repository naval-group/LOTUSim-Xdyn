"""
Unit test for MMGManeuveringForceModel
"""
import unittest

from xdyn import (
    BodyStates,
    EnvironmentAndFrames,
    GravityForceModel,
    YamlRotation,
)
from xdyn.ssc.kinematics import Point as SscPoint
from xdyn.ssc.kinematics import Transform as SscTransform
from xdyn.ssc.kinematics import Kinematics as SscKinematics

# SscKinematics()



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


class GravityForceModelTest(unittest.TestCase):
    """Test class for GravityForceModel"""

    def test_example(self):
        """Check that parse function produces a valid GravityForceModelTest object"""
        body = "body 1"
        env = EnvironmentAndFrames()
        env.g = 9.81
        env.rot = YamlRotation("angle", ["z", "y'", "x''"])
        transform = SscTransform(SscPoint("NED"), body)
        # print(transform)
        # env.k = ssc::kinematics::KinematicsPtr(new ssc::kinematics::Kinematics())
        # env.k->add()
        # env.k = SscKinematics()
        # env.k.add(transform)
        model = GravityForceModel(body, env)
        model_name = model.model_name()
        self.assertEqual("gravity", model_name)
        states = get_states()
        print(states.total_inertia)
        ti = states.get_total_inertia()
        ti[2,2] = 100
        ti2 = states.get_total_inertia()
        print(ti2)
        print(states.total_inertia)

        wrench = model.get_force(states, 0.0, env, {})
        # self.assertEqual(body, wrench.get_frame())
        self.assertEqual(0, wrench.K())
        self.assertEqual(0, wrench.M())
        self.assertEqual(0, wrench.N())
        self.assertEqual(0, wrench.X())
        self.assertEqual(0, wrench.Y())
        # TODO self.assertEqual(981, wrench.Z())



if __name__ == "__main__":

    unittest.main()
