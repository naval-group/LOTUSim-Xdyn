/*
 * check_input_yamlTest.cpp
 *
 *  Created on: 15 avr. 2014
 *      Author: cady
 */

#include "gmock/gmock.h"
using namespace testing; // So we can use 'ElementsAre' unqualified

#include "external_data_structures_parsers.hpp"
#include "check_input_yamlTest.hpp"
#include "yaml_data.hpp"
#include "SimulatorYamlParser.hpp"
#include "InvalidInputException.hpp"


check_input_yamlTest::check_input_yamlTest() : a(ssc::random_data_generator::DataGenerator(1212))
{
}

check_input_yamlTest::~check_input_yamlTest()
{
}

void check_input_yamlTest::SetUp()
{
}

void check_input_yamlTest::TearDown()
{
}

TEST_F(check_input_yamlTest, can_check_valid_controllers_and_commands_for_controlled_forces)
{
    const std::string yaml = test_data::falling_ball_example() +
                             "commands:\n"
                             "  - name: propeller\n"
                             "    t: [0,1,3,10]\n"
                             "    rpm: {unit: rad/s, values: [3, 30, 30, 40]}\n"
                             "    psi: {unit: rad, values: [0.25, 0.30, 0.40, 0]}\n"
                             "  - name: controller\n"
                             "    t: [0]\n"
                             "    psi_co: {unit: deg, values: [30]}\n"
                             "inputs:\n"
                             "  - name: propeller\n"
                             "    t: [0,1,3,10]\n"
                             "    rpm_co: {unit: rad/s, values: [3, 30, 30, 40]}\n"
                             "    P/D: {unit: 1, values: [1.064935,1.064935,1.064935,1.064935]}\n"
                             "  - name: controller\n"
                             "    t: [0,1,3,10]\n"
                             "    psi: {unit: rad, values: [0.25, 0.30, 0.40, 0]}\n"
                             "controllers:\n"
                             "  - name: propeller\n"
                             "    type: PID\n"
                             "    output: rpm_co\n"
                             "    dt: 0.1\n"
                             "    input: rpm_co\n"
                             "    states:\n"
                             "        x: 1\n"
                             "        y: -1\n"
                             "    gains:\n"
                             "        Kp: 4.2\n"
                             "        Ki: 0.25\n"
                             "        Kd: 1\n"
                             "  - name: propeller\n"
                             "    type: PID\n"
                             "    output: P/D\n"
                             "    dt: 10\n"
                             "    input: P/D\n"
                             "    states:\n"
                             "        u: 2\n"
                             "  - name: controller\n"
                             "    type: PID\n"
                             "    output: psi\n"
                             "    dt: 0.5\n"
                             "    input: psi\n"
                             "    states:\n"
                             "        psi: 1\n"
                             "    gains:\n"
                             "        Kp: 0.52\n"
                             "        Ki: 21.5\n"
                             "        Kd: 2.2\n";
    const YamlSimulatorInput input = SimulatorYamlParser(yaml).parse();
    ASSERT_NO_THROW(check_input_yaml(input));
}

TEST_F(check_input_yamlTest, should_throw_if_controllers_and_commands_define_the_same_command)
{
    const std::string yaml = test_data::falling_ball_example() +
                             "commands:\n"
                             "  - name: propeller\n"
                             "    t: [0]\n"
                             "    rpm: {unit: rad/s, values: [3]}\n"
                             "  - name: controller\n"
                             "    t: [0]\n"
                             "    psi_co: {unit: deg, values: [30]}\n"
                             "controllers:\n"
                             "  - name: propeller\n"
                             "    type: grpc\n"
                             "    output: rpm_co\n"
                             "    dt: 1\n"
                             "    input: rpm\n"
                             "    states:\n"
                             "        u: 1\n"
                             "        v: 1\n"
                             "  - name: controller\n"
                             "    type: PID\n"
                             "    output: psi_co\n"
                             "    dt: 0.5\n"
                             "    input: psi\n"
                             "    states:\n"
                             "        psi: 1\n"
                             "    gains:\n"
                             "        Kp: 0.52\n"
                             "        Ki: 21.5\n"
                             "        Kd: 2.2\n";
    const YamlSimulatorInput input = SimulatorYamlParser(yaml).parse();
    ASSERT_THROW(check_input_yaml(input), InvalidInputException);
}
