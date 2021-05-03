/*
 * PrecalParserTest.hpp
 *
 *  Created on: May 03, 2021
 *      Author: cady
 */

#ifndef PRECALPARSERTEST_HPP_
#define PRECALPARSERTEST_HPP_

#include "gtest/gtest.h"

class PrecalParserTest : public ::testing::Test
{
    protected:
        PrecalParserTest();
        virtual ~PrecalParserTest();
        virtual void SetUp();
        virtual void TearDown();
};

#endif  /* PRECALPARSERTEST_HPP_ */