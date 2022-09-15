/*
 * DirectionalWaveSpectrumTest.cpp
 *
 *  Created on: Jul 31, 2014
 *      Author: cady
 */

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI
#define EPS (1E-10)

#include "discretizeTest.hpp"
#include "environment_models/discretize.hpp"
#include "JonswapSpectrum.hpp"
#include "Cos2sDirectionalSpreading.hpp"
#include "environment_models/DiracSpectralDensity.hpp"
#include "environment_models/DiracDirectionalSpreading.hpp"
#include "xdyn/exceptions/InvalidInputException.hpp"
#include "environment_models/Stretching.hpp"
#include "external_data_structures/YamlWaveModelInput.hpp"
#include <ssc/macros.hpp>

discretizeTest::discretizeTest()
    : a(ssc::random_data_generator::DataGenerator(8421))
{
}

discretizeTest::~discretizeTest()
{
}

void discretizeTest::SetUp()
{
}

void discretizeTest::TearDown()
{
}

std::vector<double> discretizeTest::random_increasing_vector_of_size(const size_t n) const
{
    if (n <= 0)
    {
        return std::vector<double>();
    }
    std::vector<double> ret(n, a.random<double>().between(-1e6, 1e6));
    for (size_t i = 1 ; i < n ; ++i)
    {
        const double increment = a.random<double>().between(0, 1e3).but_not(0);
        ret[i] = ret[i-1] + increment;
    }
    return ret;
}

TEST_F(discretizeTest, example)
{
//! [discretizeTest example]
    const double Hs = 3;
    const double Tp = 4;
    const double gamma = 1.4;
    const JonswapSpectrum S(Hs, Tp, gamma);
    const Cos2sDirectionalSpreading D(PI/4, 2);
    YamlStretching y;
    const Stretching s(y);
    const DiscreteDirectionalWaveSpectrum A = discretize(S, D, 0.01, 3, 1000, 1000, s, false);
//! [discretizeTest example]
//! [discretizeTest expected output]
    ASSERT_EQ(1000, A.Dj.size());
    ASSERT_EQ(1000, A.Si.size());
//! [discretizeTest expected output]
}

TEST_F(discretizeTest, Dirac_in_frequency)
{
    const double omega0 = 0.2;
    const double Hs = 3;
    const DiracSpectralDensity S(omega0, Hs);
    const Cos2sDirectionalSpreading D(PI/4, 2);
    YamlStretching y;
    const Stretching s(y);
    const DiscreteDirectionalWaveSpectrum A = discretize(S, D, 0.01, 3, 1000, 1000, s, false);
    ASSERT_EQ(1000, A.Dj.size());
    ASSERT_EQ(1, A.Si.size());
    ASSERT_DOUBLE_EQ(Hs*Hs/8, A.Si.front());
}

TEST_F(discretizeTest, Dirac_in_direction)
{
    const double Hs = 3;
    const double Tp = 4;
    const double gamma = 1.4;
    const JonswapSpectrum S(Hs, Tp, gamma);
    const DiracDirectionalSpreading D(PI/4);
    YamlStretching y;
    const Stretching s(y);
    const DiscreteDirectionalWaveSpectrum A = discretize(S, D, 0.01, 3, 1000, 1000, s, false);
    ASSERT_EQ(1, A.Dj.size());
    ASSERT_EQ(1000, A.Si.size());
    ASSERT_DOUBLE_EQ(1, A.Dj.front());
}

TEST_F(discretizeTest, should_throw_if_omega_min_is_negative)
{
    const double Hs = 3;
    const double Tp = 4;
    const double gamma = 1.4;
    const JonswapSpectrum S(Hs, Tp, gamma);
    const DiracDirectionalSpreading D(PI/4);
    const double omega_min = a.random<double>().no().greater_than(0);
    const double omega_max = a.random<double>().greater_than(0);
    const size_t nfreq = a.random<size_t>();
    const size_t ndir = a.random<size_t>();
    YamlStretching y;
    const Stretching s(y);
    ASSERT_THROW(discretize(S, D, omega_min, omega_max, nfreq, ndir, s, false), InvalidInputException);
}

