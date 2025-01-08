/*
 * BasicBuoyancyForceModelTest.cpp
 *
 *  Created on: Jun 17, 2014
 *      Author: cady
 */

#include "BasicBuoyancyForceModelTest.hpp"
#include "BasicBuoyancyForceModel.hpp"
#include "xdyn/core/EnvironmentAndFrames.hpp"
#include "xdyn/core/unit_tests/generate_body_for_tests.hpp"
#include "xdyn/test_data_generator/yaml_data.hpp"
#include <ssc/kinematics.hpp>

#define BODY "body 1"

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

BasicBuoyancyForceModelTest::BasicBuoyancyForceModelTest() : a(ssc::random_data_generator::DataGenerator(123))
{
}

BasicBuoyancyForceModelTest::~BasicBuoyancyForceModelTest()
{
}

void BasicBuoyancyForceModelTest::SetUp()
{
}

void BasicBuoyancyForceModelTest::TearDown()
{
}

TEST_F(BasicBuoyancyForceModelTest, example)
{
//! [BasicBuoyancyForceModelTest example]
    EnvironmentAndFrames env;
    env.g = 9.81;
    env.rho = 1000;
    env.k = ssc::kinematics::KinematicsPtr(new ssc::kinematics::Kinematics());
    env.k->add(ssc::kinematics::Transform(ssc::kinematics::Point("NED"), BODY));
    const auto input = BasicBuoyancyForceModel::parse(test_data::basic_buoyancy_force());
    BasicBuoyancyForceModel F(input, BODY, env);
    ASSERT_EQ("basic buoyancy", F.model_name());
    BodyStates states = get_body(BODY)->get_states();
    const auto f = F(states, a.random<double>(), env);
//! [BasicBuoyancyForceModelTest example]
//! [BasicBuoyancyForceModelTest expected output]
    ASSERT_EQ(BODY, f.get_frame());
    ASSERT_DOUBLE_EQ(0, f.K());
    ASSERT_DOUBLE_EQ(0, f.M());
    ASSERT_DOUBLE_EQ(0, f.N());
    ASSERT_DOUBLE_EQ(0, f.X());
    ASSERT_DOUBLE_EQ(0, f.Y());
    ASSERT_DOUBLE_EQ(-16677, f.Z());
//! [BasicBuoyancyForceModelTest expected output]
}

TEST_F(BasicBuoyancyForceModelTest, example_with_an_orientation)
{
    using namespace ssc::kinematics;
    const double EPS = 1e-11;
    const double cosPhi   = 0.0;
    const double cosTheta = 0.5*sqrt(3);
    const double cosPsi   = 0.5;
    const double sinPhi   = 1.0;
    const double sinTheta = 0.5;
    const double sinPsi   = 0.5*sqrt(3);
    RotationMatrix rot;
    rot << cosTheta*cosPsi, sinPhi*sinTheta*cosPsi-cosPhi*sinPsi, cosPhi*sinTheta*cosPsi+sinPhi*sinPsi,
           cosTheta*sinPsi, sinPhi*sinTheta*sinPsi+cosPhi*cosPsi, cosPhi*sinTheta*sinPsi-sinPhi*cosPsi,
           -sinTheta,       sinPhi*cosTheta,                      cosPhi*cosTheta;
    EnvironmentAndFrames env;
    env.g = 9.81;
    env.rho = 1000;
    env.k = ssc::kinematics::KinematicsPtr(new ssc::kinematics::Kinematics());
    env.k->add(Transform(rot, "NED", BODY));
    const auto input = BasicBuoyancyForceModel::parse(test_data::basic_buoyancy_force());
    BasicBuoyancyForceModel F(input, BODY, env);
    BodyStates states = get_body(BODY)->get_states();

    const auto f = F(states,a.random<double>(), env);
    ASSERT_EQ(BODY, f.get_frame());
    ASSERT_NEAR(0, f.K(),EPS);
    ASSERT_NEAR(0, f.M(),EPS);
    ASSERT_NEAR(0, f.N(),EPS);
    ASSERT_NEAR(+0.5*16677, f.X(),EPS);
    ASSERT_NEAR(-0.5*sqrt(3.0)*16677, f.Y(),EPS);
    ASSERT_NEAR(0.0, f.Z(),EPS);
}

TEST_F(BasicBuoyancyForceModelTest, potential_energy)
{
    std::vector<double> x(3,0);
    using namespace ssc::kinematics;
    EnvironmentAndFrames env;
    env.g = 9.81;
    env.rho = 1000;
    env.k = ssc::kinematics::KinematicsPtr(new ssc::kinematics::Kinematics());
    env.k->add(ssc::kinematics::Transform(ssc::kinematics::Point("NED"), BODY));
    const auto input = BasicBuoyancyForceModel::parse(test_data::basic_buoyancy_force());
    BasicBuoyancyForceModel F(input, BODY, env);
    BodyStates states = get_body(BODY)->get_states();

    ASSERT_DOUBLE_EQ(0, F.potential_energy(states, x));
    x[2] = 12.3;
    ASSERT_DOUBLE_EQ(205127.1, F.potential_energy(states, x));
}
