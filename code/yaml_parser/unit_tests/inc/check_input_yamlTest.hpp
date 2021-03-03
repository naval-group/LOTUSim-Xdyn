#ifndef CHECKINPUTYAMLTEST_HPP_
#define CHECKINPUTYAMLTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator.hpp>
#include "YamlSimulatorInput.hpp"
#include "check_input_yaml.hpp"

class check_input_yamlTest : public ::testing::Test
{
    protected:
        check_input_yamlTest();
        virtual ~check_input_yamlTest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;
};

#endif  /* CHECKINPUTYAMLTEST_HPP_ */