TEST_F(discretizeTest, should_throw_if_omega_min_is_zero)
{
    const double Hs = 3;
    const double Tp = 4;
    const double gamma = 1.4;
    const JonswapSpectrum S(Hs, Tp, gamma);
    const DiracDirectionalSpreading D(PI/4);
    const double omega_min = 0;
    const double omega_max = a.random<double>().greater_than(0);
    const size_t nfreq = a.random<size_t>();
    const size_t ndir = a.random<size_t>();
    YamlStretching y;
    const Stretching s(y);
    ASSERT_THROW(discretize(S, D, omega_min, omega_max, nfreq, ndir, s, false), InvalidInputException);
}

TEST_F(discretizeTest, should_throw_if_omega_max_is_negative)
{
    const double Hs = 3;
    const double Tp = 4;
    const double gamma = 1.4;
    const JonswapSpectrum S(Hs, Tp, gamma);
    const DiracDirectionalSpreading D(PI/4);
    const double omega_min = a.random<double>().greater_than(0);
    const double omega_max = a.random<double>().no().greater_than(0);
    const size_t nfreq = a.random<size_t>();
    const size_t ndir = a.random<size_t>();
    YamlStretching y;
    const Stretching s(y);
    ASSERT_THROW(discretize(S, D, omega_min, omega_max, nfreq, ndir, s, false), InvalidInputException);
}

TEST_F(discretizeTest, should_throw_if_omega_max_is_zero)
{
    const double Hs = 3;
    const double Tp = 4;
    const double gamma = 1.4;
    const JonswapSpectrum S(Hs, Tp, gamma);
    const DiracDirectionalSpreading D(PI/4);
    const double omega_min = a.random<double>().greater_than(0);
    const double omega_max = 0;
    const size_t nfreq = a.random<size_t>();
    const size_t ndir = a.random<size_t>();
    YamlStretching y;
    const Stretching s(y);
    ASSERT_THROW(discretize(S, D, omega_min, omega_max, nfreq, ndir, s, false), InvalidInputException);
}

TEST_F(discretizeTest, should_throw_if_omega_max_is_lower_than_omega_min)
{
    const double Hs = 3;
    const double Tp = 4;
    const double gamma = 1.4;
    const JonswapSpectrum S(Hs, Tp, gamma);
    const DiracDirectionalSpreading D(PI/4);
    const double omega_min = a.random<double>().greater_than(0);
    const double omega_max = a.random<double>().no().greater_than(omega_min);
    const size_t nfreq = a.random<size_t>();
    const size_t ndir = a.random<size_t>();
    YamlStretching y;
    const Stretching s(y);
    ASSERT_THROW(discretize(S, D, omega_min, omega_max, nfreq, ndir, s, false), InvalidInputException);
}

TEST_F(discretizeTest, should_throw_if_nfreq_is_zero)
{
    const double Hs = 3;
    const double Tp = 4;
    const double gamma = 1.4;
    const JonswapSpectrum S(Hs, Tp, gamma);
    const DiracDirectionalSpreading D(PI/4);
    const double omega_min = a.random<double>().greater_than(0);
    const double omega_max = a.random<double>().greater_than(0);
    const size_t nfreq = 0;
    const size_t ndir = a.random<size_t>();
    YamlStretching y;
    const Stretching s(y);
    ASSERT_THROW(discretize(S, D, omega_min, omega_max, nfreq, ndir, s, false), InvalidInputException);
}

TEST_F(discretizeTest, should_throw_if_ndir_is_zero)
{
    const double Hs = 3;
    const double Tp = 4;
    const double gamma = 1.4;
    const JonswapSpectrum S(Hs, Tp, gamma);
    const DiracDirectionalSpreading D(PI/4);
    const double omega_min = a.random<double>().greater_than(0);
    const double omega_max = a.random<double>().greater_than(omega_min);
    const size_t ndir = 0;
    const size_t nfreq = a.random<size_t>().between(2,1000);
    YamlStretching y;
    const Stretching s(y);
    ASSERT_THROW(discretize(S, D, omega_min, omega_max, nfreq, ndir, s, false), InvalidInputException);
}

