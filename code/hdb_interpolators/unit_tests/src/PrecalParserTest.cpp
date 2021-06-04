/*
 * PrecalParserTest.cpp
 *
 *  Created on: May 03, 2021
 *      Author: cady
 */

#include "PrecalParserTest.hpp"
#include "PrecalParser.hpp"
#include "precal_test_data.hpp"

PrecalParserTest::PrecalParserTest() {}

PrecalParserTest::~PrecalParserTest() {}

void PrecalParserTest::SetUp() {}

void PrecalParserTest::TearDown() {}

TEST_F(PrecalParserTest, can_get_the_right_number_of_signals)
{
    const auto signals = parse_signals(test_data::precal());
    ASSERT_EQ(118, signals.size());
}
