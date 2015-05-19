/*
 * calculate_hashTest.cpp
 *
 *  Created on: May 19, 2015
 *      Author: cady
 */
#include <boost/functional/hash.hpp>

#include "calculate_hashTest.hpp"
#include "calculate_hash.hpp"

calculate_hashTest::calculate_hashTest() : a(ssc::random_data_generator::DataGenerator(54212))
{
}

calculate_hashTest::~calculate_hashTest()
{
}

void calculate_hashTest::SetUp()
{
}

void calculate_hashTest::TearDown()
{
}

TEST_F(calculate_hashTest, can_compute_a_hash_using_boost)
{
    boost::hash<std::string> hash;
    const size_t hash1 = hash("something");
    const size_t hash2 = hash("something else");
    ASSERT_NE(hash1, hash2);
}

TEST_F(calculate_hashTest, can_hash_YamlCoordinates)
{
//! [calculate_hashTest example]
    YamlCoordinates c;
    c.x = 1;
    c.y = -2;
    c.z = 3;
//! [calculate_hashTest example]
//! [calculate_hashTest expected output]
    ASSERT_EQ(8360662327059029742, boost::hash<YamlCoordinates>()(c));
//! [calculate_hashTest expected output]
}



