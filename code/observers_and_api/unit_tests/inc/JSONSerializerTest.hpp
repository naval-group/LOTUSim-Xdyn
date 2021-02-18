#ifndef OBSERVERS_AND_API_UNIT_TESTS_JSONSERIALIZERTEST_HPP_
#define OBSERVERS_AND_API_UNIT_TESTS_JSONSERIALIZERTEST_HPP_

#include <gtest/gtest.h>
#include <ssc/random_data_generator/DataGenerator.hpp>

class JSONSerializerTest : public testing::Test
{
public:
    JSONSerializerTest();
    virtual ~JSONSerializerTest();

    ssc::random_data_generator::DataGenerator a;
    const std::string yaml;
};

#endif /* OBSERVERS_AND_API_UNIT_TESTS_JSONSERIALIZERTEST_HPP_ */
