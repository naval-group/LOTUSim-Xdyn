#ifndef LINEARSTIFFNESSFORCEMODELTEST_HPP_
#define LINEARSTIFFNESSFORCEMODELTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator/DataGenerator.hpp>

class LinearStiffnessForceModelTest : public ::testing::Test
{
    protected:
        LinearStiffnessForceModelTest();
        virtual ~LinearStiffnessForceModelTest() = default;
        ssc::random_data_generator::DataGenerator a;
};

#endif  /* LINEARSTIFFNESSFORCEMODELTEST_HPP_ */