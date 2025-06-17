/*
 * EkmanUWCurrentTest.hpp
 *
 *  Created on: 3 June 2025
 *      Author: julien.prodhon
 */

#ifndef ENVIRONMENT_MODELS_UNIT_TESTS_INC_DEFAULTWINDMODELTEST_HPP_
#define ENVIRONMENT_MODELS_UNIT_TESTS_INC_DEFAULTWINDMODELTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator.hpp>

class EkmanUWCurrentTest : public ::testing::Test
{
    protected:
        EkmanUWCurrentTest ();
        virtual ~EkmanUWCurrentTest ();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;
};

#endif /* ENVIRONMENT_MODELS_UNIT_TESTS_INC_DEFAULTWINDMODELTEST_HPP_ */
