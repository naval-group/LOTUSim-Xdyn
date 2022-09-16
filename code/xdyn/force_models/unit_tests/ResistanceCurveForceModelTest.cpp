/*
 * ResistanceCurveForceModelTest.cpp
 *
 *  Created on: Oct 24, 2014
 *      Author: cady
 */

#include "xdyn/core/Body.hpp"
#include "ResistanceCurveForceModel.hpp"
#include "ResistanceCurveForceModelTest.hpp"
#include "test_data_generator/yaml_data.hpp"
#include "xdyn/core/EnvironmentAndFrames.hpp"

#define EPS 1E-6

ResistanceCurveForceModelTest::ResistanceCurveForceModelTest() : a(ssc::random_data_generator::DataGenerator(89451))
{
}

ResistanceCurveForceModelTest::~ResistanceCurveForceModelTest()
{
}

void ResistanceCurveForceModelTest::SetUp()
{
}

void ResistanceCurveForceModelTest::TearDown()
{
}

TEST_F(ResistanceCurveForceModelTest, parser)
{
    const auto r = ResistanceCurveForceModel::parse(test_data::resistance_curve());
    ASSERT_EQ(8, r.R.size());
    ASSERT_EQ(8, r.Va.size());

    const double knot = 0.51444444444444444;
    ASSERT_DOUBLE_EQ(0, r.Va[0]);
    ASSERT_DOUBLE_EQ(1*knot, r.Va[1]);
    ASSERT_DOUBLE_EQ(2*knot, r.Va[2]);
    ASSERT_DOUBLE_EQ(3*knot, r.Va[3]);
    ASSERT_DOUBLE_EQ(4*knot, r.Va[4]);
    ASSERT_DOUBLE_EQ(5*knot, r.Va[5]);
    ASSERT_DOUBLE_EQ(15*knot, r.Va[6]);
    ASSERT_DOUBLE_EQ(20*knot, r.Va[7]);

    ASSERT_DOUBLE_EQ(0, r.R[0]);
    ASSERT_DOUBLE_EQ(1E6, r.R[1]);
    ASSERT_DOUBLE_EQ(4E6, r.R[2]);
    ASSERT_DOUBLE_EQ(9E6, r.R[3]);
    ASSERT_DOUBLE_EQ(16E6, r.R[4]);
    ASSERT_DOUBLE_EQ(25E6, r.R[5]);
    ASSERT_DOUBLE_EQ(225E6, r.R[6]);
    ASSERT_DOUBLE_EQ(400E6, r.R[7]);
}

TEST_F(ResistanceCurveForceModelTest, example)
{
//! [ResistanceCurveForceModelTest example]
    EnvironmentAndFrames env;
    ResistanceCurveForceModel F(ResistanceCurveForceModel::parse(test_data::resistance_curve()), "", env);
//! [ResistanceCurveForceModelTest example]
    ASSERT_EQ("resistance curve",F.model_name());
//! [ResistanceCurveForceModelTest expected output]
    BodyStates states;
    states.convention.convention = {"z", "y'", "x''"};
    states.convention.order_by = "angle";
    states.u.record(0, 0);
    auto force = F.get_force(states, a.random<double>(), env, {});
    ASSERT_DOUBLE_EQ(0, force.X());
    ASSERT_DOUBLE_EQ(0, force.Y());
    ASSERT_DOUBLE_EQ(0, force.Z());
    ASSERT_DOUBLE_EQ(0, force.K());
    ASSERT_DOUBLE_EQ(0, force.M());
    ASSERT_DOUBLE_EQ(0, force.N());

    states.u.record(1, 0.514444444444444);
    force = F.get_force(states, a.random<double>(), env, {});
    ASSERT_NEAR(-1e6, force.X(), EPS);
    ASSERT_DOUBLE_EQ(0, force.Y());
    ASSERT_DOUBLE_EQ(0, force.Z());
    ASSERT_DOUBLE_EQ(0, force.K());
    ASSERT_DOUBLE_EQ(0, force.M());
    ASSERT_DOUBLE_EQ(0, force.N());

    states.u.record(2, 0.514444444444444*15);
    force = F.get_force(states, a.random<double>(), env, {});
    ASSERT_NEAR(-225e6, force.X(), EPS);
    ASSERT_DOUBLE_EQ(0, force.Y());
    ASSERT_DOUBLE_EQ(0, force.Z());
    ASSERT_DOUBLE_EQ(0, force.K());
    ASSERT_DOUBLE_EQ(0, force.M());
    ASSERT_DOUBLE_EQ(0, force.N());
//! [ResistanceCurveForceModelTest expected output]
}

