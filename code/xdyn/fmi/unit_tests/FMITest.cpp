/*
 * FMITest.cpp
 *
 *  Created on: May 12, 2015
 *      Author: cady
 */

#include "gmock/gmock.h"

#include "FMITest.hpp"
#include "FMI.hpp"
#include "test_data_generator/yaml_data.hpp"

using ::testing::ElementsAre;


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
    fmi::API fmi("test", fmiCallbackFunctions(), false, test_data::fmi());

//! [FMITest example]
//! [FMITest expected output]
//! [FMITest expected output]
}

TEST_F(FMITest, can_get_all_commands)
{
    fmi::API fmi("test", fmiCallbackFunctions(), false, test_data::fmi());
    const std::vector<std::string> command_names = fmi.get_command_names();
    ASSERT_THAT(command_names, ElementsAre("PropRudd(rpm)","PropRudd(P/D)","PropRudd(beta)"));
}
