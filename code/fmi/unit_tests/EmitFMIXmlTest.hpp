/*
 * EmitFMIXmlTest.hpp
 *
 *  Created on: May 22, 2015
 *      Author: cady
 */

#ifndef EMITFMIXMLTEST_HPP_
#define EMITFMIXMLTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator/DataGenerator.hpp>

class EmitFMIXmlTest : public ::testing::Test
{
    protected:
        EmitFMIXmlTest();
        virtual ~EmitFMIXmlTest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;
};

#endif  /* EMITFMIXMLTEST_HPP_ */