TEST_F(discretizeTest, should_throw_if_nfreq_is_one_but_omega_min_is_not_omega_max)
{
    const double Hs = 3;
    const double Tp = 4;
    const double gamma = 1.4;
    const JonswapSpectrum S(Hs, Tp, gamma);
    const DiracDirectionalSpreading D(PI/4);
    const double omega_min = a.random<double>().greater_than(0);
    const double omega_max = a.random<double>().greater_than(omega_min);
    const size_t nfreq = 1;
    const size_t ndir = a.random<size_t>();
    YamlStretching y;
    const Stretching s(y);
    ASSERT_THROW(discretize(S, D, omega_min, omega_max, nfreq, ndir, s, false), InvalidInputException);
}

TEST_F(discretizeTest, should_throw_if_omega_min_equals_omega_max_but_nfreq_is_not_one)
{
    const double Hs = 3;
    const double Tp = 4;
    const double gamma = 1.4;
    const JonswapSpectrum S(Hs, Tp, gamma);
    const DiracDirectionalSpreading D(PI/4);
    const double omega_min = a.random<double>().greater_than(0);
    const double omega_max = omega_min;
    const size_t nfreq = a.random<size_t>().but_not(1);
    const size_t ndir = a.random<size_t>();
    YamlStretching y;
    const Stretching s(y);
    ASSERT_THROW(discretize(S, D, omega_min, omega_max, nfreq, ndir, s, false), InvalidInputException);
}

/**
 * \brief This test checks that the flatten and filter functions work correctly
 * on the following spectrum discretization
 *
 * \code
 *    |  3  2  4
 *  ------------
 *  1 |  3  2  4
 *  5 | 15 10 20
 *  4 | 12  8 16
 *  3 |  9  6 12
 * \endcode
 *
 * The following Python code was used to generated threshold input for the
 * filter function
 *
 * \code{.py}
 * import numpy as np
 *  S = np.array([3,2,4])
 * D = np.array([1,5,4,3])
 * f1,f2 = np.meshgrid(S,D)
 * M = f1*f2
 * pct = np.cumsum(np.flip(np.sort(np.hstack(M)),axis=0)) / M.sum()
 * # Evaluate the sorted contribution of each component
 * filter_ratio = np.floor(100*pct)
 * \endcode
 */
TEST_F(discretizeTest, filtering_with_a_ratio_of_1_should_merely_sort_the_spectrum_by_amplitude)
{
    DiscreteDirectionalWaveSpectrum d;
    d.Si = {3,2,4};
    d.Dj = {1,5,4,3};
    d.k = a.random_vector_of<double>().of_size(3);
    d.omega = {0,1,2};
    d.phase = std::vector<std::vector<double> >(3,std::vector<double>(4,0));
    d.psi = {10,11,12,13};

    // Si.Dj     =   20    16    15    12    12     9     8
    // for (i,j) = (2,1) (2,2) (0,1) (0,2) (2,3) (0,3) (1,2)

    const auto domega = [](const size_t i){return ((i==0) || (i==2)) ? 0.5 : 1;};
    const auto dpsi = [](const size_t j){return ((j==0) || (j==3)) ? 0.5 : 1;};

    FlatDiscreteDirectionalWaveSpectrum s_ori = flatten(d);
    EXPECT_EQ(12, s_ori.a.size());
    EXPECT_EQ(12, s_ori.omega.size());
    EXPECT_EQ(12, s_ori.psi.size());
    EXPECT_EQ(12, s_ori.cos_psi.size());
    EXPECT_EQ(12, s_ori.sin_psi.size());
    EXPECT_EQ(12, s_ori.k.size());
    EXPECT_EQ(12, s_ori.phase.size());

    const FlatDiscreteDirectionalWaveSpectrum s = filter(s_ori, 1);
    EXPECT_EQ(12, s.a.size());

    const std::vector<size_t> i = {1,2,1,2,0,0,1,2,0,1,2,0};
    const std::vector<size_t> j = {1,1,2,2,1,2,3,3,3,0,0,0};

    for (size_t k = 0 ; k < 12; ++k)
    {
        EXPECT_DOUBLE_EQ(sqrt(2*domega(i[k])*dpsi(j[k])*d.Si[i[k]]*d.Dj[j[k]]), s.a[k]);
        EXPECT_DOUBLE_EQ(d.omega[i[k]], s.omega[k]);
        EXPECT_DOUBLE_EQ(d.psi[j[k]], s.psi[k]);
    }
}

