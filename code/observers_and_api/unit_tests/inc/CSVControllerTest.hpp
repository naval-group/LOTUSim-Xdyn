/*
 * CSVControllerTest.hpp
 *
 *  Created on: Mar 16, 2022
 *      Author: cady
 */

#ifndef CSVCONTROLLERTEST_HPP_
#define CSVCONTROLLERTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator/DataGenerator.hpp>
#include "TempFile.hpp"

class CSVControllerTest : public ::testing::Test
{
    protected:
      CSVControllerTest();
      virtual ~CSVControllerTest();
      virtual void SetUp();
      virtual void TearDown();
      std::string test_yaml() const;
      TempFile csv;
      ssc::random_data_generator::DataGenerator a;
};

#endif  /* CSVCONTROLLERTEST_HPP_ */
