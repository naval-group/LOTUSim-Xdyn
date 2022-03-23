/*
 * CSVControllerTest.cpp
 *
 *  Created on: Mar 16, 2022
 *      Author: cady
 */

#include <cstdio> // For remove
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
    : csv()
    , a(ssc::random_data_generator::DataGenerator(54545))
{
    csv << "t,some column,rpm_co,beta_co\n";
    csv << "0.2,23,65,78\n";
    csv.close();
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
    std::string out = yaml_string.str();
    boost::replace_all(out, "path/../to/file.csv", csv.get_filename());
    return out;
}

TEST_F(CSVControllerTest, smoke_test)
{
    ASSERT_NO_THROW(CSVController(0, test_yaml()));
}

TEST_F(CSVControllerTest, can_parse_yaml_path)
{
    const CSVController controller(0, test_yaml());
    ASSERT_EQ(csv.get_filename(), controller.yaml.path);
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


TempFile::TempFile() : filename(tmpnam(NULL)), csv(std::ofstream(filename, std::ofstream::out))
{
}

TempFile::~TempFile()
{
    remove(filename.c_str());
}

std::string TempFile::get_filename() const
{
    return filename;
}

void TempFile::close()
{
    csv.close();
}

TEST_F(CSVControllerTest, can_read_data_from_csv)
{
    std::string yaml = test_yaml();
    CSVController controller(0, yaml);
    const double tstart = 0.1;
    const double dt = 0.5;
    ssc::solver::Scheduler scheduler(tstart, 2, dt);
    Sim sys = get_system(test_data::falling_ball_example(), 0);
    controller.callback(scheduler, &sys);
    ASSERT_NEAR(65, sys.get_input_value("port side propeller(rpm)"), 1e-6);
    ASSERT_NEAR(78, sys.get_input_value("port side propeller(beta)"), 1e-6);
}