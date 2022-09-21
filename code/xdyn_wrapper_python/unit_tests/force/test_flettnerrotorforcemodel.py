"""
Unit test for AeroPolarForceModel
"""
import io
import re
import unittest
from contextlib import redirect_stderr
from typing import List, Optional

import numpy as np
from xdyn.core import BodyStates, EnvironmentAndFrames, Wrench
from xdyn.core.io import YamlCoordinates, YamlRotation
from xdyn.env.wind import WindMeanVelocityProfileInput
from xdyn.force import FlettnerRotorForceModel, FlettnerRotorForceModelInput


def get_dummy_sr() -> List[float]:
    return [
        0,
        0.0606060606060606,
        0.121212121212121,
        0.181818181818182,
        0.242424242424242,
        0.303030303030303,
        0.363636363636364,
        0.424242424242424,
        0.484848484848485,
        0.545454545454545,
        0.606060606060606,
        0.666666666666667,
        0.727272727272727,
        0.787878787878788,
        0.848484848484849,
        0.909090909090909,
        0.96969696969697,
        1.03030303030303,
        1.09090909090909,
        1.15151515151515,
        1.21212121212121,
        1.27272727272727,
        1.33333333333333,
        1.39393939393939,
        1.45454545454545,
        1.51515151515152,
        1.57575757575758,
        1.63636363636364,
        1.6969696969697,
        1.75757575757576,
        1.81818181818182,
        1.87878787878788,
        1.93939393939394,
        2,
        2.06060606060606,
        2.12121212121212,
        2.18181818181818,
        2.24242424242424,
        2.3030303030303,
        2.36363636363636,
        2.42424242424242,
        2.48484848484848,
        2.54545454545455,
        2.60606060606061,
        2.66666666666667,
        2.72727272727273,
        2.78787878787879,
        2.84848484848485,
        2.90909090909091,
        2.96969696969697,
        3.03030303030303,
        3.09090909090909,
        3.15151515151515,
        3.21212121212121,
        3.27272727272727,
        3.33333333333333,
        3.39393939393939,
        3.45454545454545,
        3.51515151515152,
        3.57575757575758,
        3.63636363636364,
        3.6969696969697,
        3.75757575757576,
        3.81818181818182,
        3.87878787878788,
        3.93939393939394,
        4,
        4.06060606060606,
        4.12121212121212,
        4.18181818181818,
        4.24242424242424,
        4.3030303030303,
        4.36363636363636,
        4.42424242424242,
        4.48484848484848,
        4.54545454545455,
        4.60606060606061,
        4.66666666666667,
        4.72727272727273,
        4.78787878787879,
        4.84848484848485,
        4.90909090909091,
        4.96969696969697,
        5.03030303030303,
        5.09090909090909,
        5.15151515151515,
        5.21212121212121,
        5.27272727272727,
        5.33333333333333,
        5.39393939393939,
        5.45454545454545,
        5.51515151515152,
        5.57575757575758,
        5.63636363636364,
        5.6969696969697,
        5.75757575757576,
        5.81818181818182,
        5.87878787878788,
        5.93939393939394,
        6,
    ]


