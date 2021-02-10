#ifndef OBSERVERS_AND_API_UNIT_TESTS_INC_COSIMULATIONOBSERVERTEST_HPP_
#define OBSERVERS_AND_API_UNIT_TESTS_INC_COSIMULATIONOBSERVERTEST_HPP_

#include <gtest/gtest.h>
#include <ssc/random_data_generator.hpp>

class CoSimulationObserverTest : public testing::Test
{
public:
    CoSimulationObserverTest();
    virtual ~CoSimulationObserverTest();
private:
    ssc::random_data_generator::DataGenerator a;
};

#endif /* OBSERVERS_AND_API_UNIT_TESTS_INC_COSIMULATIONOBSERVERTEST_HPP_ */
