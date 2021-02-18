#ifndef OBSERVERS_AND_API_UNIT_TESTS_INC_SIMULATIONSERVEROBSERVERTEST_HPP_
#define OBSERVERS_AND_API_UNIT_TESTS_INC_SIMULATIONSERVEROBSERVERTEST_HPP_

#include <gtest/gtest.h>
#include <ssc/random_data_generator.hpp>

class SimulationServerObserverTest : public testing::Test
{
public:
    SimulationServerObserverTest();
    virtual ~SimulationServerObserverTest();
private:
    ssc::random_data_generator::DataGenerator a;
};

#endif /* OBSERVERS_AND_API_UNIT_TESTS_INC_SIMULATIONSERVEROBSERVERTEST_HPP_ */
