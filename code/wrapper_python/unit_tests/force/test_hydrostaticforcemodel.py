"""
Unit test for FastHydrostaticForceModel
"""
import unittest
from typing import List
import numpy as np

from xdyn.core import (
    BlockedDOF,
    BodyBuilder,
    BodyPtr,
    BodyWithSurfaceForces,
    EnvironmentAndFrames,
)
from xdyn.core.io import YamlFilteredStates, YamlRotation
from xdyn.force import FastHydrostaticForceModel
from xdyn.ssc.kinematics import Point as SscPoint
from xdyn.ssc.kinematics import Transform as SscTransform

BODY: str = "body 1"


def get_body(name: str, points) -> BodyPtr:
    rot = YamlRotation("angle", ["z", "y'", "x''"])
    builder = BodyBuilder(rot)
    return builder.build_for_test(name, points, 0, 0.0, rot, 0.0, False)


def get_environment_and_frames() -> EnvironmentAndFrames:
    env = EnvironmentAndFrames()
    env.g = 9.81
    env.rho = 1024
    env.rot = YamlRotation("angle", ["z", "y'", "x''"])
    env.k_add(SscTransform(SscPoint("NED"), "mesh(" + BODY + ")"))
    env.k_add(SscTransform(SscPoint("NED"), BODY))
    env.set_w_as_default_surface_elevation()
    return env


def get_points() -> List[List[np.ndarray]]:
    points = [
        [
            np.array([0.0, 0.0, 0.0]),
            np.array([0.0, 4.0, 0.0]),
            np.array([0.0, 2.0, 2.0]),
        ],
        [
            np.array([0.0, 0.0, 0.0]),
            np.array([0.0, 2.0, -1.0]),
            np.array([0.0, 4.0, 0.0]),
        ],
    ]
    return points


class HydrostaticForceModelTest(unittest.TestCase):
    """Test class for HydrostaticForceModel"""

    def test_example(self):
        env = get_environment_and_frames()
        points = get_points()
        states = get_body(BODY, points).get_states()
        states.G = SscPoint("NED", 0, 2, 2.0 / 3.0)
        body = BodyWithSurfaceForces(states, 0, BlockedDOF(""), YamlFilteredStates())
        F = FastHydrostaticForceModel(BODY, env)
        self.assertEqual("non-linear hydrostatic (fast)", F.model_name())
        t = 42
        body.update_intersection_with_free_surface(env, t)
        Fhs = F(body.get_states(), t, env)
        dz = 2.0 / 3
        dS = 4
        self.assertEqual(-env.rho * env.g * dz * dS, Fhs.X())
        self.assertEqual(0, Fhs.Y())
        self.assertEqual(0, Fhs.Z())
        self.assertEqual(0, Fhs.K())
        self.assertEqual(0, Fhs.M())
        self.assertEqual(0, Fhs.N())


if __name__ == "__main__":

    unittest.main()
