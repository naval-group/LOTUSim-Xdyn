/*
 * BasicBuoyancyForceModelTest.hpp
 *
 *  Created on: Jun 17, 2014
 *      Author: cady
 */

#ifndef BASICBUOYANCYFORCEMODELTEST_HPP_
#define BASICBUOYANCYFORCEMODELTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator.hpp>

class BasicBuoyancyForceModelTest : public ::testing::Test
{
    protected:
        BasicBuoyancyForceModelTest();
        virtual ~BasicBuoyancyForceModelTest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;
};

#endif  /* BASICBUOYANCYFORCEMODELTEST_HPP_ */