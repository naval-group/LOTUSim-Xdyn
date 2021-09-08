/*
 * StatesFilterTest.cpp
 *
 *  Created on: Sep 08, 2021
 *      Author: cady
 */


#include "BlockedDOF.hpp"
#include "InvalidInputException.hpp"
#include "StatesFilter.hpp"
#include "StatesFilterTest.hpp"

StatesFilterTest::StatesFilterTest()
{
}

StatesFilterTest::~StatesFilterTest()
{
}

void StatesFilterTest::SetUp()
{
}

void StatesFilterTest::TearDown()
{
}

TEST_F(StatesFilterTest, should_throw_if_type_of_filter_is_unknown)
{
    const std::string yaml = "type of filter: some unknown filter\n"
                             "duration in seconds : 3";
    ASSERT_THROW(StatesFilter::build(yaml), InvalidInputException);
}

TEST_F(StatesFilterTest, should_not_throw_if_yaml_is_valid)
{
    const std::string yaml = "type of filter: moving average\n"
                             "duration in seconds : 3";
    ASSERT_NO_THROW(StatesFilter::build(yaml));
}
