/*
 * calculate_hashTest.cpp
 *
 *  Created on: May 19, 2015
 *      Author: cady
 */
#include <boost/functional/hash.hpp>

#include "calculate_hashTest.hpp"
#include "calculate_sha1.hpp"

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

TEST_F(calculate_hashTest, can_hash_a_string)
{
    const std::string s = "Hello world";
    unsigned int hash1[5] = {0};
    unsigned int hash2[5] = {0};
    unsigned int hash3[5] = {0};
    Sha1 sha1;
    sha1.append(s);
    sha1.get(hash1);
    sha1.get(hash2);
    Sha1 sha2;
    sha2.append(s);
    sha2.get(hash3);
    for (size_t i=0;i<5;++i)
    {
        ASSERT_EQ(hash1[i], hash2[i]);
        ASSERT_EQ(hash1[i], hash3[i]);
    }
}

TEST_F(calculate_hashTest, can_hash_array_of_doubles)
{
    const double v[2] = {1.0,2.0};
    unsigned int hash1[5] = {0};
    unsigned int hash2[5] = {0};
    Sha1 sha1;
    sha1.append(v[0]);
    sha1.append(v[1]);
    sha1.get(hash1);
    Sha1 sha2;
    sha2.append(std::vector<double>(v, v + sizeof(v) / sizeof(double)));
    sha2.get(hash2);
    for (size_t i=0;i<5;++i) ASSERT_EQ(hash1[i], hash2[i]);
    ASSERT_EQ(4, sizeof(int));
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
    unsigned int hash[5] = {0};
    Sha1 sha1;
    sha1_append(sha1,c);
    sha1.get(hash);
    ASSERT_EQ(2881807160, hash[0]);
    ASSERT_EQ(1714918628, hash[1]);
    ASSERT_EQ(3340119314, hash[2]);
    ASSERT_EQ(2538060347, hash[3]);
    ASSERT_EQ(974152203, hash[4]);
//! [calculate_hashTest expected output]
}

