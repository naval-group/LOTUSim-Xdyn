/*
 * PIDControllerTest.cpp
 *
 *  Created on: Fev 12, 2021
 *      Author: lincker
 */

#include "simulator_api.hpp"
#include <ssc/solver/Scheduler.hpp>
#include <ssc/solver/DiscreteSystem.hpp>
#include "listeners.hpp"
#include "PIDControllerTest.hpp"
#include "PIDController.hpp"
#include "parse_controllers.hpp"
#include "parse_time_series.hpp"
#include "yaml_data.hpp"

#define EPS (1E-14)
#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

PIDControllerTest::PIDControllerTest() : a(ssc::random_data_generator::DataGenerator(54545))
{
}

PIDControllerTest::~PIDControllerTest()
{
}

void PIDControllerTest::SetUp()
{
}

void PIDControllerTest::TearDown()
{
}

std::string PIDControllerTest::pid_specific_yaml(const double Kp, const double Ki, const double Kd,
                                                 const std::string& setpoint_name,
                                                 const std::string& command_name
                                                 )
{
    std::stringstream yaml_string;
    yaml_string << "gains:\n"
                << "    Kp: " << std::to_string(Kp) << "\n"
                << "    Ki: " << std::to_string(Ki) << "\n"
                << "    Kd: " << std::to_string(Kd) << "\n"
                << "setpoint: " << setpoint_name << "\n"
                << "command: " << command_name << "\n";
    return yaml_string.str();
}

TEST_F(PIDControllerTest, can_parse_controller_specific_yaml)
{
    for (size_t i = 0 ; i < 100 ; ++i)
    {
        const double dt = 0.2;
        const double Kp = a.random<double>();
        const double Ki = a.random<double>();
        const double Kd = a.random<double>();
        const std::string setpoint_name = a.random<std::string>();
        const std::string command_name = a.random<std::string>();

        const double x_weight = a.random<double>();
        const double psi_weight = a.random<double>();
        const PIDController controller = PIDController(0, dt,
                                                       pid_specific_yaml(Kp, Ki, Kd, setpoint_name, command_name) +
                                                       "state weights:\n    x: " + std::to_string(x_weight) +
                                                       "\n    psi: " + std::to_string(psi_weight) + "\n");

        ASSERT_NEAR(Kp, controller.yaml.Kp, 1e-6);
        ASSERT_NEAR(Ki, controller.yaml.Ki, 1e-6);
        ASSERT_NEAR(Kd, controller.yaml.Kd, 1e-6);
        std::map<std::string, double> state_weights = controller.yaml.state_weights;
        ASSERT_EQ(2, state_weights.size());
        ASSERT_EQ(1, state_weights.count("x"));
        ASSERT_NEAR(x_weight, state_weights["x"], 1e-6);
        ASSERT_EQ(1, state_weights.count("psi"));
        ASSERT_NEAR(psi_weight, state_weights["psi"], 1e-6);
        ASSERT_EQ(setpoint_name, controller.yaml.setpoint_name);
        ASSERT_EQ(command_name, controller.yaml.command_name);
    }
}

TEST_F(PIDControllerTest, update_command_in_ds_example)
{
    const double tstart = 0.1;
    const double dt = 0.5;
    ssc::solver::Scheduler scheduler(tstart, 10, dt);

    const double Kp = 2.5;
    const double Ki = 0.1;
    const double Kd = 0.314;
    PIDController controller(tstart, dt, pid_specific_yaml(Kp, Ki, Kd, "rpm_co", "propeller(rpm)") + "state weights:\n    x: 1\n    y: -1\n");

    const double rpm_co = 5;
    const double x = 2 * rpm_co;
    const double y = rpm_co + 0.05;

    Sim sys = get_system(test_data::falling_ball_example(), 0);

    //! [callback example]
    // First time step
    sys.set_command_listener(std::map<std::string, double>({ { "t", tstart }, { "rpm_co", rpm_co } }));
    StateType states = {x, y, 300.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 1, 0, 0, 0};
    sys.get_bodies().front()->update_body_states(states, tstart);

    controller.callback(scheduler, sys);
    const double error = rpm_co - (x - y);
    ASSERT_NEAR(Kp * error, sys.get_input_value("propeller(rpm)"), 1e-6);

    //! [callback example]
}