def get_dummy_cl() -> List[float]:
    return [
        0.0105739471309258,
        0.0399367265474693,
        0.072058508529026,
        0.107043505527202,
        0.149670302977324,
        0.202342221841618,
        0.26442905093088,
        0.330431128235237,
        0.3988939100341,
        0.470364075655192,
        0.546007028849051,
        0.628199626342556,
        0.718780838331206,
        0.815985401153217,
        0.914526895680504,
        1.026387948917,
        1.14129196868148,
        1.25284814889886,
        1.36694920777679,
        1.49145460014218,
        1.63184878127778,
        1.79088458583472,
        1.94844933729455,
        2.10829869653572,
        2.27312183835823,
        2.44654156609153,
        2.63132563218868,
        2.8254170732857,
        3.02666823743301,
        3.24504065498775,
        3.46550773577052,
        3.68608329042831,
        3.91353199884286,
        4.17215916870612,
        4.44240772695598,
        4.70733654559508,
        4.96746847032953,
        5.23987130144215,
        5.52859282413597,
        5.83198677525646,
        6.13671231922079,
        6.44951242504243,
        6.76860993230993,
        7.07391073798104,
        7.34097007026769,
        7.59080667231868,
        7.84614634372363,
        8.09498917016622,
        8.33115418532565,
        8.55299153583023,
        8.76267860441976,
        8.9630274574074,
        9.15945740015305,
        9.35354491151068,
        9.52489832649205,
        9.68768978276974,
        9.84490260015248,
        9.99471529636744,
        10.1604392100818,
        10.3238245027859,
        10.4731194450617,
        10.6090183210073,
        10.7372288995433,
        10.8618593062491,
        10.986403116858,
        11.1122782007882,
        11.2271496068174,
        11.3298246405184,
        11.430533791124,
        11.5319263018088,
        11.6265852438647,
        11.7086921286499,
        11.7874085674215,
        11.8648008349364,
        11.9390655775186,
        12.005151577277,
        12.0666304795994,
        12.1255337059462,
        12.1826106156416,
        12.235705390107,
        12.2857381309972,
        12.333277366086,
        12.37879800752,
        12.4219034132712,
        12.4627354403117,
        12.5014989683363,
        12.538486451714,
        12.5739778906983,
        12.6083158991274,
        12.6418182151469,
        12.67475443037,
        12.7070255582738,
        12.7384529384734,
        12.7686908542843,
        12.7980082257541,
        12.8265065453304,
        12.8542068194257,
        12.8807642010603,
        12.9066033119997,
        12.9320420006235,
    ]


def get_dummy_cd() -> List[float]:
    return [
        0.707879384618923,
        0.710500577397145,
        0.711897795880358,
        0.712131186056217,
        0.710977016328604,
        0.709349595884967,
        0.707811679105292,
        0.707114115754159,
        0.710480325739258,
        0.721081342427976,
        0.736124953354832,
        0.748336660865343,
        0.756178203516182,
        0.758552458791286,
        0.750680685382373,
        0.723243045928634,
        0.69123313358198,
        0.657949939595037,
        0.623840966183153,
        0.587596738606393,
        0.557491209020001,
        0.54416228553694,
        0.54998686013943,
        0.561632175382373,
        0.575364320462983,
        0.590143895110837,
        0.606122227021072,
        0.625879565537212,
        0.650897547751408,
        0.681529985018002,
        0.717891872181257,
        0.761339474127944,
        0.810603234787223,
        0.860056739461378,
        0.910807453906465,
        0.965654136831177,
        1.02868336223761,
        1.10211354236727,
        1.18169725479842,
        1.26437247594141,
        1.34738331160461,
        1.433314389308,
        1.52173569856991,
        1.61220662172249,
        1.70434269221543,
        1.79916199262558,
        1.89514470573389,
        1.9899472607723,
        2.08737742684354,
        2.18655307136005,
        2.28499120481253,
        2.37522195667073,
        2.46948632906157,
        2.56823617784359,
        2.67010004915205,
        2.76501462875946,
        2.85757014875227,
        2.94988435412899,
        3.04403631934883,
        3.14011921524458,
        3.23675114133649,
        3.33206161095697,
        3.42057718190673,
        3.51211647865884,
        3.60652842412667,
        3.70139186558589,
        3.79243083586837,
        3.88296821771931,
        3.97452174308745,
        4.0677736577733,
        4.16130706795002,
        4.24848768756761,
        4.32403284286364,
        4.39142849491037,
        4.45644511390669,
        4.52039843444385,
        4.58293711610103,
        4.64241983957186,
        4.69863248893836,
        4.75194726851179,
        4.8038570310441,
        4.85502088962883,
        4.90530556831586,
        4.95435934269666,
        5.00198130893857,
        5.04843156478873,
        5.0940604088399,
        5.13946000764902,
        5.18492512240496,
        5.22933064375959,
        5.2715650673331,
        5.31081966899604,
        5.35028784471468,
        5.39117900390024,
        5.43434943829576,
        5.47665932172658,
        5.5174069484744,
        5.55670669672206,
        5.59673221790009,
        5.63754917657541,
    ]


