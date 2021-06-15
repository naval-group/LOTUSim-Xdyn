/*
 * listenersTest.cpp
 *
 *  Created on: Oct 21, 2014
 *      Author: cady
 */

#include "listenersTest.hpp"
#include "listeners.hpp"
#include "parse_controllers.hpp"
#include "parse_time_series.hpp"
#include "yaml_data.hpp"
#include "InvalidInputException.hpp"
#include "PIDController.hpp"

#define EPS (1E-14)
#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

listenersTest::listenersTest() : a(ssc::random_data_generator::DataGenerator(54545))
{
}

listenersTest::~listenersTest()
{
}

void listenersTest::SetUp()
{
}

void listenersTest::TearDown()
{
}

TEST_F(listenersTest, example)
{
//! [listenersTest listen_to_file_example]
    auto ds = make_command_listener(parse_command_yaml(test_data::controlled_forces()));
    ds.check_in("listenersTest (example)");
    ds.set<double>("t", 0);
    ASSERT_DOUBLE_EQ(3, ds.get<double>("propeller(rpm)"));
    ASSERT_DOUBLE_EQ(1.064935, ds.get<double>("propeller(P/D)"));

    ds.set<double>("t", 0.5);
    ASSERT_DOUBLE_EQ(16.5, ds.get<double>("propeller(rpm)"));
    ASSERT_DOUBLE_EQ(1.064935, ds.get<double>("propeller(P/D)"));

    ds.set<double>("t", 1);
    ASSERT_DOUBLE_EQ(30, ds.get<double>("propeller(rpm)"));
    ASSERT_DOUBLE_EQ(1.064935, ds.get<double>("propeller(P/D)"));

    for (size_t i = 0 ; i < 1000 ; ++i)
    {
        ds.set<double>("t", a.random<double>().greater_than(10));
        ASSERT_DOUBLE_EQ(40, ds.get<double>("propeller(rpm)"));
        ASSERT_DOUBLE_EQ(1.064935, ds.get<double>("propeller(P/D)"));
    }
    ds.check_out();
    //! [listenersTest listen_to_file_example]
}

TEST_F(listenersTest, can_parse_simple_track_keeping_commands)
{
    const std::string commands =
          "  - name: controller\n"
          "    t: [0,1,3,10]\n"
          "    psi_co: {unit: rad, values: [0.25, 0.30, 0.40, 0]}\n";
    auto ds = make_command_listener(parse_command_yaml(test_data::controlled_forces()+commands));
    ds.check_in("listenersTest (can_parse_simple_track_keeping_commands)");
    ds.set<double>("t", 0);
    ASSERT_NEAR(0.25, ds.get<double>("controller(psi_co)"),EPS);

    ds.set<double>("t", 0.5);
    ASSERT_NEAR(0.275, ds.get<double>("controller(psi_co)"),EPS);

    ds.set<double>("t", 1);
    ASSERT_NEAR(0.3, ds.get<double>("controller(psi_co)"),EPS);

    ds.set<double>("t", 2);
    ASSERT_NEAR(0.35, ds.get<double>("controller(psi_co)"),EPS);

    ds.set<double>("t", 3);
    ASSERT_NEAR(0.4, ds.get<double>("controller(psi_co)"),EPS);

    ds.set<double>("t", 5);
    ASSERT_NEAR(2./7., ds.get<double>("controller(psi_co)"),EPS);

    ds.set<double>("t", 10);
    ASSERT_NEAR(0, ds.get<double>("controller(psi_co)"), EPS);

    ds.set<double>("t", 100);
    ASSERT_NEAR(0, ds.get<double>("controller(psi_co)"), EPS);
    ds.check_out();
}

TEST_F(listenersTest, bug_2961_can_have_a_single_value_for_commands)
{
    auto ds = make_command_listener(parse_command_yaml(test_data::bug_2961()));
    ds.check_in("listenersTest (bug_2961_can_have_a_single_value_for_commands)");
    for (size_t i = 0 ; i < 100 ; ++i)
    {
        ds.set<double>("t", a.random<double>());
        ASSERT_NEAR(30*PI/180., ds.get<double>("controller(psi_co)"),EPS);
    }
    ds.check_out();
}


TEST_F(listenersTest, can_add_setpoints_to_an_empty_ds)
{
    ssc::data_source::DataSource ds;
    add_setpoints_listener(ds, parse_setpoint_yaml(test_data::setpoints()));
    ds.check_in("listenersTest (can_add_setpoints_to_an_empty_ds)");
    ds.set<double>("t", 0);
    ASSERT_DOUBLE_EQ(3, ds.get<double>("rpm_co"));
    ASSERT_DOUBLE_EQ(1.064935, ds.get<double>("P/D"));

    ds.set<double>("t", 1);
    ASSERT_DOUBLE_EQ(30, ds.get<double>("rpm_co"));
    ASSERT_DOUBLE_EQ(1.064935, ds.get<double>("P/D"));

    ds.set<double>("t", 3);
    ASSERT_DOUBLE_EQ(30, ds.get<double>("rpm_co"));
    ASSERT_DOUBLE_EQ(1.064935, ds.get<double>("P/D"));

    for (size_t i = 0 ; i < 1000 ; ++i)
    {
        ds.set<double>("t", a.random<double>().greater_than(10));
        ASSERT_DOUBLE_EQ(40, ds.get<double>("rpm_co"));
        ASSERT_DOUBLE_EQ(1.064935, ds.get<double>("P/D"));
    }
    ds.check_out();
}

