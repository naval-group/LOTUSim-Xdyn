#ifndef STL_READER_TEST_HPP
#define STL_READER_TEST_HPP

#include "gtest/gtest.h"
#include <ssc/random_data_generator.hpp>

class StlReaderTest : public ::testing::Test
{
    protected:
        StlReaderTest() : a(65478) {}
        virtual void SetUp(){}
        virtual void TearDown(){}
        std::string random_string_of_size(const size_t n);
        ssc::random_data_generator::DataGenerator a;
};


#endif