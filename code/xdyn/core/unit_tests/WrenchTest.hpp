#ifndef CORE_UNIT_TESTS_INC_WRENCHTEST_HPP_
#define CORE_UNIT_TESTS_INC_WRENCHTEST_HPP_

#include "xdyn/core/Wrench.hpp"

#include "gtest/gtest.h"
#include <ssc/random_data_generator/DataGenerator.hpp>

class WrenchTest : public ::testing::Test
{
public:
    WrenchTest();
    virtual ~WrenchTest();
    ssc::random_data_generator::DataGenerator a;
};

#endif /* CORE_UNIT_TESTS_INC_WRENCHTEST_HPP_ */
