"""
Unit test for HoltropMennenForceModel
"""
import unittest
from typing import Optional

from xdyn.core import BodyStates, EnvironmentAndFrames
from xdyn.core.io import YamlRotation
from xdyn.force import (
    HoltropMennenForceModel,
    HoltropMennenForceModelDerivedData,
    HoltropMennenForceModelInput,
)
from xdyn.ssc.random import DataGenerator


def get_env() -> EnvironmentAndFrames:
    env = EnvironmentAndFrames()
    env.rho = 1025
    env.nu = 1.18e-6
    env.g = 9.81
    return env


def get_steady_forward_speed_states(u: Optional[float] = 0.0) -> BodyStates:
    """Create a body state variable with one record"""
    states = BodyStates()
    states.convention = YamlRotation("angle", ["z", "y'", "x''"])
    states.x.record(0, 0)
    states.y.record(0, 0)
    states.z.record(0, 0)
    states.u.record(0, u)
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


def get_yaml_input_with_optional() -> str:
    return """
    Lwl: {value: 325.5,unit: m}
    Lpp: {value: 320,unit: m}
    B: {value: 58,unit: m}
    Ta: {value: 20.8,unit: m}
    Tf: {value: 20.8,unit: m}
    Vol: {value: 312622,unit: m^3}
    S: {value: 27194, unit: m^2}
    lcb: 3.48
    Abt: {value: 25,unit: m^2}
    hb: {value: 2.5,unit: m}
    Cm: 0.998
    Cwp: 0.83
    At: {value: 0,unit: m^2}
    Sapp: {value: 273.3,unit: m^2}
    Cstern: 0
    1+k2: 2
    apply on ship speed direction: true
    iE: {value: 25, unit: deg}
    1+k1: 1.5
    """


def get_yaml_input_without_optional() -> str:
    return """
    Lwl: {value: 325.5,unit: m}
    Lpp: {value: 320,unit: m}
    B: {value: 58,unit: m}
    Ta: {value: 20.8,unit: m}
    Tf: {value: 20.8,unit: m}
    Vol: {value: 312622,unit: m^3}
    lcb: 3.48
    Abt: {value: 25,unit: m^2}
    hb: {value: 2.5,unit: m}
    Cm: 0.998
    Cwp: 0.83
    At: {value: 0,unit: m^2}
    Sapp: {value: 273.3,unit: m^2}
    Cstern: 0
    1+k2: 2
    """


def get_Holtrop_Mennen_1982_input() -> HoltropMennenForceModelInput:
    data = HoltropMennenForceModelInput()
    data.Lwl = 205
    data.Lpp = 200
    data.B = 32
    data.Ta = 10
    data.Tf = 10
    data.Vol = 37500
    data.lcb = -0.75
    data.Abt = 20
    data.hb = 4
    data.Cm = 0.98
    data.Cwp = 0.75
    data.At = 16
    data.Sapp = 50
    data.Cstern = 10
    data.app_form_coeff = 1.5
    return data


def get_Holtrop_1984_input() -> HoltropMennenForceModelInput:
    data = HoltropMennenForceModelInput()
    data.Lwl = 50
    data.Lpp = 50
    data.B = 12
    data.Ta = 3.3
    data.Tf = 3.1
    data.Vol = 900
    data.lcb = -4.5
    data.S = 584.9
    data.Abt = 0
    data.hb = 2.5
    data.Cm = 0.78
    data.Cwp = 0.8
    data.At = 10
    data.Sapp = 50
    data.Cstern = 0
    data.app_form_coeff = 3
    data.iE = 25
    return data


