"""
Unit test for discretize
"""
import re
import unittest
from typing import List

import numpy as np
from xdyn.env.wave import (
    Cos2sDirectionalSpreading,
    DiracDirectionalSpreading,
    DiracSpectralDensity,
    DiscreteDirectionalWaveSpectrum,
    FlatDiscreteDirectionalWaveSpectrum,
    JonswapSpectrum,
    Stretching,
    area_curve,
    discretize,
    dynamic_pressure_factor,
    equal_area_abscissae,
    filter_spectrum,
    find_integration_bound_yielding_target_area,
    flatten,
)
from xdyn.exceptions import InvalidInputException
from xdyn.ssc.random import DataGenerator

NB_TRIALS: int = 10
EPS: float = 1e-10


class DiscretizeTest(unittest.TestCase):
    """Test class for discretize"""

    def setUp(self) -> None:
        self.rng = DataGenerator(666)

    def random_increasing_vector_of_size(self, n: int) -> List[float]:
        if n <= 0:
            return []
        ret = [0] * n
        ret[0] = self.rng.random_double().between(-1e6, 1e6)()
        for i in range(n):
            increment = self.rng.random_double().between(0, 1e3).but_not(0)()
            ret[i] = ret[i - 1] + increment
        return ret

    def test_example(self):
        Hs = 3
        Tp = 4
        gamma = 1.4
        S = JonswapSpectrum(Hs, Tp, gamma)
        D = Cos2sDirectionalSpreading(np.pi / 4, 2)
        s = Stretching(delta=0, h=0)
        A = discretize(S, D, 0.01, 3, 1000, 1000, s, False)
        self.assertTrue(isinstance(A, DiscreteDirectionalWaveSpectrum))
        self.assertEqual(1000, len(A.Dj))
        self.assertEqual(1000, len(A.Si))

    def test_Dirac_in_frequency(self):
        omega0 = 0.2
        Hs = 3
        S = DiracSpectralDensity(omega0, Hs)
        D = Cos2sDirectionalSpreading(np.pi / 4, 2)
        s = Stretching(delta=0, h=0)
        A = discretize(S, D, 0.01, 3, 1000, 1000, s, False)
        self.assertTrue(isinstance(A, DiscreteDirectionalWaveSpectrum))
        self.assertEqual(1000, len(A.Dj))
        self.assertEqual(1, len(A.Si))
        self.assertEqual(Hs * Hs / 8, A.Si[0])

    def test_Dirac_in_direction(self):
        Hs = 3
        Tp = 4
        gamma = 1.4
        S = JonswapSpectrum(Hs, Tp, gamma)
        D = DiracDirectionalSpreading(np.pi / 4)
        s = Stretching(delta=0, h=0)
        A = discretize(S, D, 0.01, 3, 1000, 1000, s, False)
        self.assertTrue(isinstance(A, DiscreteDirectionalWaveSpectrum))
        self.assertEqual(1, len(A.Dj))
        self.assertEqual(1000, len(A.Si))
        self.assertEqual(1, A.Dj[0])

    def test_should_throw_if_omega_min_is_negative(self):
        Hs = 3
        Tp = 4
        gamma = 1.4
        S = JonswapSpectrum(Hs, Tp, gamma)
        D = DiracDirectionalSpreading(np.pi / 4)
        omega_min = self.rng.random_double().no().greater_than(0)()
        omega_max = self.rng.random_double().greater_than(0)()
        nfreq = self.rng.random_size_t()()
        ndir = self.rng.random_size_t()()
        s = Stretching(delta=0, h=0)
        expected_msg = "should be positive."
        with self.assertRaises(InvalidInputException) as pcm:
            discretize(S, D, omega_min, omega_max, nfreq, ndir, s, False)
        self.assertTrue(expected_msg in str(pcm.exception), str(pcm.exception))

    def test_should_throw_if_omega_min_is_zero(self):
        Hs = 3
        Tp = 4
        gamma = 1.4
        S = JonswapSpectrum(Hs, Tp, gamma)
        D = DiracDirectionalSpreading(np.pi / 4)
        omega_min = 0
        omega_max = self.rng.random_double().greater_than(0)()
        nfreq = self.rng.random_size_t()()
        ndir = self.rng.random_size_t()()
        s = Stretching(delta=0, h=0)
        expected_msg = "should be positive."
        with self.assertRaises(InvalidInputException) as pcm:
            discretize(S, D, omega_min, omega_max, nfreq, ndir, s, False)
        self.assertTrue(expected_msg in str(pcm.exception), str(pcm.exception))

    def test_should_throw_if_omega_max_is_negative(self):
        Hs = 3
        Tp = 4
        gamma = 1.4
        S = JonswapSpectrum(Hs, Tp, gamma)
        D = DiracDirectionalSpreading(np.pi / 4)
        omega_min = self.rng.random_double().greater_than(0)()
        omega_max = self.rng.random_double().no().greater_than(0)()
        nfreq = self.rng.random_size_t()()
        ndir = self.rng.random_size_t()()
        s = Stretching(delta=0, h=0)
        expected_msg = "should be positive."
        with self.assertRaises(InvalidInputException) as pcm:
            discretize(S, D, omega_min, omega_max, nfreq, ndir, s, False)
        self.assertTrue(expected_msg in str(pcm.exception), str(pcm.exception))

    def test_should_throw_if_omega_max_is_zero(self):
        Hs = 3
        Tp = 4
        gamma = 1.4
        S = JonswapSpectrum(Hs, Tp, gamma)
        D = DiracDirectionalSpreading(np.pi / 4)
        omega_min = self.rng.random_double().greater_than(0)()
        omega_max = 0
        nfreq = self.rng.random_size_t()()
        ndir = self.rng.random_size_t()()
        s = Stretching(delta=0, h=0)
        expected_msg = "should be positive."
        with self.assertRaises(InvalidInputException) as pcm:
            discretize(S, D, omega_min, omega_max, nfreq, ndir, s, False)
        self.assertTrue(expected_msg in str(pcm.exception), str(pcm.exception))

    def test_should_throw_if_omega_max_is_lower_than_omega_min(self):
        Hs = 3
        Tp = 4
        gamma = 1.4
        S = JonswapSpectrum(Hs, Tp, gamma)
        D = DiracDirectionalSpreading(np.pi / 4)
        omega_min = self.rng.random_double().greater_than(0)()
        omega_max = self.rng.random_double().greater_than(0).no().greater_than(omega_min)()
        nfreq = self.rng.random_size_t()()
        ndir = self.rng.random_size_t()()
        s = Stretching(delta=0, h=0)
        expected_regex = "omega_max.*<.*omega_min"
        with self.assertRaises(InvalidInputException) as pcm:
            discretize(S, D, omega_min, omega_max, nfreq, ndir, s, False)
        self.assertTrue(re.search(expected_regex, str(pcm.exception)), str(pcm.exception))

    def test_should_throw_if_nfreq_is_zero(self):
        Hs = 3
        Tp = 4
        gamma = 1.4
        S = JonswapSpectrum(Hs, Tp, gamma)
        D = DiracDirectionalSpreading(np.pi / 4)
        omega_min = self.rng.random_double().greater_than(0)()
        omega_max = self.rng.random_double().greater_than(omega_min)()
        nfreq = 0
        ndir = self.rng.random_size_t()()
        s = Stretching(delta=0, h=0)
        expected_msg = "nfreq == 0"
        with self.assertRaises(InvalidInputException) as pcm:
            discretize(S, D, omega_min, omega_max, nfreq, ndir, s, False)
        self.assertTrue(expected_msg in str(pcm.exception), str(pcm.exception))

    def test_should_throw_if_ndir_is_zero(self):
        Hs = 3
        Tp = 4
        gamma = 1.4
        S = JonswapSpectrum(Hs, Tp, gamma)
        D = DiracDirectionalSpreading(np.pi / 4)
        omega_min = self.rng.random_double().greater_than(0)()
        omega_max = self.rng.random_double().greater_than(omega_min)()
        ndir = 0
        nfreq = self.rng.random_size_t().between(2, 1000)()
        s = Stretching(delta=0, h=0)
        expected_msg = "ndir == 0"
        with self.assertRaises(InvalidInputException) as pcm:
            discretize(S, D, omega_min, omega_max, nfreq, ndir, s, False)
        self.assertTrue(expected_msg in str(pcm.exception), str(pcm.exception))

    def test_should_throw_if_nfreq_is_one_but_omega_min_is_not_omega_max(self):
        Hs = 3
        Tp = 4
        gamma = 1.4
        S = JonswapSpectrum(Hs, Tp, gamma)
        D = DiracDirectionalSpreading(np.pi / 4)
        omega_min = self.rng.random_double().greater_than(0)()
        omega_max = self.rng.random_double().greater_than(omega_min)()
        nfreq = 1
        ndir = self.rng.random_size_t()()
        s = Stretching(delta=0, h=0)
        expected_regex = (
            "Asked for a single frequency \(nfreq = 1\), but omega_min \(=.*\) != omega_max \(=.*\)"
        )
        with self.assertRaises(InvalidInputException) as pcm:
            discretize(S, D, omega_min, omega_max, nfreq, ndir, s, False)
        self.assertTrue(re.search(expected_regex, str(pcm.exception)), str(pcm.exception))

    def test_should_throw_if_omega_min_equals_omega_max_but_nfreq_is_not_one(self):
        Hs = 3
        Tp = 4
        gamma = 1.4
        S = JonswapSpectrum(Hs, Tp, gamma)
        D = DiracDirectionalSpreading(np.pi / 4)
        omega_min = self.rng.random_double().greater_than(0)()
        omega_max = omega_min
        nfreq = self.rng.random_size_t().but_not(1)()
        ndir = self.rng.random_size_t()()
        s = Stretching(delta=0, h=0)
        expected_regex = "omega_min = omega_max \(=.*\) but nfreq != 1"
        with self.assertRaises(InvalidInputException) as pcm:
            discretize(S, D, omega_min, omega_max, nfreq, ndir, s, False)
        self.assertTrue(re.search(expected_regex, str(pcm.exception)), str(pcm.exception))

    def test_filtering_with_a_ratio_of_1_should_merely_sort_the_spectrum_by_amplitude(
        self,
    ):
        """
        \brief This test checks that the flatten and filter functions work correctly
        on the following spectrum discretization

        \code
        |  3  2  4
        ------------
        1 |  3  2  4
        5 | 15 10 20
        4 | 12  8 16
        3 |  9  6 12
        \endcode

        The following Python code was used to generated threshold input for the
        filter function

        \code{.py}
        import numpy as np
        S = np.array([3,2,4])
        D = np.array([1,5,4,3])
        f1,f2 = np.meshgrid(S,D)
        M = f1*f2
        pct = np.cumsum(np.flip(np.sort(np.hstack(M)),axis=0)) / M.sum()
        # Evaluate the sorted contribution of each component
        filter_ratio = np.floor(100*pct)
        \endcode
        """
        d = DiscreteDirectionalWaveSpectrum()
        d.Si = [3, 2, 4]
        d.Dj = [1, 5, 4, 3]
        d.k = self.rng.random_vector_of_double().of_size(3)()
        d.omega = [0, 1, 2]
        d.psi = [10, 11, 12, 13]
        d.phase = [[0.0] * 4, [0.0] * 4, [0.0] * 4]

        # Si.Dj     =   20    16    15    12    12     9     8
        # for (i,j) = (2,1) (2,2) (0,1) (0,2) (2,3) (0,3) (1,2)
        domega = lambda i: 0.5 if ((i == 0) or (i == 2)) else 1.0
        dpsi = lambda j: 0.5 if ((j == 0) or (j == 3)) else 1.0

        s_ori = flatten(d)
        self.assertEqual(type(s_ori), FlatDiscreteDirectionalWaveSpectrum)
        self.assertEqual(12, len(s_ori.a))
        self.assertEqual(12, len(s_ori.omega))
        self.assertEqual(12, len(s_ori.psi))
        self.assertEqual(12, len(s_ori.cos_psi))
        self.assertEqual(12, len(s_ori.sin_psi))
        self.assertEqual(12, len(s_ori.k))
        self.assertEqual(12, len(s_ori.phase))

        s = filter_spectrum(s_ori, 1)
        self.assertEqual(type(s), FlatDiscreteDirectionalWaveSpectrum)
        self.assertEqual(12, len(s.a))

        i = [1, 2, 1, 2, 0, 0, 1, 2, 0, 1, 2, 0]
        j = [1, 1, 2, 2, 1, 2, 3, 3, 3, 0, 0, 0]

        for k in range(12):
            self.assertEqual(
                np.sqrt(2 * domega(i[k]) * dpsi(j[k]) * d.Si[i[k]] * d.Dj[j[k]]), s.a[k]
            )
            self.assertEqual(d.omega[i[k]], s.omega[k])
            self.assertEqual(d.psi[j[k]], s.psi[k])

    def test_filtering_with_a_ratio_of_r_should_give_us_at_least_a_ratio_r_of_the_energy(
        self,
    ):
        d = DiscreteDirectionalWaveSpectrum()
        d.Si = [3, 2, 4]
        d.Dj = [1, 5, 4, 3]
        d.k = self.rng.random_vector_of_double().of_size(3)()
        d.omega = [0, 1, 2]
        d.psi = [10, 11, 12, 13]
        d.phase = [[0.0] * 4, [0.0] * 4, [0.0] * 4]

        s_ori = flatten(d)
        original_energy = sum(a * a for a in s_ori.a)
        for r in range(101):
            ratio = float(r) / 100.0
            s = filter_spectrum(s_ori, ratio)
            self.assertEqual(type(s), FlatDiscreteDirectionalWaveSpectrum)
            energy = sum(a * a for a in s.a)
            self.assertGreaterEqual(energy, ratio * original_energy)

    def test_dynamic_pressure_factor(self):
        assert_almost_equal = lambda x, y: self.assertAlmostEqual(x,y, places=12)
        s = Stretching(delta=0, h=0)
        assert_almost_equal(np.exp(-3), dynamic_pressure_factor(k=1, z=3, eta=2, stretching=s))
        assert_almost_equal(np.exp(-10), dynamic_pressure_factor(k=2, z=5, eta=4, stretching=s))
        assert_almost_equal(
            (np.exp(1) + np.exp(-1)) / (np.exp(3) + np.exp(-3)),
            dynamic_pressure_factor(k=1, z=2, eta=-4, stretching=s, h=3),
        )
        assert_almost_equal(
            (np.exp(0.14) + np.exp(-0.14)) / (np.exp(0.08) + np.exp(-0.08)),
            dynamic_pressure_factor(k=0.2, z=-0.3, h=0.4, eta=-0.5, stretching=s),
        )

    def test_equal_area_abscissae_should_return_empty_list_if_xs_and_ys_are_empty(self):
        xs = []
        ys = []
        self.assertTrue(len(equal_area_abscissae(xs, ys)) == 0)

    def test_equal_area_abscissae_should_throw_if_xs_and_ys_do_not_have_the_same_size(
        self,
    ):
        expected_msg = "xs and ys should have the same number of points."
        with self.assertRaises(InvalidInputException) as pcm:
            equal_area_abscissae([], [1.0])
        self.assertTrue(expected_msg in str(pcm.exception), str(pcm.exception))
        expected_msg = "xs and ys should have the same number of points"
        for _ in range(1000):
            nx = self.rng.random_size_t().no().greater_than(1000)()
            dn = self.rng.random_size_t().between(100, 200)()
            ny = nx + dn
            xs = self.rng.random_vector_of_double().of_size(nx)()
            ys = self.rng.random_vector_of_double().of_size(ny).greater_than(0)()
            with self.assertRaises(InvalidInputException) as pcm:
                equal_area_abscissae(xs, ys)
            self.assertTrue(expected_msg in str(pcm.exception), str(pcm.exception))

    def test_equal_area_abscissae_should_return_one_point_if_xs_has_one_point(self):
        for _ in range(1000):
            xs = [self.rng.random_double()()]
            ys = [abs(self.rng.random_double()())]
            self.assertEqual(1, len(equal_area_abscissae(xs, ys)))

    def test_equal_area_abscissae_should_return_xs_if_xs_has_one_point(self):
        for _ in range(1000):
            xs = [self.rng.random_double()()]
            ys = [abs(self.rng.random_double()())]
            self.assertEqual(xs[0], equal_area_abscissae(xs, ys)[0])

    def test_equal_area_abscissae_should_return_xs_if_xs_has_two_points(self):
        for _ in range(1000):
            xs = self.random_increasing_vector_of_size(2)
            ys = [
                self.rng.random_double().greater_than(0)(),
                self.rng.random_double().greater_than(0)(),
            ]
            ret = equal_area_abscissae(xs, ys)
            self.assertEqual(2, len(ret))
            self.assertEqual(xs[0], ret[0])
            self.assertEqual(xs[1], ret[1])

    def test_equal_area_abscissae_should_throw_if_xs_are_not_increasing(self):
        expected_msg = "xs should be strictly increasing"
        for _ in range(1000):
            n = self.rng.random_size_t().greater_than(1).no().greater_than(10)()
            xs = self.random_increasing_vector_of_size(n)
            ys = self.rng.random_vector_of_double().of_size(n).greater_than(0)()
            k1 = self.rng.random_size_t().between(0, n - 1)()
            k2 = self.rng.random_size_t().between(0, n - 1).but_not(k1)()
            val = xs[k1]
            xs[k1] = xs[k2]
            xs[k2] = val
            with self.assertRaises(InvalidInputException) as pcm:
                equal_area_abscissae(xs, ys)
            self.assertTrue(expected_msg in str(pcm.exception), str(pcm.exception))

    def test_equal_area_abscissae_should_throw_if_xs_are_not_strictly_increasing(self):
        for _ in range(1000):
            n = self.rng.random_size_t().greater_than(1).no().greater_than(10)()
            xs = self.random_increasing_vector_of_size(n)
            ys = self.rng.random_vector_of_double().of_size(n).greater_than(0)()
            k = self.rng.random_size_t().between(1, n - 1)()
            xs[k] = xs[k - 1]
            expected_msg = "xs should be strictly increasing"
            with self.assertRaises(InvalidInputException) as pcm:
                equal_area_abscissae(xs, ys)
            self.assertTrue(expected_msg in str(pcm.exception), str(pcm.exception))

    def test_equal_area_abscissae_should_throw_if_ys_are_not_positive(self):
        expected_msg = "All values in ys should be positive."
        for _ in range(1000):
            n = self.rng.random_size_t().greater_than(1).no().greater_than(10)()
            xs = self.random_increasing_vector_of_size(n)
            ys = self.rng.random_vector_of_double().of_size(n).greater_than(0)()
            k = self.rng.random_size_t().between(0, n - 1)()
            ys[k] = -ys[k]
            with self.assertRaises(InvalidInputException) as pcm:
                equal_area_abscissae(xs, ys)
            self.assertTrue(expected_msg in str(pcm.exception), str(pcm.exception))

    def test_equal_area_abscissae_should_return_equally_spaced_values_if_all_ys_are_identical(
        self,
    ):
        for _ in range(1000):
            n = self.rng.random_size_t().greater_than(1).no().greater_than(10)()
            xs = self.random_increasing_vector_of_size(n)
            xmin = xs[0]
            xmax = xs[-1]
            ref = [j / (n - 1) * (xmax - xmin) + xmin for j in range(n)]
            val = self.rng.random_double().greater_than(0)()
            ys = [val] * n
            res = equal_area_abscissae(xs, ys)
            self.assertEqual(n, len(res))
            for j in range(n):
                self.assertAlmostEqual(ref[j], res[j], delta=EPS)

    def test_area_curve_should_return_list_of_same_size_as_inputs(self):
        large_int = int(1e5)
        for n in range(1000):
            xs = self.random_increasing_vector_of_size(n)
            ys = self.rng.random_vector_of_double().of_size(n).greater_than(0)()
            self.assertEqual(n, len(area_curve(xs, ys)))
        xs_big = self.random_increasing_vector_of_size(large_int)
        ys_big = self.rng.random_vector_of_double().of_size(large_int).greater_than(0)()
        self.assertEqual(large_int, len(area_curve(xs_big, ys_big)))

    def test_area_curve_should_throw_if_xs_and_ys_do_not_have_the_same_size(self):
        expected_msg = "xs and ys should have the same number of points"
        for _ in range(1000):
            nx = self.rng.random_size_t().no().greater_than(1000)()
            ny = self.rng.random_size_t().no().greater_than(1000).but_not(nx)()
            xs = self.random_increasing_vector_of_size(nx)
            ys = self.rng.random_vector_of_double().of_size(ny).greater_than(0)()
            with self.assertRaises(InvalidInputException) as pcm:
                area_curve(xs, ys)
            self.assertTrue(expected_msg in str(pcm.exception), str(pcm.exception))

    def test_area_curve_should_throw_if_xs_are_not_increasing(self):
        expected_msg = "xs should be strictly increasing"
        for _ in range(1000):
            n = self.rng.random_size_t().greater_than(1).no().greater_than(10)()
            xs = self.random_increasing_vector_of_size(n)
            ys = self.rng.random_vector_of_double().of_size(n).greater_than(0)()
            k1 = self.rng.random_size_t().between(0, n - 1)()
            k2 = self.rng.random_size_t().between(0, n - 1).but_not(k1)()
            val = xs[k1]
            xs[k1] = xs[k2]
            xs[k2] = val
            with self.assertRaises(InvalidInputException) as pcm:
                area_curve(xs, ys)
            self.assertTrue(expected_msg in str(pcm.exception), str(pcm.exception))

    def test_area_curve_should_throw_if_xs_are_not_strictly_increasing(self):
        expected_msg = "xs should be strictly increasing"
        for _ in range(1000):
            n = self.rng.random_size_t().greater_than(1).no().greater_than(10)()
            xs = self.random_increasing_vector_of_size(n)
            ys = self.rng.random_vector_of_double().of_size(n).greater_than(0)()
            k = self.rng.random_size_t().between(1, n - 1)()
            xs[k] = xs[k - 1]
            with self.assertRaises(InvalidInputException) as pcm:
                area_curve(xs, ys)
            self.assertTrue(expected_msg in str(pcm.exception), str(pcm.exception))

    def test_area_curve_should_throw_if_ys_are_not_positive(self):
        expected_msg = "All values in ys should be positive"
        for _ in range(1000):
            n = self.rng.random_size_t().greater_than(1).no().greater_than(10)()
            xs = self.random_increasing_vector_of_size(n)
            ys = self.rng.random_vector_of_double().of_size(n).greater_than(0)()
            k = self.rng.random_size_t().between(0, n - 1)()
            ys[k] = -ys[k]
            with self.assertRaises(InvalidInputException) as pcm:
                area_curve(xs, ys)
            self.assertTrue(expected_msg in str(pcm.exception), str(pcm.exception))

    def test_area_curve_first_value_always_zero(self):
        for _ in range(1000):
            n = self.rng.random_size_t().greater_than(1).no().greater_than(10)()
            xs = self.random_increasing_vector_of_size(n)
            ys = self.rng.random_vector_of_double().of_size(n).greater_than(0)()
            self.assertEqual(0, area_curve(xs, ys)[0])

    def test_area_curve_should_return_zero_if_ys_are_zero(self):
        for _ in range(1000):
            n = self.rng.random_size_t().greater_than(1).no().greater_than(10)()
            xs = self.random_increasing_vector_of_size(n)
            ys = [0.0] * n
            for r in area_curve(xs, ys):
                self.assertEqual(0, r)

    def test_area_curve_can_integrate_rectangle(self):
        for _ in range(1000):
            xa = self.rng.random_double()()
            xb = self.rng.random_double().greater_than(xa)()
            ya = self.rng.random_double().greater_than(0)()
            yb = ya
            xs = [xa, xb]
            ys = [ya, yb]
            ret = area_curve(xs, ys)
            self.assertEqual(0, ret[0])
            self.assertEqual((xb - xa) * ya, ret[1])

    def test_area_curve_can_integrate_trapeze(self):
        for _ in range(1000):
            xa = self.rng.random_double()()
            xb = self.rng.random_double().greater_than(xa)()
            ya = self.rng.random_double().greater_than(0)()
            yb = self.rng.random_double().greater_than(0)()
            xs = [xa, xb]
            ys = [ya, yb]
            ret = area_curve(xs, ys)
            self.assertEqual(0, ret[0])
            self.assertEqual((xb - xa) * (ya + yb) / 2, ret[1])

    def test_area_curve_can_integrate_two_rectangles(self):
        for _ in range(1000):
            xa = self.rng.random_double()()
            xb = self.rng.random_double().greater_than(xa)()
            xc = self.rng.random_double().greater_than(xb)()
            ya = self.rng.random_double().greater_than(0)()
            yb = ya
            yc = ya
            xs = [xa, xb, xc]
            ys = [ya, yb, yc]
            ret = area_curve(xs, ys)
            self.assertEqual(0, ret[0])
            self.assertAlmostEqual((xb - xa) * ya, ret[1], delta=1e-12 * max(1.0, abs(ret[1])))
            self.assertAlmostEqual((xc - xa) * ya, ret[2], delta=1e-12 * max(1.0, abs(ret[1])))

    def test_area_curve_can_integrate_rectangle_after_lots_of_zeros(self):
        for _ in range(1000):
            nb_of_zeros = self.rng.random_size_t().between(0, 10000)()
            xs = self.random_increasing_vector_of_size(nb_of_zeros + 2)
            ys = [0.0] * nb_of_zeros
            ya = self.rng.random_double().greater_than(0)()
            yb = ya
            ys.append(ya)
            ys.append(yb)
            ret = area_curve(xs, ys)
            self.assertEqual(nb_of_zeros + 2, len(ret))
            for k in range(nb_of_zeros):
                self.assertEqual(0, ret[k])
            self.assertAlmostEqual(
                0.5 * (xs[nb_of_zeros] - xs[nb_of_zeros - 1]) * ya,
                ret[nb_of_zeros],
                delta=EPS,
            )
            self.assertAlmostEqual(
                ret[nb_of_zeros] + (xs[nb_of_zeros + 1] - xs[nb_of_zeros]) * ya,
                ret[nb_of_zeros + 1],
                delta=EPS,
            )

    def test_can_find_given_area_in_area_curve(self):
        xs = [0, 8]
        ys = [0, 8]
        self.assertAlmostEqual(
            2,
            find_integration_bound_yielding_target_area(2, xs, ys, area_curve(xs, ys)),
            delta=EPS,
        )

    def test_can_find_given_area_in_area_curve_2(self):
        xs = [1, 2, 3, 4, 5]
        ys = [2, 3, 3, 4, 0]
        self.assertAlmostEqual(
            1,
            find_integration_bound_yielding_target_area(0, xs, ys, area_curve(xs, ys)),
            delta=EPS,
        )
        self.assertAlmostEqual(
            2,
            find_integration_bound_yielding_target_area(2.5, xs, ys, area_curve(xs, ys)),
            delta=EPS,
        )
        self.assertAlmostEqual(
            3,
            find_integration_bound_yielding_target_area(5.5, xs, ys, area_curve(xs, ys)),
            delta=EPS,
        )
        self.assertAlmostEqual(
            4,
            find_integration_bound_yielding_target_area(9, xs, ys, area_curve(xs, ys)),
            delta=EPS,
        )
        self.assertAlmostEqual(
            5,
            find_integration_bound_yielding_target_area(11, xs, ys, area_curve(xs, ys)),
            delta=EPS,
        )
        self.assertAlmostEqual(
            2.5,
            find_integration_bound_yielding_target_area(4, xs, ys, area_curve(xs, ys)),
            delta=EPS,
        )

    def test_equal_area_abscissae_should_return_equally_spaced_values_for_test_case(
        self,
    ):
        xs = [1, 2, 3, 4, 5.5]
        ys = [2, 3, 3, 4, 0]
        res = equal_area_abscissae(xs, ys)
        self.assertEqual(5, len(res))
        self.assertAlmostEqual(1, res[0], delta=EPS)
        self.assertAlmostEqual(2 + 1.0 / 6.0, res[1], delta=EPS)
        self.assertAlmostEqual(np.sqrt(10), res[2], delta=EPS)
        self.assertAlmostEqual(4, res[3], delta=EPS)
        self.assertAlmostEqual(5.5, res[4], delta=EPS)

    def test_equal_energy_bins(self):
        Hs = 3
        Tp = 4
        gamma = 1.4
        S = JonswapSpectrum(Hs, Tp, gamma)
        D = Cos2sDirectionalSpreading(np.pi / 4, 2)
        s = Stretching(delta=0, h=0)
        A = discretize(S, D, 0.01, 3, 1000, 1840, s, True)
        self.assertEqual(1000, len(A.omega))
        self.assertAlmostEqual(0.01, A.omega[0], delta=EPS)
        self.assertAlmostEqual(1.025029449067951, A.omega[1], delta=EPS)
        self.assertAlmostEqual(1.0525530074258682, A.omega[2], delta=EPS)
        self.assertAlmostEqual(1.0842190871695854, A.omega[4], delta=EPS)
        self.assertAlmostEqual(1.128270684351939, A.omega[9], delta=EPS)
        self.assertAlmostEqual(1.3472975072489475, A.omega[99], delta=EPS)
        self.assertAlmostEqual(3, A.omega[999], delta=EPS)
        self.assertEqual(1840, len(A.psi))
        B = discretize(S, D, 0.01, 3, 10, 17, s, True)
        self.assertEqual(type(B), DiscreteDirectionalWaveSpectrum)
        self.assertEqual(10, len(B.omega))
        self.assertEqual(17, len(B.psi))
        self.assertAlmostEqual(0, B.psi[0], delta=EPS)
        self.assertAlmostEqual(2 * np.pi / 17 * 1, B.psi[1], delta=EPS)
        self.assertAlmostEqual(2 * np.pi / 17 * 2, B.psi[2], delta=EPS)
        self.assertAlmostEqual(2 * np.pi / 17 * 3, B.psi[3], delta=EPS)
        self.assertAlmostEqual(2 * np.pi / 17 * 4, B.psi[4], delta=EPS)
        self.assertAlmostEqual(2 * np.pi / 17 * 5, B.psi[5], delta=EPS)
        self.assertAlmostEqual(2 * np.pi / 17 * 6, B.psi[6], delta=EPS)
        self.assertAlmostEqual(2 * np.pi / 17 * 7, B.psi[7], delta=EPS)
        self.assertAlmostEqual(2 * np.pi / 17 * 8, B.psi[8], delta=EPS)
        self.assertAlmostEqual(2 * np.pi / 17 * 9, B.psi[9], delta=EPS)
        self.assertAlmostEqual(2 * np.pi / 17 * 10, B.psi[10], delta=EPS)
        self.assertAlmostEqual(2 * np.pi / 17 * 11, B.psi[11], delta=EPS)
        self.assertAlmostEqual(2 * np.pi / 17 * 12, B.psi[12], delta=EPS)
        self.assertAlmostEqual(2 * np.pi / 17 * 13, B.psi[13], delta=EPS)
        self.assertAlmostEqual(2 * np.pi / 17 * 14, B.psi[14], delta=EPS)
        self.assertAlmostEqual(2 * np.pi / 17 * 15, B.psi[15], delta=EPS)
        self.assertAlmostEqual(2 * np.pi / 17 * 16, B.psi[16], delta=EPS)
        self.assertAlmostEqual(0.01, B.omega[0], delta=EPS)
        self.assertAlmostEqual(1.3266362675988099, B.omega[1], delta=EPS)
        self.assertAlmostEqual(1.4718706793919485, B.omega[2], delta=EPS)
        self.assertAlmostEqual(1.5894739415969286, B.omega[3], delta=EPS)
        self.assertAlmostEqual(1.6915444316496033, B.omega[4], delta=EPS)
        self.assertAlmostEqual(1.80136306541316, B.omega[5], delta=EPS)
        self.assertAlmostEqual(1.9390777413117828, B.omega[6], delta=EPS)
        self.assertAlmostEqual(2.1335838860523002, B.omega[7], delta=EPS)
        self.assertAlmostEqual(2.4226798199184207, B.omega[8], delta=EPS)
        self.assertAlmostEqual(3, B.omega[9], delta=EPS)


if __name__ == "__main__":

    unittest.main()