TEST_F(discretizeTest, filtering_with_a_ratio_of_r_should_give_us_at_least_a_ratio_r_of_the_energy)
{
    DiscreteDirectionalWaveSpectrum d;
    d.Si = {3,2,4};
    d.Dj = {1,5,4,3};
    d.k = a.random_vector_of<double>().of_size(3);
    d.omega = {0,1,2};
    d.phase = std::vector<std::vector<double> >(3,std::vector<double>(4,0));
    d.psi = {10,11,12,13};

    FlatDiscreteDirectionalWaveSpectrum s_ori = flatten(d);
    double original_energy = 0;
    for (const double a : s_ori.a)
    {
        original_energy += a*a;
    }
    for (size_t r = 0 ; r <= 100 ; ++r)
    {
        const double ratio = ((double)r)/100.;
        const FlatDiscreteDirectionalWaveSpectrum s = filter(s_ori, ratio);
        double energy = 0;
        for (const double a : s.a)
        {
            energy += a*a;
        }
        EXPECT_GE(energy, ratio*original_energy);
    }
}

TEST_F(discretizeTest, dynamic_pressure_factor)
{
    //! [discretizeTest dynamic_pressure_factor example]
    YamlStretching y;
    const Stretching s(y);
    ASSERT_DOUBLE_EQ(exp(-3), dynamic_pressure_factor(1,3,2,s));
    ASSERT_DOUBLE_EQ(exp(-10), dynamic_pressure_factor(2,5,4,s));
    ASSERT_DOUBLE_EQ((exp(1)+exp(-1))/(exp(3)+exp(-3)), dynamic_pressure_factor(1,2,3,-4,s));
    ASSERT_DOUBLE_EQ((exp(0.14)+exp(-0.14))/(exp(0.08)+exp(-0.08)), dynamic_pressure_factor(0.2,-0.3,0.4,-0.5,s));
    //! [discretizeTest dynamic_pressure_factor example]
}

TEST_F(discretizeTest, equal_area_abscissae_should_return_empty_list_if_xs_and_ys_are_empty)
{
    const std::vector<double> xs;
    const std::vector<double> ys;
    ASSERT_TRUE(equal_area_abscissae(xs, ys).empty());
}

TEST_F(discretizeTest, equal_area_abscissae_should_throw_if_xs_and_ys_do_not_have_the_same_size)
{
    ASSERT_THROW(equal_area_abscissae(std::vector<double>(), std::vector<double>(1, 1)), InvalidInputException);
    for (size_t i = 0 ; i < 1000 ; ++i)
    {
        const size_t nx = a.random<size_t>().no().greater_than(1000);
        const size_t dn = a.random<size_t>().between(100,200);
        const size_t ny = nx + dn;
        const std::vector<double> xs = a.random_vector_of<double>().of_size(nx);
        const std::vector<double> ys = a.random_vector_of<double>().of_size(ny).greater_than(0);
        ASSERT_THROW(equal_area_abscissae(xs, ys), InvalidInputException);
    }
}

TEST_F(discretizeTest, equal_area_abscissae_should_return_one_point_if_xs_has_one_point)
{
    for (size_t i = 0 ; i < 1000 ; ++i)
    {
        const std::vector<double> xs(1, a.random<double>());
        const std::vector<double> ys(1, std::abs(a.random<double>()));
        ASSERT_EQ(1, equal_area_abscissae(xs, ys).size());
    }
}

TEST_F(discretizeTest, equal_area_abscissae_should_return_xs_if_xs_has_one_point)
{
    for (size_t i = 0 ; i < 1000 ; ++i)
    {
        const std::vector<double> xs(1, a.random<double>());
        const std::vector<double> ys(1, std::abs(a.random<double>()));
        ASSERT_DOUBLE_EQ(xs[0], equal_area_abscissae(xs, ys).at(0));
    }
}

TEST_F(discretizeTest, equal_area_abscissae_should_return_xs_if_xs_has_two_points)
{
    for (size_t i = 0 ; i < 1000 ; ++i)
    {
        const std::vector<double> xs = random_increasing_vector_of_size(2);
        const std::vector<double> ys = {a.random<double>().greater_than(0),a.random<double>().greater_than(0)};
        const auto ret = equal_area_abscissae(xs, ys);
        ASSERT_EQ(2, ret.size());
        ASSERT_DOUBLE_EQ(xs[0], ret.at(0));
        ASSERT_DOUBLE_EQ(xs[1], ret.at(1));
    }
}

