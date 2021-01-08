#ifndef FORCE_MODELS_UNIT_TESTS_INC_HOLTROPMENNENFORCEMODELTEST_HPP_
#define FORCE_MODELS_UNIT_TESTS_INC_HOLTROPMENNENFORCEMODELTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator/DataGenerator.hpp>
#include "HoltropMennenForceModel.hpp"

class HoltropMennenForceModelTest : public ::testing::Test
{
protected:
    HoltropMennenForceModelTest();
    virtual ~HoltropMennenForceModelTest();
    virtual void SetUp();
    virtual void TearDown();
    ssc::random_data_generator::DataGenerator a;
    EnvironmentAndFrames env;
};

#endif /* FORCE_MODELS_UNIT_TESTS_INC_HOLTROPMENNENFORCEMODELTEST_HPP_ */
