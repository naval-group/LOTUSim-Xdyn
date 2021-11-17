"""
Unit test for XDyn
"""
import unittest
from pydoc import render_doc

import numpy as np
import xdyn
import xdyn.ssc
from xdyn import (
    BodyStates,
    Dog,
    EnvironmentAndFrames,
    GravityForceModel,
    History,
    Wrench,
    YamlBody,
    call_go,
)
from xdyn.hdbinterpolators import RAOData
from xdyn.ssc.kinematics import EulerAngles as SscEulerAngles
from xdyn.ssc.kinematics import Kinematics as SscKinematics
from xdyn.ssc.kinematics import KinematicTree as SscKinematicTree
from xdyn.ssc.kinematics import Point as SscPoint
from xdyn.ssc.kinematics import Transform as SscTransform
from xdyn.ssc.kinematics import Wrench as SscWrench
from xdyn.ssc.kinematics import coriolis_and_centripetal


class XDynCallTest(unittest.TestCase):
    """Test class for MMGManeuveringForceModel"""

    def test_can_parse(self):
        print(render_doc(xdyn))
        sscki = SscKinematics()

        print(render_doc(coriolis_and_centripetal))
        ssckt = SscKinematicTree()
        ssckt.add("NED", "BODY")
        ssckt.add("BODY", "CRANE")
        print(ssckt.get_path("NED", "CRANE"))

        sscea1 = SscEulerAngles()
        print(sscea1)
        print(sscea1.phi)
        sscea1.phi = 666
        print(sscea1.phi)

        sscpoint1 = SscPoint()
        sscpoint2 = SscPoint("toto")
        sscpoint3 = SscPoint("toto", 1, 2, 3)
        print(sscpoint3.get_frame())
        print(sscpoint3.v)
        sscpoint4 = SscPoint("toto", [7, 8, 9])
        print(sscpoint4.v)

        sscwrench = SscWrench()
        print(sscwrench)
        print("Uninitialized")
        print(sscwrench.force)
        print("Uninitialized")
        print(sscwrench.torque)
        sscwrench.torque = [2, 5, 6]
        print(sscwrench.torque)

        ssctransform1 = SscTransform()
        ssctransform2 = SscTransform(SscPoint("frameA", 1, 2, 3), "frameB")
        ssctransform3 = SscTransform(
            SscPoint("frame1", 4, 5, 6),
            np.array([[1.0, 0.0, 0.0], [0.0, -1.0, 0.0], [0.0, 0.0, -1.0]]),
            "frameB",
        )
        print(ssctransform1.get_from_frame())
        print(ssctransform2.get_from_frame())
        print(ssctransform3.get_from_frame())
        print(xdyn.add(1, 2))
        print(xdyn.subtract(1, 2))

        env = EnvironmentAndFrames()
        print(env)
        print(dir(env))
        env.g = 9.81
        bs = BodyStates(0.0)
        print(bs)
        print(bs.name)
        bs.name = "gh"
        print(bs.name)
        print(bs.x)
        print(bs.y)
        print(bs.z)
        print(bs.x.size())
        bs.x.record(0.0, 0.0)
        bs.x.record(1.0, 42.0)
        print(bs.x.size())
        print(bs.x(0.0))
        print(bs.x[0])
        bs.y.record(0.0, 1.0)
        print(bs.y.size())
        bs.z.record(0.0, 10.0)
        print(bs.z.size())
        print(dir(bs))

        wrench = Wrench(sscwrench)
        print(wrench)
        print(wrench.force)
        print(wrench.torque)

        gfm = GravityForceModel("g", env)
        print(gfm)

        d = Dog()
        result = call_go(d)
        print(result)

        h1 = History()
        h2 = History(0.2)

        raodata = RAOData()
        print(raodata.periods)
        print(raodata.psi)
        print(raodata.values)

        body = YamlBody()
        print(body)
        print(dir(xdyn))


if __name__ == "__main__":

    unittest.main()