TEST_F(discretizeTest, equal_area_abscissae_should_throw_if_xs_are_not_increasing)
{
    for (size_t i = 0 ; i < 1000 ; ++i)
    {
        const size_t n = a.random<size_t>().greater_than(1).no().greater_than(10);
        std::vector<double> xs = random_increasing_vector_of_size(n);
        const std::vector<double> ys = a.random_vector_of<double>().of_size(n).greater_than(0);
        const size_t k1 = a.random<size_t>().between(0, n-1);
        const size_t k2 = a.random<size_t>().between(0, n-1).but_not(k1);
        const double val = xs[k1];
        xs[k1] = xs[k2];
        xs[k2] = val;
        ASSERT_THROW(equal_area_abscissae(xs, ys), InvalidInputException);
    }
}

TEST_F(discretizeTest, equal_area_abscissae_should_throw_if_xs_are_not_strictly_increasing)
{
    for (size_t i = 0 ; i < 1000 ; ++i)
    {
        const size_t n = a.random<size_t>().greater_than(1).no().greater_than(10);
        std::vector<double> xs = random_increasing_vector_of_size(n);
        const std::vector<double> ys = a.random_vector_of<double>().of_size(n).greater_than(0);
        const size_t k = a.random<size_t>().between(1, n-1);
        xs[k] = xs[k-1];
        ASSERT_THROW(equal_area_abscissae(xs, ys), InvalidInputException);
    }
}

TEST_F(discretizeTest, equal_area_abscissae_should_throw_if_ys_are_not_positive)
{
    for (size_t i = 0 ; i < 1000 ; ++i)
    {
        const size_t n = a.random<size_t>().greater_than(1).no().greater_than(10);
        const std::vector<double> xs = random_increasing_vector_of_size(n);
        std::vector<double> ys = a.random_vector_of<double>().of_size(n).greater_than(0);
        const size_t k = a.random<size_t>().between(0, n-1);
        ys[k] = -ys[k];
        ASSERT_THROW(equal_area_abscissae(xs, ys), InvalidInputException);
    }
}

TEST_F(discretizeTest, equal_area_abscissae_should_return_equally_spaced_values_if_all_ys_are_identical)
{
    for (size_t i = 0 ; i < 1000 ; ++i)
    {
        const size_t n = a.random<size_t>().greater_than(1).no().greater_than(10);
        const std::vector<double> xs = random_increasing_vector_of_size(n);
        const double xmin = xs.front();
        const double xmax = xs.back();
        std::vector<double> ref(n);
        for (size_t j = 0 ; j < n ; ++j)
        {
            ref[j] = ((double)j)/((double)n-1)*(xmax-xmin) + xmin;
        }
        const double val = a.random<double>().greater_than(0);
        const std::vector<double> ys(n, val);
        const auto res = equal_area_abscissae(xs, ys);
        ASSERT_EQ(n, res.size());
        for (size_t j = 0 ; j < n ; ++j)
        {
            ASSERT_SMALL_RELATIVE_ERROR(ref.at(j), res.at(j), EPS);
        }
    }
}

TEST_F(discretizeTest, area_curve_should_return_list_of_same_size_as_inputs)
{
    for (size_t n = 0 ; n < 1000 ; ++n)
    {
        const std::vector<double> xs = random_increasing_vector_of_size(n);
        const std::vector<double> ys = a.random_vector_of<double>().of_size(n).greater_than(0);
        ASSERT_EQ(n, area_curve(xs, ys).size());
    }
    const std::vector<double> xs_big = random_increasing_vector_of_size(1e5);
    const std::vector<double> ys_big = a.random_vector_of<double>().of_size(1e5).greater_than(0);
    ASSERT_EQ(1e5, area_curve(xs_big, ys_big).size());
}

