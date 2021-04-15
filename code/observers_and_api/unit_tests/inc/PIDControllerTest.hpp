/*
 * PIDControllerTest.hpp
 *
 *  Created on: Fev 12, 2021
 *      Author: lincker
 */

#ifndef PIDCONTROLLERTEST_HPP_
#define PIDCONTROLLERTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator/DataGenerator.hpp>

class PIDControllerTest : public ::testing::Test
{
    protected:
        PIDControllerTest();
        virtual ~PIDControllerTest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;
        std::string pid_specific_yaml(const double Kp, const double Ki, const double Kd,
                                      const std::string& setpoint_name,
                                      const std::string& command_name
                                      );
};

#endif  /* PIDCONTROLLERTEST_HPP_ */
