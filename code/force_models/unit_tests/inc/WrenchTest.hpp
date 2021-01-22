#ifndef FORCE_MODELS_UNIT_TESTS_INC_WRENCHTEST_HPP_
#define FORCE_MODELS_UNIT_TESTS_INC_WRENCHTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator/DataGenerator.hpp>
#include "Wrench.hpp"

class WrenchTest : public ::testing::Test
{
public:
    WrenchTest();
    virtual ~WrenchTest();
    ssc::random_data_generator::DataGenerator a;
};

#endif /* FORCE_MODELS_UNIT_TESTS_INC_WRENCHTEST_HPP_ */
