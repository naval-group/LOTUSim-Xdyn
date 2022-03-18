/*
 * CSVControllerTest.cpp
 *
 *  Created on: Mar 16, 2022
 *      Author: cady
 */

#include "simulator_api.hpp"
#include <ssc/solver/Scheduler.hpp>
#include <ssc/solver/DiscreteSystem.hpp>
#include "listeners.hpp"
#include "CSVControllerTest.hpp"
#include "CSVController.hpp"
#include "parse_controllers.hpp"
#include "parse_time_series.hpp"
#include "yaml_data.hpp"

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
