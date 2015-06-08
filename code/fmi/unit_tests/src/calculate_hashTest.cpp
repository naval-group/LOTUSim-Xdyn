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

TEST_F(calculate_hashTest, can_calculate_the_same_sha_several_times)
{
    const std::string s = a.random<std::string>();
    Sha1 sha;
    sha.append(s);
    ASSERT_EQ(sha.get(), sha.get());
    ASSERT_EQ(sha.get(), sha.get());
}

TEST_F(calculate_hashTest, two_different_sha_objects_give_the_same_sha)
{
    const std::string s = "Hello world";
    Sha1 sha1;
    sha1.append(s);
    Sha1 sha2;
    sha2.append(s);
    ASSERT_EQ(sha1.get(), sha2.get());
}

TEST_F(calculate_hashTest, integer_type_has_the_right_size_otherwise_sha_will_be_inconsistent_accross_platforms)
{
    ASSERT_EQ(4, sizeof(int));
}

TEST_F(calculate_hashTest, adding_a_vector_gives_the_same_result_as_adding_doubles_separately)
{
    const std::vector<double> data = {1,2};
    Sha1 sha1;
    sha1.append(data[0]);
    sha1.append(data[1]);
    Sha1 sha2;
    sha2.append(data);
    ASSERT_EQ(sha1.get(), sha2.get());
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
    Sha1 sha1;
    sha1_append(sha1,c);
    ASSERT_EQ("abc4e338663794e4c7162d129747ba3b3a10620b", sha1.get());
//! [calculate_hashTest expected output]
}

