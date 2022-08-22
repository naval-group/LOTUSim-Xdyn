/*
 * FMITest.hpp
 *
 *  Created on: May 12, 2015
 *      Author: cady
 */

#ifndef FMITEST_HPP_
#define FMITEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator/DataGenerator.hpp>

class FMITest : public ::testing::Test
{
    protected:
        FMITest();
        virtual ~FMITest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;
};

#endif  /* FMITEST_HPP_ */