def get_data() -> str:
    """Create a YAML data model string"""
    return f"""
    name: test
    rotor position in body frame:
        x: {{value: 1, unit: m}}
        y: {{value: 2, unit: m}}
        z: {{value: 3, unit: m}}
    diameter: {{value: 5, unit: m}}
    length: {{value: 30, unit: m}}
    spin ratio: [{','.join(map(lambda x: str(x), get_dummy_sr()))}]
    lift coefficient: [{','.join(map(lambda x: str(x), get_dummy_cl()))}]
    drag coefficient: [{','.join(map(lambda x: str(x), get_dummy_cd()))}]
    """


def get_states(forward_speed: Optional[float] = 0.0) -> BodyStates:
    """Create a body state variable with one record"""
    states = BodyStates(0)
    states.convention = YamlRotation("angle", ["z", "y'", "x''"])
    states.x.record(0, 0)
    states.y.record(0, 0)
    states.z.record(0, 0)
    states.u.record(0, forward_speed)
    states.v.record(0, 0)
    states.w.record(0, 0)
    states.p.record(0, 0)
    states.q.record(0, 0)
    states.r.record(0, 0)
    states.qr.record(0, 1)
    states.qi.record(0, 0)
    states.qj.record(0, 0)
    states.qk.record(0, 0)
    return states


