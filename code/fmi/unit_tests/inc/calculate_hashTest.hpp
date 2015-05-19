/*
 * calculate_hashTest.hpp
 *
 *  Created on: May 19, 2015
 *      Author: cady
 */

#ifndef CALCULATE_HASHTEST_HPP_
#define CALCULATE_HASHTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator/DataGenerator.hpp>

class calculate_hashTest : public ::testing::Test
{
    protected:
        calculate_hashTest();
        virtual ~calculate_hashTest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;
};

#endif  /* CALCULATE_HASHTEST_HPP_ */
