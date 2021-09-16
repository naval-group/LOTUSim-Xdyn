/*
 * StatesFilterTest.hpp
 *
 *  Created on: Sep 8, 2021
 *      Author: cady
 */


#ifndef STATESFILTERTEST_HPP_
#define STATESFILTERTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator.hpp>

class StatesFilterTest : public ::testing::Test
{
    protected:
        StatesFilterTest();
        virtual ~StatesFilterTest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;
};

#endif  /* STATESFILTERTEST_HPP_ */