TEST_F(discretizeTest, area_curve_should_throw_if_xs_and_ys_do_not_have_the_same_size)
{
    for (size_t i = 0 ; i < 1000 ; ++i)
    {
        const size_t nx = a.random<size_t>().no().greater_than(1000);
        const size_t ny = a.random<size_t>().no().greater_than(1000).but_not(nx);
        const std::vector<double> xs = random_increasing_vector_of_size(nx);
        const std::vector<double> ys = a.random_vector_of<double>().of_size(ny).greater_than(0);
        ASSERT_THROW(area_curve(xs, ys), InvalidInputException);
    }
}

TEST_F(discretizeTest, area_curve_should_throw_if_xs_are_not_increasing)
{
    for (size_t i = 0 ; i < 1000 ; ++i)
    {
        const size_t n = a.random<size_t>().greater_than(1).no().greater_than(10);
        std::vector<double> xs = random_increasing_vector_of_size(n);
        const std::vector<double> ys = a.random_vector_of<double>().of_size(n).greater_than(0);
        const size_t k1 = a.random<size_t>().between(0, n-1);
        const size_t k2 = a.random<size_t>().between(0, n-1).but_not(k1);
        const double val = xs[k1];
        xs[k1] = xs[k2];
        xs[k2] = val;
        ASSERT_THROW(area_curve(xs, ys), InvalidInputException);
    }
}

TEST_F(discretizeTest, area_curve_should_throw_if_xs_are_not_strictly_increasing)
{
    for (size_t i = 0 ; i < 1000 ; ++i)
    {
        const size_t n = a.random<size_t>().greater_than(1).no().greater_than(10);
        std::vector<double> xs = random_increasing_vector_of_size(n);
        const std::vector<double> ys = a.random_vector_of<double>().of_size(n).greater_than(0);
        const size_t k = a.random<size_t>().between(1, n-1);
        xs[k] = xs[k-1];
        ASSERT_THROW(area_curve(xs, ys), InvalidInputException);
    }
}

TEST_F(discretizeTest, area_curve_should_throw_if_ys_are_not_positive)
{
    for (size_t i = 0 ; i < 1000 ; ++i)
    {
        const size_t n = a.random<size_t>().greater_than(1).no().greater_than(10);
        const std::vector<double> xs = random_increasing_vector_of_size(n);
        std::vector<double> ys = a.random_vector_of<double>().of_size(n).greater_than(0);
        const size_t k = a.random<size_t>().between(0, n-1);
        ys[k] = -ys[k];
        ASSERT_THROW(area_curve(xs, ys), InvalidInputException);
    }
}

TEST_F(discretizeTest, area_curve_first_value_always_zero)
{
    for (size_t i = 0 ; i < 1000 ; ++i)
    {
        const size_t n = a.random<size_t>().greater_than(1).no().greater_than(10);
        const std::vector<double> xs = random_increasing_vector_of_size(n);
        std::vector<double> ys = a.random_vector_of<double>().of_size(n).greater_than(0);
        ASSERT_DOUBLE_EQ(0, area_curve(xs, ys).at(0));
    }
}

TEST_F(discretizeTest, area_curve_should_return_zero_if_ys_are_zero)
{
    for (size_t i = 0 ; i < 1000 ; ++i)
    {
        const size_t n = a.random<size_t>().greater_than(1).no().greater_than(10);
        const std::vector<double> xs = random_increasing_vector_of_size(n);
        const std::vector<double> ys(n, 0);
        const auto ret = area_curve(xs, ys);
        for (const auto r:ret)
        {
            ASSERT_DOUBLE_EQ(0, r);
        }
    }
}

TEST_F(discretizeTest, area_curve_can_integrate_rectangle)
{
    for (size_t i = 0 ; i < 1000 ; ++i)
    {
        const double xa = a.random<double>();
        const double xb = a.random<double>().greater_than(xa);
        const double ya = a.random<double>().greater_than(0);
        const double yb = ya;
        const std::vector<double> xs = {xa, xb};
        const std::vector<double> ys = {ya, yb};
        const std::vector<double> ret = area_curve(xs, ys);
        ASSERT_DOUBLE_EQ(0, ret.at(0));
        ASSERT_DOUBLE_EQ((xb-xa)*ya, ret.at(1));
    }
}

