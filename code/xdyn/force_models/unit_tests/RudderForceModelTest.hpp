/*
 * RudderForceModelTest.hpp
 *
 *  Created on: Feb 2, 2015
 *      Author: cady
 */


#ifndef RUDDERFORCEMODELTEST_HPP_
#define RUDDERFORCEMODELTEST_HPP_

#include "xdyn/environment_models/WaveModel.hpp"
#include <ssc/random_data_generator/DataGenerator.hpp>
#include <ssc/macros.hpp>
#include "gtest/gtest.h"

class RudderForceModelTest : public ::testing::Test
{
    protected:
        RudderForceModelTest();
        virtual ~RudderForceModelTest();
        virtual void SetUp();
        virtual void TearDown();
        TR1(shared_ptr)<WaveModel> get_wave_model() const;
        ssc::random_data_generator::DataGenerator a;
};

#endif  /* RUDDERFORCEMODELTEST_HPP_ */