class HoltropMennenForceModelTest(unittest.TestCase):
    """Test class for HoltropMennenForceModel"""

    def setUp(self) -> None:
        self.rng = DataGenerator(666)
        self.env = get_env()

    def random_double_between(self, low: float = 0.0, high: float = 1.0) -> float:
        return self.rng.random_double().between(low, high)()

    def random_int_between(self, low: int = 0, high: int = 1) -> int:
        return self.rng.random_size_t().between(low, high)()

    def test_can_parse(self):
        data = HoltropMennenForceModel.parse(get_yaml_input_with_optional())
        self.assertEqual(type(data), HoltropMennenForceModelInput)
        self.assertEqual(data.Lwl, 325.5)
        self.assertEqual(data.Lpp, 320)
        self.assertEqual(data.B, 58)
        self.assertEqual(data.Ta, 20.8)
        self.assertEqual(data.Tf, 20.8)
        self.assertEqual(data.Vol, 312622)
        self.assertEqual(data.lcb, 3.48)
        self.assertEqual(data.Abt, 25)
        self.assertEqual(data.hb, 2.5)
        self.assertEqual(data.Cm, 0.998)
        self.assertEqual(data.Cwp, 0.83)
        self.assertEqual(data.At, 0)
        self.assertEqual(data.Sapp, 273.3)
        self.assertEqual(data.Cstern, 0)
        self.assertEqual(data.app_form_coeff, 2)
        self.assertTrue(data.apply_on_ship_speed_direction)
        self.assertEqual(data.S, 27194)
        self.assertEqual(data.iE, 25)
        self.assertEqual(data.hull_form_coeff, 1.5)

    def test_can_ignore_optional_inputs(self):
        data = HoltropMennenForceModel.parse(get_yaml_input_without_optional())
        self.assertEqual(type(data), HoltropMennenForceModelInput)
        self.assertFalse(data.apply_on_ship_speed_direction)
        self.assertIsNone(data.S)
        self.assertIsNone(data.iE)
        self.assertIsNone(data.hull_form_coeff)

    def test_no_resistance_at_zero_speed(self):
        data = get_Holtrop_Mennen_1982_input()
        force_model = HoltropMennenForceModel(data, "body", self.env)
        states = get_steady_forward_speed_states(0)
        self.assertEqual(0, force_model(states, 0, self.env).X())

    def test_numerical_example_1982(self):
        """
        Example from Holtrop & Mennen's original 1982 paper (tolerance is based on the precision given in the paper)
        Note:
        Holtrop & Mennen's original 1982 method was revised by Holtrop in 1984. A number of formulae changed, in particular for the friction and wave components of the force.
        In consequence, some of the checks in this test have rather large margins because the values in the 1982 paper used the early version of the model, but the 1984 version is used in xdyn.
        All the values that have large margins in this test are properly tested with the 1984 test.
        """
        data = get_Holtrop_Mennen_1982_input()
        derived_data = HoltropMennenForceModelDerivedData(data)
        # Checking derived input values
        assert_equal = lambda x, y, delta: self.assertAlmostEqual(x, y, delta=delta)
        assert_equal(0.1561, derived_data.c7, 0.0001)
        assert_equal(0.04, derived_data.c4, 0.01)
        assert_equal(10, derived_data.T, 0.0001)
        assert_equal(0.57164634146341464, derived_data.Cb, 0.0001)
        assert_equal(0.58331259333001484, derived_data.Cp, 0.0001)
        assert_equal(7381.45, derived_data.S, 0.01)
        assert_equal(-1.9412639420954685, derived_data.m3, 0.0001)
        assert_equal(12.08, derived_data.iE, 0.01)
        # Note: there is a sign error for the value of c15 in the original 1982 paper
        assert_equal(-1.69385, derived_data.c15, 0.00001)
        assert_equal(0.6261, derived_data.Pb, 0.0001)
        assert_equal(0.02119, derived_data.c3, 0.00001)
        assert_equal(0.7595, derived_data.c2, 0.0001)
        assert_equal(0.000352, derived_data.Ca, 0.000001)
        assert_equal(1.0289534685942132, derived_data.c17, 0.0001)
        assert_equal(1.380877247519813, derived_data.c16, 0.0001)
        assert_equal(-2.1274, derived_data.m1, 0.0001)
        assert_equal(81.385, derived_data.Lr, 0.001)
        assert_equal(0.6513, derived_data.Lambda, 0.0001)
        assert_equal(1.398, derived_data.c1, 0.001)
        assert_equal(0.9592, derived_data.c5, 0.0001)
        assert_equal(1.11, derived_data.c14, 0.0001)

        force_model = HoltropMennenForceModel(data, "body", self.env)
        states = get_steady_forward_speed_states(25.0 * 1852.0 / 3600.0)
        # Constant intermediate values
        # Large margin because the formulae were revised in 1984
        assert_equal(1.156, derived_data.hull_form_coeff, 0.05)
        # Resulting forces
        # Large margin because the formulae were revised in 1984
        assert_equal(869630, force_model.Rf(states, self.env), 200000)
        assert_equal(8830, force_model.Rapp(states, self.env), 10)
        # Large margin because the formulae were revised in 1984
        assert_equal(557110, force_model.Rw(states, self.env), 5000)
        assert_equal(49, force_model.Rb(states, self.env), 1)
        assert_equal(0.0, force_model.Rtr(states, self.env), 1)
        # Note: there seems to be a rounding error in the original paper (Ra = 221980 N)
        assert_equal(220572, force_model.Ra(states, self.env), 1)
        # large margin to account for all the margins
        assert_equal(1793260, -force_model(states, 0, self.env).X(), 200000)

    def test_numerical_example_1984(self):
        # Example from Holtrop's 1984 paper (revision of the Holtrop-Mennen method)
        data = get_Holtrop_1984_input()
        derived_data = HoltropMennenForceModelDerivedData(data)
        # Checking derived input values
        assert_equal = lambda x, y, delta: self.assertAlmostEqual(x, y, delta=delta)
        assert_equal(0.60096, derived_data.Cp, delta=0.00001)
        assert_equal(0.46875, derived_data.Cb, delta=0.00001)
        assert_equal(14.1728, derived_data.Lr, delta=0.0001)
        assert_equal(0.00064, derived_data.Ca, delta=0.00001)
        assert_equal(1.4133, derived_data.c17, delta=0.0001)
        assert_equal(0.7329, derived_data.c5, delta=0.0001)
        assert_equal(-2.0298, derived_data.m3, delta=0.0001)
        assert_equal(0.7440, derived_data.Lambda, delta=0.0001)
        assert_equal(1.0, derived_data.c2, delta=0.1)
        assert_equal(-1.69385, derived_data.c15, delta=0.00001)
        force_model = HoltropMennenForceModel(data, "body", self.env)
        states = BodyStates()
        # Constant intermediate values
        assert_equal(1.297, derived_data.hull_form_coeff, delta=0.001)
        assert_equal(584.9, derived_data.S, delta=0.1)
        # Numerical values given in Holtrop (1982)
        speed_kts = [25, 27, 29, 31, 33, 35]
        Rw = [475000, 512000, 539000, 564000, 590000, 618000]
        Rapp = [21000, 24000, 28000, 31000, 35000, 39000]
        Rtr = [25000, 16000, 2000, 0, 0, 0]
        R = [662000, 715000, 756000, 807000, 864000, 925000]
        for i in range(len(speed_kts)):
            states = get_steady_forward_speed_states(speed_kts[i] * 1852 / 3600)
            assert_equal(force_model.Rw(states, self.env), Rw[i], delta=1000)
            assert_equal(force_model.Rapp(states, self.env), Rapp[i], delta=1000)
            assert_equal(force_model.Rtr(states, self.env), Rtr[i], delta=1000)
            assert_equal(-force_model(states, 0, self.env).X(), R[i], delta=R[i] / 100)


if __name__ == "__main__":

    unittest.main()
