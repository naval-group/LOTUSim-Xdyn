/*
 * GrpcControllerTest.cpp
 *
 *  Created on: Apr 19, 2021
 *      Author: cady
 */

#include "GrpcController.hpp"
#include "GrpcControllerTest.hpp"
#include "InvalidInputException.hpp"
#include "yaml_data.hpp"

GrpcControllerTest::GrpcControllerTest () {}

GrpcControllerTest::~GrpcControllerTest () {}

void
GrpcControllerTest::SetUp ()
{
}

void
GrpcControllerTest::TearDown ()
{
}

TEST_F (GrpcControllerTest, can_parse_url)
{
    const auto input = GrpcController::parse (test_data::gRPC_controller ());
    ASSERT_EQ ("pid:9002", input.url);
}

TEST_F (GrpcControllerTest, should_throw_a_helpful_message_if_input_is_invalid)
{
    std::string error_msg;
    try
    {
        GrpcController::parse ("some: invalid YAML");
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