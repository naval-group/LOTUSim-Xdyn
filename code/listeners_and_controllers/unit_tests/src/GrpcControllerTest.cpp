/*
 * GrpcControllerTest.cpp
 *
 *  Created on: Apr 19, 2021
 *      Author: cady
 */

#include "GrpcController.hpp"
#include "GrpcControllerTest.hpp"
#include "yaml_data.hpp"


GrpcControllerTest::GrpcControllerTest()
{
}

GrpcControllerTest::~GrpcControllerTest()
{
}

void GrpcControllerTest::SetUp()
{
}

void GrpcControllerTest::TearDown()
{
}

TEST_F(GrpcControllerTest, can_parse_url)
{
    GrpcController::parse(test_data::gRPC_controller());
}