TEST_F(ResistanceCurveForceModelTest, should_issue_a_warning_when_speed_is_lower_than_min_speed_specified_in_resistance_curve_table)
{
    EnvironmentAndFrames env;
    ResistanceCurveForceModel F(ResistanceCurveForceModel::parse(test_data::resistance_curve()), "", env);
    BodyStates states;
    states.convention.convention = {"z", "y'", "x''"};
    states.convention.order_by = "angle";
    std::stringstream error;
    // Redirect cerr to our stringstream buffer or any other ostream
    std::streambuf* orig =std::cerr.rdbuf(error.rdbuf());
    ASSERT_TRUE(error.str().empty());
    // Call the resistance curve model
    states.u.record(0, -1);
    F.get_force(states, a.random<double>(), env, {});
    ASSERT_FALSE(error.str().empty());
    // Restore cerr's buffer
    std::cerr.rdbuf(orig);
}

TEST_F(ResistanceCurveForceModelTest, should_issue_a_warning_when_speed_is_greater_than_max_speed_specified_in_resistance_curve_table)
{
    EnvironmentAndFrames env;
    ResistanceCurveForceModel F(ResistanceCurveForceModel::parse(test_data::resistance_curve()), "", env);
    BodyStates states;
    states.convention.convention = {"z", "y'", "x''"};
    states.convention.order_by = "angle";
    std::stringstream error;
    // Redirect cerr to our stringstream buffer or any other ostream
    std::streambuf* orig =std::cerr.rdbuf(error.rdbuf());
    ASSERT_TRUE(error.str().empty());
    // Call the resistance curve model
    states.u.record(0, 20*0.514444444444444444 + 1E-10);
    F.get_force(states, a.random<double>(), env, {});
    ASSERT_FALSE(error.str().empty());
    // Restore cerr's buffer
    std::cerr.rdbuf(orig);
}


TEST_F(ResistanceCurveForceModelTest, with_filtered_states)
{
    EnvironmentAndFrames env;
    ResistanceCurveForceModel F(ResistanceCurveForceModel::parse(test_data::resistance_curve()), "", env);
    YamlFilteredStates filters;
    filters.u = "type of filter: moving average\n"
                "duration in seconds : 9";
    BodyStates states(filters, 9);
    states.convention.convention = {"z", "y'", "x''"};
    states.convention.order_by = "angle";
    const double knot = 1852.0/3600.0;
    for (const auto i:{0,1,2,3,4,5,6,7,8,9})
    {
        states.u.record((double)i, i*knot);
    }
    auto force = F.get_force(states, a.random<double>(), env, {});
    const double average_speed_over_nine_seconds_in_knots = 4.5;
    const double interpolated_resistance = average_speed_over_nine_seconds_in_knots*average_speed_over_nine_seconds_in_knots;
    const double MN = 1E6; // 1 mega Newton
    ASSERT_DOUBLE_EQ(-interpolated_resistance*MN, force.X());
    ASSERT_DOUBLE_EQ(0, force.Y());
    ASSERT_DOUBLE_EQ(0, force.Z());
    ASSERT_DOUBLE_EQ(0, force.K());
    ASSERT_DOUBLE_EQ(0, force.M());
    ASSERT_DOUBLE_EQ(0, force.N());
}