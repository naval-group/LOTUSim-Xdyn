/*
 * EkmanUWCurrentTest.cpp
 *
 *  Created on: 5 august 2025
 *      Author: julien.prodhon
 */

#include "EkmanUWCurrentTest.hpp"
#include "EkmanUWCurrentModel.hpp"
#include <Eigen/Dense>

EkmanUWCurrentTest::EkmanUWCurrentTest () : a(ssc::random_data_generator::DataGenerator(149555))
{}

EkmanUWCurrentTest::~EkmanUWCurrentTest (){}

void EkmanUWCurrentTest::SetUp(){}

void EkmanUWCurrentTest::TearDown(){}


TEST_F(EkmanUWCurrentTest, test_velocity)
{
    Seabed seabed(65.65);
    EkmanUWCurrentModel::Input input(seabed);
    EkmanUWCurrentModel model(input);
    Eigen::Vector3d point1(1.8,13.2,1.5);
    model.get_UWCurrent(point1,0,5.5);
    Eigen::Vector3d point2(-5,-11.1,8);
    model.get_UWCurrent(point2,1,-2);
}

TEST_F(EkmanUWCurrentTest, test_bug_map_at)
{
    std::string yaml_input = "{seabed file: seabedtest.png,"
                             " latitude: { value: 47, unit: rad },"
                             " top layer thickness: { value: 10, unit: m },"
                             " bottom layer thickness: { value: 20, unit: m },"
                             " current velocity: { value: 1, unit: m/s },"
                             " current orientation: { value: 30, unit: deg },"
                             " U10: { value: 1, unit: m/s },"
                             " wind orientation: { value: 20, unit: deg }}";
    EkmanUWCurrentModel model = EkmanUWCurrentModel::parse(yaml_input);
    Eigen::Vector3d point1(3.5,-7,0.2);
    model.get_UWCurrent(point1,0,5.5);
    Eigen::Vector3d point2(-7,150,6);
    model.get_UWCurrent(point2,0,5.5);
}
