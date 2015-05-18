/*
 * FMITest.cpp
 *
 *  Created on: May 12, 2015
 *      Author: cady
 */

#include "FMITest.hpp"

FMITest::FMITest() : a(ssc::random_data_generator::DataGenerator(21213))
{
}

FMITest::~FMITest()
{
}

void FMITest::SetUp()
{
}

void FMITest::TearDown()
{
}

TEST_F(FMITest, example)
{
//! [FMITest example]
//! [FMITest example]
//! [FMITest expected output]
//! [FMITest expected output]
}



