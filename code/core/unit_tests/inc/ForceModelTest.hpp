/*
 * ForceModelTest.hpp
 *
 *  Created on: May 11, 2015
 *      Author: cady
 */


#ifndef FORCEMODELTEST_HPP_
#define FORCEMODELTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator/DataGenerator.hpp>

class ForceModelTest : public ::testing::Test
{
    protected:
        ForceModelTest();
        virtual ~ForceModelTest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;
};

#endif  /* FORCEMODELTEST_HPP_ */