TEST_F(discretizeTest, area_curve_can_integrate_trapeze)
{
    for (size_t i = 0 ; i < 1000 ; ++i)
    {
        const double xa = a.random<double>();
        const double xb = a.random<double>().greater_than(xa);
        const double ya = a.random<double>().greater_than(0);
        const double yb = a.random<double>().greater_than(0);
        const std::vector<double> xs = {xa, xb};
        const std::vector<double> ys = {ya, yb};
        const std::vector<double> ret = area_curve(xs, ys);
        ASSERT_DOUBLE_EQ(0, ret.at(0));
        ASSERT_DOUBLE_EQ((xb-xa)*(ya+yb)/2, ret.at(1));
    }
}

TEST_F(discretizeTest, area_curve_can_integrate_two_rectangles)
{
    for (size_t i = 0 ; i < 1000 ; ++i)
    {
        const double xa = a.random<double>();
        const double xb = a.random<double>().greater_than(xa);
        const double xc = a.random<double>().greater_than(xb);
        const double ya = a.random<double>().greater_than(0);
        const double yb = ya;
        const double yc = ya;
        const std::vector<double> xs = {xa, xb, xc};
        const std::vector<double> ys = {ya, yb, yc};
        const std::vector<double> ret = area_curve(xs, ys);
        ASSERT_DOUBLE_EQ(0, ret.at(0));
        ASSERT_SMALL_RELATIVE_ERROR((xb-xa)*ya, ret.at(1), EPS);
        ASSERT_SMALL_RELATIVE_ERROR((xc-xa)*ya, ret.at(2), EPS);
    }
}

TEST_F(discretizeTest, area_curve_can_integrate_rectangle_after_lots_of_zeros)
{
    for (size_t i = 0 ; i < 1000 ; ++i)
    {
        const size_t nb_of_zeros = a.random<size_t>().between(0, 10000);
        std::vector<double> xs = random_increasing_vector_of_size(nb_of_zeros+2);
        std::vector<double> ys(nb_of_zeros, 0);
        const double ya = a.random<double>().greater_than(0);
        const double yb = ya;
        ys.push_back(ya);
        ys.push_back(yb);
        const std::vector<double> ret = area_curve(xs, ys);
        ASSERT_EQ(nb_of_zeros+2, ret.size());
        for (size_t k = 0 ; k < nb_of_zeros ; ++k)
        {
            ASSERT_DOUBLE_EQ(0, ret.at(k));
        }
        ASSERT_SMALL_RELATIVE_ERROR(0.5*(xs[nb_of_zeros]-xs[nb_of_zeros-1])*ya, ret.at(nb_of_zeros), EPS);
        ASSERT_SMALL_RELATIVE_ERROR(ret.at(nb_of_zeros) + (xs[nb_of_zeros+1]-xs[nb_of_zeros])*ya, ret.at(nb_of_zeros+1), EPS);
    }
}

TEST_F(discretizeTest, can_find_given_area_in_area_curve)
{
    const std::vector<double> xs = {0, 8};
    const std::vector<double> ys = {0, 8};
    ASSERT_NEAR(2, find_integration_bound_yielding_target_area(2, xs, ys, area_curve(xs, ys)), EPS);
}

TEST_F(discretizeTest, can_find_given_area_in_area_curve_2)
{
    const std::vector<double> xs = {1,2,3,4,5};
    const std::vector<double> ys = {2,3,3,4,0};
    ASSERT_NEAR(1, find_integration_bound_yielding_target_area(0, xs, ys, area_curve(xs, ys)), EPS);
    ASSERT_NEAR(2, find_integration_bound_yielding_target_area(2.5, xs, ys, area_curve(xs, ys)), EPS);
    ASSERT_NEAR(3, find_integration_bound_yielding_target_area(5.5, xs, ys, area_curve(xs, ys)), EPS);
    ASSERT_NEAR(4, find_integration_bound_yielding_target_area(9, xs, ys, area_curve(xs, ys)), EPS);
    ASSERT_NEAR(5, find_integration_bound_yielding_target_area(11, xs, ys, area_curve(xs, ys)), EPS);
    ASSERT_NEAR(2.5, find_integration_bound_yielding_target_area(4, xs, ys, area_curve(xs, ys)), EPS);
}

