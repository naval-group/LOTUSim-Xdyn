#ifndef MMGMANEUVERINGFORCEMODELTEST_HPP_
#define MMGMANEUVERINGFORCEMODELTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator/DataGenerator.hpp>

class MMGManeuveringForceModelTest : public ::testing::Test
{
    protected:
        MMGManeuveringForceModelTest();
        virtual ~MMGManeuveringForceModelTest();
        ssc::random_data_generator::DataGenerator a;
};

#endif  /* MMGMANEUVERINGFORCEMODELTEST_HPP_ */