TEST_F(PIDControllerTest, can_compute_PID_commands)
{
    const double tstart = 0.1;
    const double dt = 0.5;
    ssc::solver::Scheduler scheduler(tstart, 2, dt);

    const double Kp = 2.5;
    const double Ki = 0.1;
    const double Kd = 0.314;
    PIDController controller(tstart, dt, pid_specific_yaml(Kp, Ki, Kd, "rpm_co", "propeller(rpm)") + "state weights:\n    x: 1\n    y: -1\n");

    Sim sys = get_system(test_data::falling_ball_example(), 0);

    // First time step
    ASSERT_EQ(tstart, scheduler.get_time());
    const double error = 0.05;
    double rpm_co = 5;
    double x = 2 * rpm_co;
    double y = rpm_co + error;
    sys.set_command_listener(std::map<std::string, double>({ { "t", tstart }, { "rpm_co", rpm_co } }));

    StateType states = {x, y, 300.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 1, 0, 0, 0};
    sys.get_bodies().front()->update_body_states(states, tstart);

    controller.callback(scheduler, sys);
    const double first_expected_command = Kp * error;
    ASSERT_NEAR(first_expected_command, sys.get_input_value("propeller(rpm)"), 1e-6);

    // Second time step
    scheduler.advance_to_next_time_event();
    ASSERT_EQ(tstart + dt, scheduler.get_time());
    const double error2 = -0.03;
    rpm_co = 1.5;
    x = 2 * rpm_co;
    y = rpm_co + error2;
    sys.set_command_listener(std::map<std::string, double>({ { "t", tstart + dt }, { "rpm_co", rpm_co } }));
    states = {x, y, 300.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 1, 0, 0, 0};
    sys.get_bodies().front()->update_body_states(states, tstart + dt);

    controller.callback(scheduler, sys);
    const double second_expected_command = Kp * error2 + Ki * error2 * dt + Kd * (error2 - error) / dt;
    ASSERT_NEAR(second_expected_command, sys.get_input_value("propeller(rpm)"), 1e-6);

    // Third time step
    scheduler.advance_to_next_time_event();
    ASSERT_EQ(tstart + 2 * dt, scheduler.get_time());
    const double error3 = 0.01;
    rpm_co = 5.5;
    x = 2 * rpm_co;
    y = rpm_co + error3;
    sys.set_command_listener(std::map<std::string, double>({ { "t", tstart + 2 * dt }, { "rpm_co", rpm_co } }));
    states = {x, y, 300.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 1, 0, 0, 0};
    sys.get_bodies().front()->update_body_states(states, tstart + 2 * dt);

    controller.callback(scheduler, sys);
    const double third_expected_command = Kp * error3 + Ki * (error2 * dt + error3 * dt) + Kd * (error3 - error2) / dt;
    ASSERT_NEAR(third_expected_command, sys.get_input_value("propeller(rpm)"), 1e-6);
}

TEST_F(PIDControllerTest, can_compute_PID_commands_several_times_at_first_time_step)
{
    const double tstart = 0.1;
    const double dt = 0.5;
    ssc::solver::Scheduler scheduler(tstart, 10, dt);

    const double Kp = 2.5;
    const double Ki = 0.1;
    const double Kd = 0.314;
    PIDController controller(tstart, dt, pid_specific_yaml(Kp, Ki, Kd, "rpm_co", "propeller(rpm)") + "state weights:\n    x: 1\n    y: -1\n");

    Sim sys = get_system(test_data::falling_ball_example(), 0);

    // First time step
    ASSERT_EQ(tstart, scheduler.get_time());
    const double error = 0.05;
    const double rpm_co = 5;
    const double x = 2 * rpm_co;
    const double y = rpm_co + error;
    sys.set_command_listener(std::map<std::string, double>({ { "t", tstart }, { "rpm_co", rpm_co } }));
    const StateType states = {x, y, 300.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 1, 0, 0, 0};
    sys.get_bodies().front()->update_body_states(states, tstart);
    controller.callback(scheduler, sys);
    const double first_expected_command = Kp * error;
    ASSERT_NEAR(first_expected_command, sys.get_input_value("propeller(rpm)"), 1e-6);

    // First time step again should send back the same result
    ASSERT_EQ(tstart, scheduler.get_time());
    controller.callback(scheduler, sys);
    ASSERT_NEAR(first_expected_command, sys.get_input_value("propeller(rpm)"), 1e-6);

    // A time step before the previous time should send back the same result
    scheduler.add_time_event(tstart - dt / 2);
    scheduler.advance_to_next_time_event();
    ASSERT_EQ(tstart - dt / 2, scheduler.get_time());
    controller.callback(scheduler, sys);
    ASSERT_NEAR(first_expected_command, sys.get_input_value("propeller(rpm)"), 1e-6);
}

