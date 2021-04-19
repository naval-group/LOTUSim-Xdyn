/*
 * GrpcControllerTest.hpp
 *
 *  Created on: Apr 19, 2021
 *      Author: cady
 */

#ifndef GRPC_UNIT_TESTS_INC_GRPCCONTROLLERTEST_HPP_
#define GRPC_UNIT_TESTS_INC_GRPCCONTROLLERTEST_HPP_

#include "gtest/gtest.h"

class GrpcControllerTest : public ::testing::Test
{
    protected:
        GrpcControllerTest();
        virtual ~GrpcControllerTest();
        virtual void SetUp();
        virtual void TearDown();
};


#endif /* GRPC_UNIT_TESTS_INC_GRPCCONTROLLERTEST_HPP_ */
