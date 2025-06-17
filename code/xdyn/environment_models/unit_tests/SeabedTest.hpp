/*
 * SeabedTest.hpp
 *
 *  Created on: 3 June 2025
 *      Author: julien.prodhon
 */

#ifndef ENVIRONMENT_MODELS_UNIT_TESTS_INC_DEFAULTWINDMODELTEST_HPP_
#define ENVIRONMENT_MODELS_UNIT_TESTS_INC_DEFAULTWINDMODELTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator.hpp>
#include "Seabed.hpp"

class SeabedTest : public ::testing::Test
{
    protected:
        SeabedTest ();
        virtual ~SeabedTest ();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;
};

#endif /* SEABEDTEST_HPP_ */