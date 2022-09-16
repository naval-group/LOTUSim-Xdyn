/*
 * parse_controllersTest.cpp
 *
 *  Created on: Oct 22, 2014
 *      Author: cady
 */

#include "parse_controllersTest.hpp"
#include "parse_controllers.hpp"
#include "test_data_generator/yaml_data.hpp"

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
    std::vector<YamlController> controllers = parse_controller_yaml(test_data::controllers()+test_data::grpc_controller());
    ASSERT_EQ(3, controllers.size());

    ASSERT_EQ("pId", controllers[0].type);
    ASSERT_EQ(0.1, controllers[0].dt);

    ASSERT_EQ("gRPC", controllers[2].type);
    ASSERT_EQ(0.01, controllers[2].dt);

    ASSERT_EQ("PID", controllers[1].type);
    ASSERT_EQ(0.5, controllers[1].dt);
//! [parse_controllersTest example]
}