TEST_F(listenersTest, can_have_a_single_value_for_setpoints)
{
    ssc::data_source::DataSource ds;
    add_setpoints_listener(ds, parse_setpoint_yaml(test_data::setpoints()));
    ds.check_in("listenersTest (can_have_a_single_value_for_setpoints)");
    for (size_t i = 0 ; i < 100 ; ++i)
    {
        ds.set<double>("t", a.random<double>());
        ASSERT_NEAR(2.5, ds.get<double>("psi_co"),EPS);
    }
    ds.check_out();
}

TEST_F(listenersTest, can_add_setpoints_when_commands_with_same_name_already_exist)
{
    std::string commands = "commands:\n"
                           "  - name: propeller\n"
                           "    t: [0]\n"
                           "    rpm_co: {unit: rad/s, values: [50]}\n" // also defined in test_data::setpoints()
                           "    command: {unit: 1, values: [42]}\n";
    ssc::data_source::DataSource ds = make_command_listener(parse_command_yaml(commands));

    add_setpoints_listener(ds, parse_setpoint_yaml(test_data::setpoints()));
    ds.check_in("listenersTest (can_add_setpoints_when_commands_with_same_name_already_exist)");
    ds.set<double>("t", 0);
    ASSERT_DOUBLE_EQ(3, ds.get<double>("rpm_co")); // setpoints value
    ASSERT_DOUBLE_EQ(1.064935, ds.get<double>("P/D")); // setpoints value
    ASSERT_DOUBLE_EQ(2.5, ds.get<double>("psi_co")); // setpoints value
    ASSERT_DOUBLE_EQ(50, ds.get<double>("propeller(rpm_co)")); // commands value
    ASSERT_DOUBLE_EQ(42, ds.get<double>("propeller(command)")); // commands value

    ds.set<double>("t", 1);
    ASSERT_DOUBLE_EQ(30, ds.get<double>("rpm_co")); // setpoints value
    ASSERT_DOUBLE_EQ(1.064935, ds.get<double>("P/D")); // setpoints value
    ASSERT_DOUBLE_EQ(2.5, ds.get<double>("psi_co")); // setpoints value
    ASSERT_DOUBLE_EQ(50, ds.get<double>("propeller(rpm_co)")); // commands value
    ASSERT_DOUBLE_EQ(42, ds.get<double>("propeller(command)")); // commands value

    ds.set<double>("t", 3);
    ASSERT_DOUBLE_EQ(30, ds.get<double>("rpm_co")); // setpoints value
    ASSERT_DOUBLE_EQ(1.064935, ds.get<double>("P/D")); // setpoints value
    ASSERT_DOUBLE_EQ(2.5, ds.get<double>("psi_co")); // setpoints value
    ASSERT_DOUBLE_EQ(50, ds.get<double>("propeller(rpm_co)")); // commands value
    ASSERT_DOUBLE_EQ(42, ds.get<double>("propeller(command)")); // commands value

    ds.check_out();
}

TEST_F(listenersTest, should_throw_if_force_model_name_is_not_defined_for_a_command)
{
    std::string commands = "commands:\n"
                           "  - t: [0]\n"
                           "    rpm_co: {unit: rad/s, values: [50]}\n"
                           "    command: {unit: 1, values: [42]}\n";
    ASSERT_THROW(make_command_listener(parse_command_yaml(commands)), InvalidInputException);
}

TEST_F(listenersTest, get_pid_controllers_example)
{
    const std::string commands =
        "commands:\n"
        "  - name: propeller\n"
        "    t: [0]\n"
        "    rpm_co: {unit: rad/s, values: [3]}\n"
        "  - name: controller\n"
        "    t: [0]\n"
        "    rpm: {unit: rad/s, values: [3]}\n"
        "    psi_co: {unit: deg, values: [30]}\n";
    //! [controllersTest listen_to_file_example]
    const auto controllers = build_controllers(0,
                                             parse_controller_yaml(test_data::controllers()),
                                             parse_command_yaml(commands), NULL
                                             );
    ASSERT_EQ(2, controllers.size());
    ASSERT_EQ("propeller(rpm)", ((PIDController*)controllers.at(0).get())->yaml.command_name);
    ASSERT_EQ("controller(psi)", ((PIDController*)controllers.at(1).get())->yaml.command_name);
    //! [controllersTest listen_to_file_example]
}

TEST_F(listenersTest, should_throw_if_controllers_and_commands_define_the_same_command)
{
    const std::vector<YamlTimeSeries> yaml_commands = parse_command_yaml(
        "commands:\n"
        "  - name: controller\n"
        "    t: [0]\n"
        "    psi_co: {unit: deg, values: [30]}\n"
        "  - name: propeller\n"
        "    t: [0]\n"
        "    rpm: {unit: rad/s, values: [3]}\n");
    const std::vector<YamlController> yaml_controllers = parse_controller_yaml(test_data::controllers());

    ASSERT_THROW(build_controllers(0, yaml_controllers, yaml_commands, NULL), InvalidInputException);
}
