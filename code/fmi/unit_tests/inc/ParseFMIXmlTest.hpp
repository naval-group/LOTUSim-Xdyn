/*
 * ParseFMIXmlTest.hpp
 *
 *  Created on: May 21, 2015
 *      Author: cady
 */

#ifndef PARSEFMIXMLTEST_HPP_
#define PARSEFMIXMLTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator/DataGenerator.hpp>

class ParseFMIXmlTest : public ::testing::Test
{
    protected:
        ParseFMIXmlTest();
        virtual ~ParseFMIXmlTest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;
};

#endif  /* PARSEFMIXMLTEST_HPP_ */
