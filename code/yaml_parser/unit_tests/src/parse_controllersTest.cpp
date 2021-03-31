/*
 * parse_controllersTest.cpp
 *
 *  Created on: Oct 22, 2014
 *      Author: cady
 */

#include "parse_controllersTest.hpp"
#include "parse_controllers.hpp"
#include "yaml_data.hpp"

parse_controllersTest::parse_controllersTest() : a(ssc::random_data_generator::DataGenerator(542121))
{
}

parse_controllersTest::~parse_controllersTest()
{
}

void parse_controllersTest::SetUp()
{
}

void parse_controllersTest::TearDown()
{
}

TEST_F(parse_controllersTest, example)
{
//! [parse_controllersTest example]
    std::vector<YamlController> controllers = parse_controller_yaml(test_data::controllers());
    ASSERT_EQ(3, controllers.size());

    ASSERT_EQ("propeller", controllers[0].name);
    ASSERT_EQ("rpm", controllers[0].output);
    ASSERT_EQ("PID", controllers[0].type);
    ASSERT_EQ(0.1, controllers[0].dt);
    ASSERT_EQ("rpm_co", controllers[0].setpoint);
    ASSERT_EQ(2, controllers[0].state_weights.size());
    ASSERT_EQ(1, controllers[0].state_weights["x"]);
    ASSERT_EQ(-1, controllers[0].state_weights["y"]);

    ASSERT_EQ("propeller", controllers[1].name);
    ASSERT_EQ("P/D", controllers[1].output);
    ASSERT_EQ("gRPC", controllers[1].type);
    ASSERT_EQ(0.01, controllers[1].dt);
    ASSERT_EQ("P/D", controllers[1].setpoint);
    ASSERT_EQ(1, controllers[1].state_weights.size());
    ASSERT_EQ(2, controllers[1].state_weights["u"]);

    ASSERT_EQ("controller", controllers[2].name);
    ASSERT_EQ("psi", controllers[2].output);
    ASSERT_EQ("PID", controllers[2].type);
    ASSERT_EQ(0.5, controllers[2].dt);
    ASSERT_EQ("psi_co", controllers[2].setpoint);
    ASSERT_EQ(1, controllers[2].state_weights.size());
    ASSERT_EQ(1, controllers[2].state_weights["psi_est"]);
//! [parse_controllersTest example]
}