TEST_F(discretizeTest, equal_area_abscissae_should_return_equally_spaced_values_for_test_case)
{
    //! [discretizeTest example]
    const std::vector<double> xs = {1,2,3,4,5.5};
    const std::vector<double> ys = {2,3,3,4,0};
    const auto res = equal_area_abscissae(xs, ys);
    ASSERT_EQ(5, res.size());
    ASSERT_NEAR(1, res[0], EPS);
    ASSERT_NEAR(2+1./6., res[1], EPS);
    ASSERT_NEAR(std::sqrt(10), res[2], EPS);
    ASSERT_NEAR(4, res[3], EPS);
    ASSERT_NEAR(5.5, res[4], EPS);
}

TEST_F(discretizeTest, equal_energy_bins)
{
//! [discretizeTest equal_area_abscissae_example]
    const double Hs = 3;
    const double Tp = 4;
    const double gamma = 1.4;
    const JonswapSpectrum S(Hs, Tp, gamma);
    const Cos2sDirectionalSpreading D(PI/4, 2);
    YamlStretching y;
    const Stretching s(y);
    const DiscreteDirectionalWaveSpectrum A = discretize(S, D, 0.01, 3, 1000, 1840, s, true);
//! [discretizeTest equal_area_abscissae_example]
//! [discretizeTest equal_area_abscissae_expected_output]
    ASSERT_EQ(1000, A.omega.size());
    ASSERT_NEAR(0.01, A.omega[0], EPS);
    ASSERT_NEAR(1.025029449067951, A.omega[1], EPS);
    ASSERT_NEAR(1.0525530074258682, A.omega[2], EPS);
    ASSERT_NEAR(1.0842190871695854, A.omega[4], EPS);
    ASSERT_NEAR(1.128270684351939, A.omega[9], EPS);
    ASSERT_NEAR(1.3472975072489475, A.omega[99], EPS);
    ASSERT_NEAR(3, A.omega[999], EPS);
    ASSERT_EQ(1840, A.psi.size());
    const DiscreteDirectionalWaveSpectrum B = discretize(S, D, 0.01, 3, 10, 17, s, true);
    ASSERT_EQ(10, B.omega.size());
    ASSERT_EQ(17, B.psi.size());
    ASSERT_NEAR(0, B.psi[0], EPS);
    ASSERT_NEAR(2*PI/17*1, B.psi[1], EPS);
    ASSERT_NEAR(2*PI/17*2, B.psi[2], EPS);
    ASSERT_NEAR(2*PI/17*3, B.psi[3], EPS);
    ASSERT_NEAR(2*PI/17*4, B.psi[4], EPS);
    ASSERT_NEAR(2*PI/17*5, B.psi[5], EPS);
    ASSERT_NEAR(2*PI/17*6, B.psi[6], EPS);
    ASSERT_NEAR(2*PI/17*7, B.psi[7], EPS);
    ASSERT_NEAR(2*PI/17*8, B.psi[8], EPS);
    ASSERT_NEAR(2*PI/17*9, B.psi[9], EPS);
    ASSERT_NEAR(2*PI/17*10, B.psi[10], EPS);
    ASSERT_NEAR(2*PI/17*11, B.psi[11], EPS);
    ASSERT_NEAR(2*PI/17*12, B.psi[12], EPS);
    ASSERT_NEAR(2*PI/17*13, B.psi[13], EPS);
    ASSERT_NEAR(2*PI/17*14, B.psi[14], EPS);
    ASSERT_NEAR(2*PI/17*15, B.psi[15], EPS);
    ASSERT_NEAR(2*PI/17*16, B.psi[16], EPS);
    ASSERT_NEAR(0.01, B.omega[0], EPS);
    ASSERT_NEAR(1.3266362675988099, B.omega[1], EPS);
    ASSERT_NEAR(1.4718706793919485, B.omega[2], EPS);
    ASSERT_NEAR(1.5894739415969286, B.omega[3], EPS);
    ASSERT_NEAR(1.6915444316496033, B.omega[4], EPS);
    ASSERT_NEAR(1.80136306541316, B.omega[5], EPS);
    ASSERT_NEAR(1.9390777413117828, B.omega[6], EPS);
    ASSERT_NEAR(2.1335838860523002, B.omega[7], EPS);
    ASSERT_NEAR(2.4226798199184207, B.omega[8], EPS);
    ASSERT_NEAR(3, B.omega[9], EPS);
    //! [discretizeTest equal_area_abscissae_expected_output]
}