TEST_F(PIDControllerTest, can_use_euler_angles_in_states)
{
    const double tstart = 0.1;
    const double dt = 0.5;
    ssc::solver::Scheduler scheduler(tstart, 10, dt);

    const double Kp = 2.5;
    const double Ki = 0.1;
    const double Kd = 0.314;
    PIDController controller(tstart, dt, pid_specific_yaml(Kp, Ki, Kd, "psi", "propeller(psi_co)") + "state weights:\n    psi: 1\n");

    Sim sys = get_system(test_data::falling_ball_example(), 0);

    const double psi = 0.45;
    const double error = -0.05;
    sys.set_command_listener(std::map<std::string, double>({ { "t", tstart }, { "psi", psi } }));

    const ssc::kinematics::EulerAngles angles(0.2, 0.9, psi - error);
    std::tuple<double, double, double, double> quaternion = sys.get_bodies().front()->get_quaternions(angles, sys.get_env().rot);
    const StateType states = {100.0, 200.0, 300.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, std::get<0>(quaternion), std::get<1>(quaternion), std::get<2>(quaternion), std::get<3>(quaternion)};
    sys.get_bodies().front()->update_body_states(states, tstart);

    controller.callback(scheduler, sys);
    const double first_expected_command = Kp * error;
    ASSERT_NEAR(first_expected_command, sys.get_input_value("propeller(psi_co)"), 1e-6);
}

TEST_F(PIDControllerTest, can_initialize_controllers)
{
    //! [controllersTest initialize_controllers]
    Sim sys = get_system(test_data::falling_ball_example(), 0);
    const double tstart = 0.1;
    const double dt = 1;
    ssc::solver::Scheduler scheduler(tstart, 10, dt);

    // initialize setpoints
    const double rpm_co = 0.45;
    const double pd_co = 0.5;
    sys.set_command_listener(std::map<std::string, double>({ { "t", tstart }, { "rpm_co", rpm_co }, { "P/D_co", pd_co } }));

    // initialize states
    const double u = 1.0;
    const StateType states = {100.0, 200.0, 300.0, u, 2.0, 3.0, 4.0, 5.0, 6.0, 1, 0, 0, 0};
    sys.get_bodies().front()->update_body_states(states, tstart);

    std::stringstream yaml_controllers;
    yaml_controllers << "controllers:\n"
                   "  - type: PID\n"
                   "    dt: 0.15\n"
                   "    setpoint: rpm_co\n"
                   "    state weights:\n"
                   "        u: 0.5\n"
                   "    command: propeller(rpm)\n"
                   "    gains:\n"
                   "        Kp: 4.2\n"
                   "        Ki: 0.25\n"
                   "        Kd: 1\n"
                   "  - type: PID\n"
                   "    dt: 0.5\n"
                   "    setpoint: P/D_co\n"
                   "    state weights:\n"
                   "        u: 0\n"
                   "    command: controller(P/D)\n"
                   "    gains:\n"
                   "        Kp: 1\n"
                   "        Ki: 0\n"
                   "        Kd: 0\n";

    const auto controllers = get_initialized_controllers(tstart,
                                             parse_controller_yaml(yaml_controllers.str()),
                                             std::vector<YamlTimeSeries>(), scheduler, sys
                                             );

    // Check controllers commands have been initialized in the datasource
    ASSERT_EQ(tstart, scheduler.get_time());
    ASSERT_NEAR(4.2 * (rpm_co - u * 0.5 ), sys.get_input_value("propeller(rpm)"), 1e-6);
    ASSERT_NEAR(pd_co, sys.get_input_value("controller(P/D)"), 1e-6);
    ASSERT_EQ(0, scheduler.get_discrete_state_updaters_to_run().size()); // no controller callback should be called anymore at t=tstart

    // Check 'propeller' callback has been added to the scheduler.
    scheduler.advance_to_next_time_event();
    ASSERT_EQ(tstart + 0.15, scheduler.get_time());
    ASSERT_EQ(1, scheduler.get_discrete_state_updaters_to_run().size()); // propeller callback

    // Check 'propeller' callback has been added to the scheduler.
    scheduler.advance_to_next_time_event();
    ASSERT_EQ(tstart + 0.5, scheduler.get_time());
    ASSERT_EQ(1, scheduler.get_discrete_state_updaters_to_run().size()); // controller callback

    //! [controllersTest initialize_controllers]
}


TEST_F(PIDControllerTest, can_get_the_commands_outputted_by_the_controller)
{
    const PIDController controller(a.random<double>(), a.random<double>(), pid_specific_yaml(a.random<double>(), a.random<double>(), a.random<double>(), "psi", "propeller(psi_co)") + "state weights:\n    psi: 1\n");
    ASSERT_EQ(1, controller.get_command_names().size());
    ASSERT_EQ("propeller(psi_co)", controller.get_command_names().at(0));
}