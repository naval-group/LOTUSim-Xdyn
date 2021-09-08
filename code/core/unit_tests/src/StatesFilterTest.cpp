/*
 * StatesFilterTest.cpp
 *
 *  Created on: Sep 08, 2021
 *      Author: cady
 */


#include "BlockedDOF.hpp"
#include "History.hpp"
#include "InvalidInputException.hpp"
#include "StatesFilter.hpp"
#include "StatesFilterTest.hpp"

StatesFilterTest::StatesFilterTest() :  a(ssc::random_data_generator::DataGenerator(556677))
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

TEST_F(StatesFilterTest, if_duration_is_zero_no_filtering_should_take_place)
{
    History h(2);
    const double last_value = a.random<double>();
    h.record(0, 1);
    h.record(1, 2);
    h.record(2, last_value);
    const std::string yaml = "type of filter: moving average\n"
                             "duration in seconds : 0";
    const auto filter = StatesFilter::build(yaml);
    ASSERT_DOUBLE_EQ(last_value, filter->get_value(h));
}