/*
 * CSVControllerTest.cpp
 *
 *  Created on: Mar 16, 2022
 *      Author: cady
 */

#include "CSVControllerTest.hpp"
#include "CSVController.hpp"
#include "listeners.hpp"
#include "parse_controllers.hpp"
#include "parse_time_series.hpp"
#include "simulator_api.hpp"
#include "yaml_data.hpp"
#include <boost/algorithm/string.hpp>
#include <ssc/solver/DiscreteSystem.hpp>
#include <ssc/solver/Scheduler.hpp>
#define EPS (1E-14)
#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

CSVControllerTest::CSVControllerTest()
    : a(ssc::random_data_generator::DataGenerator(54545))
{
}

CSVControllerTest::~CSVControllerTest() {}

void CSVControllerTest::SetUp() {}

void CSVControllerTest::TearDown() {}

std::string CSVControllerTest::test_yaml() const
{
    std::stringstream yaml_string;
    yaml_string << "name: some unique name\n"
                << "type: csv\n"
                << "path: path/../to/file.csv\n"
                << "time column name: t\n"
                << "separator: comma\n"
                << "shift time column to match tstart: true\n"
                << "commands:\n"
                << "    port side propeller(beta): beta_co\n"
                << "    port side propeller(rpm): rpm_co";
    return yaml_string.str();
}

TEST_F(CSVControllerTest, smoke_test)
{
    ASSERT_NO_THROW(CSVController(0, test_yaml()));
}

TEST_F(CSVControllerTest, can_parse_yaml_path)
{
    const CSVController controller(0, test_yaml());
    ASSERT_EQ("path/../to/file.csv", controller.yaml.path);
}

TEST_F(CSVControllerTest, can_parse_time_column)
{
    const CSVController controller(0, test_yaml());
    ASSERT_EQ("t", controller.yaml.time_column);
}

TEST_F(CSVControllerTest, can_parse_separator_and_throw_if_it_is_unknown)
{
    std::string yaml = test_yaml();
    ASSERT_NO_THROW(CSVController(0, yaml));
    ASSERT_EQ(',', CSVController(0, yaml).yaml.separator);
    boost::replace_all(yaml, "separator: comma", "separator: something");
    ASSERT_THROW(CSVController(0, yaml), InvalidInputException);
    boost::replace_all(yaml, "separator: something", "separator: semicolon");
    ASSERT_NO_THROW(CSVController(0, yaml));
    ASSERT_EQ(';', CSVController(0, yaml).yaml.separator);
}

TEST_F(CSVControllerTest, can_parse_time_shift_to_match_tstart)
{
    std::string yaml = test_yaml();
    ASSERT_TRUE(CSVController(0, yaml).yaml.shift_time_column);
    boost::replace_all(yaml, "shift time column to match tstart: true", "shift time column to match tstart: false");
    ASSERT_FALSE(CSVController(0, yaml).yaml.shift_time_column);
    boost::replace_all(yaml, "shift time column to match tstart: false", "shift time column to match tstart: foo");
    ASSERT_THROW(CSVController(0, yaml), InvalidInputException);
}

TEST_F(CSVControllerTest, can_parse_commands)
{
    auto commands = CSVController(0, test_yaml()).yaml.commands;
    ASSERT_EQ(2, commands.size());
    ASSERT_EQ(commands["port side propeller(beta)"], "beta_co");
    ASSERT_EQ(commands["port side propeller(rpm)"], "rpm_co");
}

TEST_F(CSVControllerTest, commands_should_be_correct)
{
    const CSVController controller(0, test_yaml());
    const std::vector<std::string> expected_commands = {"port side propeller(beta)", "port side propeller(rpm)"};
    ASSERT_EQ(expected_commands, controller.get_command_names());
}