class FlettnerRotorForceModelTest(unittest.TestCase):
    """Test class for FlettnerRotorForceModel"""

    def check_zkmn_are_zeros(self, wrench: Wrench):
        self.assertEqual(wrench.Z(), 0.0)
        self.assertEqual(wrench.K(), 0.0)
        self.assertEqual(wrench.M(), 0.0)
        self.assertEqual(wrench.N(), 0.0)

    def check_wrench(self, wrench: Wrench, f_x: float, f_y: float, eps: float = 1e-10):
        self.assertAlmostEqual(wrench.X(), f_x, delta=eps)
        self.assertAlmostEqual(wrench.Y(), f_y, delta=eps)
        self.check_zkmn_are_zeros(wrench)

    def test_can_parse(self):
        data = FlettnerRotorForceModel.parse(get_data())
        self.assertEqual(type(data), FlettnerRotorForceModelInput)
        self.assertEqual(data.name, "test")
        assert_equal = lambda x, y: self.assertAlmostEqual(x, y, delta=1e-12)
        assert_equal(data.diameter, 5)
        assert_equal(data.length, 30)
        assert_equal(data.calculation_point_in_body_frame.x, 1)
        assert_equal(data.calculation_point_in_body_frame.y, 2)
        assert_equal(data.calculation_point_in_body_frame.z, 3)
        vec_sr = get_dummy_sr()
        vec_cl = get_dummy_cl()
        vec_cd = get_dummy_cd()
        self.assertEqual(len(data.spin_ratio), 100)
        self.assertEqual(len(data.lift_coefficient), 100)
        self.assertEqual(len(data.drag_coefficient), 100)
        for i in range(100):
            assert_equal(data.spin_ratio[i], vec_sr[i])
            assert_equal(data.lift_coefficient[i], vec_cl[i])
            assert_equal(data.drag_coefficient[i], vec_cd[i])

    def test_example(self):
        data = FlettnerRotorForceModelInput()
        data.name = "test"
        # Rotor frame is rotated 180° along x-axis to be above water.
        data.calculation_point_in_body_frame = YamlCoordinates(0, 0, 0)
        data.diameter = 5
        data.length = 30
        data.spin_ratio = get_dummy_sr()
        data.lift_coefficient = get_dummy_cl()
        data.drag_coefficient = get_dummy_cd()
        env = EnvironmentAndFrames()
        env.rot = YamlRotation("angle", ["z", "y'", "x''"])
        env.set_rho_air(1.2)
        force_model = FlettnerRotorForceModel(data, "body", env)
        states = get_states(6.0)
        wind_data = WindMeanVelocityProfileInput()
        wind_data.velocity = 10
        # U in [4; 16] m/s so that with 80rpm, the spin ratio stays in [0; 6]
        def check_result(rpm: float, direction_deg: float, f_x: float, f_y: float):
            wind_data.direction = direction_deg * np.pi / 180
            env.set_wind_model(wind_data)
            wrench = force_model.get_force(states, 0, env, {"rpm": rpm})
            self.check_wrench(wrench, f_x, f_y)

        rpm = 80 * 2 * np.pi / 60.0  # Convert 80 rpm to rad/s
        check_result(rpm=rpm, direction_deg=0, f_x=7361.1764011673276, f_y=18075.777282637151)
        check_result(rpm=rpm, direction_deg=45, f_x=-36787.49720659107, f_y=15437.791488833873)
        check_result(rpm=rpm, direction_deg=90, f_x=-39956.556862188991, f_y=-13927.940130287869)
        check_result(rpm=rpm, direction_deg=135, f_x=-30206.149589655892, f_y=-32236.318966192313)
        check_result(rpm=rpm, direction_deg=180, f_x=-12583.849591836341, f_y=-43440.368957916231)
        check_result(rpm=rpm, direction_deg=225, f_x=10452.554605245305, f_y=-42922.439769275981)
        check_result(rpm=rpm, direction_deg=270, f_x=31092.444520695881, f_y=-28701.460699443058)
        check_result(rpm=rpm, direction_deg=315, f_x=39709.160968113858, f_y=-3850.7001942607922)
        check_result(rpm=rpm, direction_deg=355, f_x=11295.30237797498, f_y=16484.557927518184)
        rpm = -80 * 2 * np.pi / 60.0  # -80 rpm in rad/s
        check_result(rpm=rpm, direction_deg=0, f_x=7361.1764011673313, f_y=-18075.777282637151)
        check_result(rpm=rpm, direction_deg=45, f_x=39709.160968113865, f_y=3850.7001942607767)
        check_result(rpm=rpm, direction_deg=90, f_x=31092.444520695884, f_y=28701.460699443051)
        check_result(rpm=rpm, direction_deg=135, f_x=10452.554605245312, f_y=42922.439769275981)
        check_result(rpm=rpm, direction_deg=180, f_x=-12583.849591836333, f_y=43440.368957916231)
        check_result(rpm=rpm, direction_deg=225, f_x=-30206.149589655892, f_y=32236.31896619231)
        check_result(rpm=rpm, direction_deg=270, f_x=-39956.556862188991, f_y=13927.940130287881)
        check_result(rpm=rpm, direction_deg=315, f_x=-36787.497206591077, f_y=-15437.791488833853)
        check_result(rpm=rpm, direction_deg=355, f_x=3334.7095751087259, f_y=-19702.898718850181)

    def test_should_issue_a_warning_when_spin_ratio_out_of_interpolation_bounds(self):
        data = FlettnerRotorForceModelInput()
        data.name = "test"
        # Rotor frame is rotated 180° along x-axis to be over water.
        data.calculation_point_in_body_frame = YamlCoordinates(0, 0, 0)
        data.diameter = 5
        data.length = 30
        # Removing first element to test warning on low SR
        data.spin_ratio = get_dummy_sr()[1:]
        data.lift_coefficient = get_dummy_cl()[1:]
        data.drag_coefficient = get_dummy_cd()[1:]
        env = EnvironmentAndFrames()
        env.rot = YamlRotation("angle", ["z", "y'", "x''"])
        env.set_rho_air(1.2)
        force_model = FlettnerRotorForceModel(data, "body", env)
        states = get_states(6.0)
        wind_data = WindMeanVelocityProfileInput()
        wind_data.velocity = 10
        wind_data.direction = 0 * np.pi / 180
        env.set_wind_model(wind_data)
        # U = 4 m/s
        buf = io.StringIO()
        with redirect_stderr(buf):
            rpm = 200 * 2 * np.pi / 60.0  # 80 rpm
            force_model.get_force(states, 0, env, {"rpm": rpm})
        expected_regex = "Warning: spin ratio in Flettner rotor model 'test' exceeds maximum tabulated value. Saturating at SR=6"
        self.assertTrue(re.search(expected_regex, buf.getvalue()), buf.getvalue())

        buf = io.StringIO()
        with redirect_stderr(buf):
            rpm = 0.01
            force_model.get_force(states, 0, env, {"rpm": rpm})
        expected_regex = "Warning: spin ratio in Flettner rotor model 'test' subceeds minimum tabulated value. Saturating at SR=0.0606060606060606"
        self.assertTrue(re.search(expected_regex, buf.getvalue()), buf.getvalue())


if __name__ == "__main__":

    unittest.main()
