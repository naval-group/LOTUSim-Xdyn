/*
 * GrpcControllerInterfaceTest.cpp
 *
 *  Created on: Apr 20, 2021
 *      Author: cady
 */

#include "GrpcControllerInterface.hpp"
#include "GrpcControllerInterfaceTest.hpp"
#include "xdyn/exceptions/InvalidInputException.hpp"
#include "yaml_data.hpp"


GrpcControllerInterfaceTest::GrpcControllerInterfaceTest()
{
}

GrpcControllerInterfaceTest::~GrpcControllerInterfaceTest()
{
}

void GrpcControllerInterfaceTest::SetUp()
{
}

void GrpcControllerInterfaceTest::TearDown()
{
}


TEST_F (GrpcControllerInterfaceTest, can_parse_url)
{
    const auto input = GrpcControllerInterface::parse (test_data::gRPC_controller ());
    ASSERT_EQ ("pid:9002", input.url);
}

TEST_F (GrpcControllerInterfaceTest, should_throw_a_helpful_message_if_input_is_invalid)
{
    std::string error_msg;
    try
    {
        GrpcControllerInterface::parse ("some: invalid YAML");
    }
    catch (const InvalidInputException &e)
    {
        error_msg = e.get_message ();
    }
    ASSERT_EQ ("Unable to parse YAML data for a gRPC controller:\n"
               "yaml-cpp: error at line 1, column 1: key not found: url\n"
               "The offending YAML block was:\n"
               "some: invalid YAML",
               error_msg);
}

TEST_F (GrpcControllerInterfaceTest, can_parse_name)
{
    const auto input = GrpcControllerInterface::parse (test_data::gRPC_controller ());
    ASSERT_EQ ("PID", input.name);
}


TEST_F (GrpcControllerInterfaceTest, rest_of_yaml_should_be_available_for_use_by_grpc_method_set_parameters)
{
    const auto input = GrpcControllerInterface::parse (test_data::gRPC_controller ());
    ASSERT_EQ (test_data::gRPC_controller (), input.yaml);
}