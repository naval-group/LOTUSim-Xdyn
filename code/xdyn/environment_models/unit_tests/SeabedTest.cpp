/*
 * SeabedTest.cpp
 *
 *  Created on: 3 June 2025
 *      Author: julien.prodhon
 */

#include "SeabedTest.hpp"
#include "gtest/gtest.h"
#include <Eigen/Dense>
#include <fstream>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb.hpp"

SeabedTest::SeabedTest () : a(ssc::random_data_generator::DataGenerator(149555))
{}

SeabedTest::~SeabedTest (){}

void SeabedTest::SetUp(){}

void SeabedTest::TearDown(){}

TEST_F(SeabedTest, seabed_write)
{
    const unsigned width = 4;
    const unsigned height = 4;
    const unsigned channels = 4; // RGBA = 4 channels

    std::vector<uint8_t> rgba_image = {
        // Row 1
        75, 75, 75, 255,  72, 72, 72, 255,  73, 73, 73, 255,  69, 69, 69, 255,
        // Row 2
        74, 74, 74, 255,  68, 68, 68, 255,  64, 64, 64, 255,  62, 62, 62, 255,
        // Row 3
        77, 77, 77, 255,  69, 69, 69, 255,  63, 63, 63, 255,  62, 62, 62, 255,
        // Row 4
        84, 84, 84, 255,  73, 73, 73, 255,  65, 65, 65, 255,  64, 64, 64, 255,
    };
    stbi_write_png("seabedtest.png", width, height, channels, rgba_image.data(), width * channels);
    std::ifstream file("seabedtest.png");
    ASSERT_TRUE(file.good()) << "Failed to create seabedtest.png file";
}

TEST_F(SeabedTest, parse_seabed_from_double)
{
    double depth = 65.65;
    Seabed seabed(depth);
    double depth_out = seabed.get_height(1,2);
    ASSERT_DOUBLE_EQ(depth_out, depth);
    Eigen::Vector2d grad_out = seabed.get_gradient(1,2);
    ASSERT_DOUBLE_EQ(grad_out(0), 0);
    ASSERT_DOUBLE_EQ(grad_out(1), 0);
}

TEST_F(SeabedTest, parse_seabed_from_file)
{
    Seabed seabed("seabedtest.png");
    seabed.get_height(135,-3);
    seabed.get_gradient(-120,4.4);
}
