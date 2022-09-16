/*
 * GrpcControllerInterfaceTest.hpp
 *
 *  Created on: Apr 20, 2021
 *      Author: cady
 */

#ifndef GRPCCONTROLLERINTERFACETESTHPP
#define GRPCCONTROLLERINTERFACETESTHPP

#include "gtest/gtest.h"
#include <ssc/random_data_generator/DataGenerator.hpp>

class GrpcControllerInterfaceTest : public ::testing::Test
{
    protected:
        GrpcControllerInterfaceTest();
        virtual ~GrpcControllerInterfaceTest();
        virtual void SetUp();
        virtual void TearDown();
};


#endif /* GRPCCONTROLLERINTERFACETESTHPP */
