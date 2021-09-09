/*
 * StatesFilterTest.cpp
 *
 *  Created on: Sep 08, 2021
 *      Author: cady
 */


#include "AbstractStates.hpp"
#include "History.hpp"
#include "InvalidInputException.hpp"
#include "StatesFilter.hpp"
#include "StatesFilterTest.hpp"
#include "YamlBody.hpp"
#include "YamlRotation.hpp"
#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

StatesFilterTest::StatesFilterTest() :  a(ssc::random_data_generator::DataGenerator(556677))
{
}

StatesFilterTest::~StatesFilterTest()
{
}

void StatesFilterTest::SetUp()
{
}

void StatesFilterTest::TearDown()
{
}

TEST_F(StatesFilterTest, should_throw_if_type_of_filter_is_unknown)
{
    const std::string yaml = "type of filter: some unknown filter\n"
                             "duration in seconds : 3";
    ASSERT_THROW(StateFilter::build(yaml), InvalidInputException);
}

TEST_F(StatesFilterTest, should_not_throw_if_yaml_is_valid)
{
    const std::string yaml = "type of filter: moving average\n"
                             "duration in seconds : 3";
    ASSERT_NO_THROW(StateFilter::build(yaml));
}

TEST_F(StatesFilterTest, if_duration_is_zero_no_filtering_should_take_place)
{
    History h(3);
    const double last_value = a.random<double>();
    h.record(0, 1);
    h.record(1, 2);
    h.record(2, last_value);
    const std::string yaml = "type of filter: moving average\n"
                             "duration in seconds : 0";
    const auto filter = StateFilter::build(yaml);
    ASSERT_DOUBLE_EQ(last_value, filter->get_value(h));
}

TEST_F(StatesFilterTest, should_be_able_to_filter)
{
    History h(3);
    h.record(0, 1);
    h.record(1, 2);
    h.record(2, 3);
    const std::string yaml = "type of filter: moving average\n"
                             "duration in seconds : 1";
    const auto filter = StateFilter::build(yaml);
    ASSERT_DOUBLE_EQ(2.5, filter->get_value(h));
}


TEST_F(StatesFilterTest, should_be_able_to_filter_all_states)
{
    YamlFilteredStates input;
    input.x = "type of filter: moving average\n"
              "duration in seconds : 1";
    input.y = "type of filter: moving average\n"
              "duration in seconds : 2";
    input.z = "type of filter: moving average\n"
              "duration in seconds : 3";
    input.u = "type of filter: moving average\n"
              "duration in seconds : 4";
    input.v = "type of filter: moving average\n"
              "duration in seconds : 5";
    input.w = "type of filter: moving average\n"
              "duration in seconds : 6";
    input.p = "type of filter: moving average\n"
              "duration in seconds : 7";
    input.q = "type of filter: moving average\n"
              "duration in seconds : 8";
    input.r = "type of filter: moving average\n"
              "duration in seconds : 9";
    input.phi = "type of filter: moving average\n"
              "duration in seconds : 1";
    input.theta = "type of filter: moving average\n"
              "duration in seconds : 1";
    input.psi = "type of filter: moving average\n"
              "duration in seconds : 1";
    const StatesFilter filters(input);
    AbstractStates<History> states(10);
    states.x.record(0, 1);
    states.x.record(1, 2);
    states.x.record(2, 3);
    states.y.record(0, 4);
    states.y.record(1, 5);
    states.y.record(3, 6);
    states.z.record(0, 7);
    states.z.record(1, 8);
    states.z.record(4, 9);
    states.u.record(0, 11);
    states.u.record(1, 12);
    states.u.record(5, 13);
    states.v.record(0, 14);
    states.v.record(1, 15);
    states.v.record(6, 16);
    states.w.record(0, 17);
    states.w.record(1, 18);
    states.w.record(7, 19);
    states.p.record(0, 21);
    states.p.record(1, 22);
    states.p.record(8, 23);
    states.q.record(0, 24);
    states.q.record(1, 25);
    states.q.record(9, 26);
    states.r.record(0, 27);
    states.r.record(1, 28);
    states.r.record(10, 29);
    states.qr.record(0, 0.5);
    states.qr.record(1, 0.5);
    states.qr.record(2, 0.5);
    states.qi.record(0, 0.5);
    states.qi.record(1, 0.5);
    states.qi.record(2, 0.5);
    states.qj.record(0, 0.5);
    states.qj.record(1, 0.5);
    states.qj.record(2, 0.5);
    states.qk.record(0, 0.5);
    states.qk.record(1, 0.5);
    states.qk.record(2, 0.5);

    const double eps = 1E-10;
    ASSERT_DOUBLE_EQ(2.5, filters.get_filtered_x(states));
    ASSERT_DOUBLE_EQ(5.5, filters.get_filtered_y(states));
    ASSERT_DOUBLE_EQ(8.5, filters.get_filtered_z(states));
    ASSERT_DOUBLE_EQ(12.5, filters.get_filtered_u(states));
    ASSERT_DOUBLE_EQ(15.5, filters.get_filtered_v(states));
    ASSERT_DOUBLE_EQ(18.5, filters.get_filtered_w(states));
    ASSERT_DOUBLE_EQ(22.5, filters.get_filtered_p(states));
    ASSERT_DOUBLE_EQ(25.5, filters.get_filtered_q(states));
    ASSERT_DOUBLE_EQ(28.5, filters.get_filtered_r(states));
    const YamlRotation rot("angle", {"z", "y'", "x''"});
    ASSERT_NEAR(PI/2, filters.get_filtered_phi(states, rot), eps);
    ASSERT_NEAR(0, filters.get_filtered_theta(states, rot), eps);
    ASSERT_NEAR(PI/2, filters.get_filtered_psi(states, rot), eps);
}