/*
 * parse_time_seriesTest.cpp
 *
 *  Created on: Oct 22, 2014
 *      Author: cady
 */

#include "parse_time_seriesTest.hpp"
#include "parse_time_series.hpp"
#include "yaml_data.hpp"

parse_time_seriesTest::parse_time_seriesTest() : a(ssc::random_data_generator::DataGenerator(542121))
{
}

parse_time_seriesTest::~parse_time_seriesTest()
{
}

void parse_time_seriesTest::SetUp()
{
}

void parse_time_seriesTest::TearDown()
{
}

TEST_F(parse_time_seriesTest, example)
{
//! [parse_command_yaml example]
    const std::string commands_yaml =
          "  - name: controller\n"
          "    t: [0,1,3,10]\n"
          "    psi_co: {unit: rad, values: [0.25, 0.30, 0.40, 0]}\n";
    std::vector<YamlTimeSeries> commands = parse_command_yaml(test_data::controlled_forces()+commands_yaml);
    ASSERT_EQ(2, commands.size());
    ASSERT_EQ("propeller", commands[0].name);
    ASSERT_EQ(4, commands[0].t.size());
    ASSERT_DOUBLE_EQ(0, commands[0].t[0]);
    ASSERT_DOUBLE_EQ(1, commands[0].t[1]);
    ASSERT_DOUBLE_EQ(3, commands[0].t[2]);
    ASSERT_DOUBLE_EQ(10, commands[0].t[3]);
    ASSERT_EQ(2, commands[0].values.size());
    ASSERT_EQ(4, commands[0].values["rpm"].size());
    ASSERT_DOUBLE_EQ(3, commands[0].values["rpm"][0]);
    ASSERT_DOUBLE_EQ(30, commands[0].values["rpm"][1]);
    ASSERT_DOUBLE_EQ(30, commands[0].values["rpm"][2]);
    ASSERT_DOUBLE_EQ(40, commands[0].values["rpm"][3]);
    ASSERT_EQ(4, commands[0].values["P/D"].size());
    ASSERT_DOUBLE_EQ(1.064935, commands[0].values["P/D"][0]);
    ASSERT_DOUBLE_EQ(1.064935, commands[0].values["P/D"][1]);
    ASSERT_DOUBLE_EQ(1.064935, commands[0].values["P/D"][2]);
    ASSERT_DOUBLE_EQ(1.064935, commands[0].values["P/D"][3]);

    ASSERT_EQ("controller", commands[1].name);
    ASSERT_EQ(4, commands[1].t.size());
    ASSERT_DOUBLE_EQ(0, commands[1].t[0]);
    ASSERT_DOUBLE_EQ(1, commands[1].t[1]);
    ASSERT_DOUBLE_EQ(3, commands[1].t[2]);
    ASSERT_DOUBLE_EQ(10, commands[1].t[3]);
    ASSERT_EQ(1, commands[1].values.size());
    ASSERT_EQ(4, commands[1].values["psi_co"].size());
    ASSERT_DOUBLE_EQ(0.25, commands[1].values["psi_co"][0]);
    ASSERT_DOUBLE_EQ(0.30, commands[1].values["psi_co"][1]);
    ASSERT_DOUBLE_EQ(0.40, commands[1].values["psi_co"][2]);
    ASSERT_DOUBLE_EQ(0, commands[1].values["psi_co"][3]);
//! [parse_command_yaml example]
}


TEST_F(parse_time_seriesTest, setpoints_example)
{
//! [parse_setpoint_yaml example]
    std::vector<YamlTimeSeries> setpoints = parse_setpoint_yaml(test_data::setpoints());
    ASSERT_EQ(2, setpoints.size());

    ASSERT_EQ("", setpoints[0].name);
    ASSERT_EQ(4, setpoints[0].t.size());
    ASSERT_DOUBLE_EQ(0, setpoints[0].t[0]);
    ASSERT_DOUBLE_EQ(1, setpoints[0].t[1]);
    ASSERT_DOUBLE_EQ(3, setpoints[0].t[2]);
    ASSERT_DOUBLE_EQ(10, setpoints[0].t[3]);
    ASSERT_EQ(2, setpoints[0].values.size());
    ASSERT_EQ(4, setpoints[0].values["rpm_co"].size());
    ASSERT_DOUBLE_EQ(3, setpoints[0].values["rpm_co"][0]);
    ASSERT_DOUBLE_EQ(30, setpoints[0].values["rpm_co"][1]);
    ASSERT_DOUBLE_EQ(30, setpoints[0].values["rpm_co"][2]);
    ASSERT_DOUBLE_EQ(40, setpoints[0].values["rpm_co"][3]);
    ASSERT_EQ(4, setpoints[0].values["P/D"].size());
    ASSERT_DOUBLE_EQ(1.064935, setpoints[0].values["P/D"][0]);
    ASSERT_DOUBLE_EQ(1.064935, setpoints[0].values["P/D"][1]);
    ASSERT_DOUBLE_EQ(1.064935, setpoints[0].values["P/D"][2]);
    ASSERT_DOUBLE_EQ(1.064935, setpoints[0].values["P/D"][3]);

    ASSERT_EQ("", setpoints[1].name);
    ASSERT_EQ(1, setpoints[1].t.size());
    ASSERT_DOUBLE_EQ(4.2, setpoints[1].t[0]);
    ASSERT_EQ(1, setpoints[1].values.size());
    ASSERT_EQ(1, setpoints[1].values["psi_co"].size());
    ASSERT_DOUBLE_EQ(2.5, setpoints[1].values["psi_co"][0]);
//! [parse_setpoint_yaml example]
}
