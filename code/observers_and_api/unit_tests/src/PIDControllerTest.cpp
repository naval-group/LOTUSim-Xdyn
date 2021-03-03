/*
 * PIDControllerTest.cpp
 *
 *  Created on: Fev 12, 2021
 *      Author: lincker
 */

#include "simulator_api.hpp"
#include "Scheduler.hpp"
#include "DiscreteSystem.hpp"
#include "PIDControllerTest.hpp"
#include "PIDController.hpp"
#include "parse_commands.hpp"
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

std::string PIDControllerTest::yaml_gains(const double Kp, const double Ki, const double Kd)
{
    std::stringstream yaml_string;
    yaml_string << "gains:\n"
                << "    Kp: " << Kp << "\n"
                << "    Ki: " << Ki << "\n"
                << "    Kd: " << Kd << "\n";
    return yaml_string.str();
}

TEST_F(PIDControllerTest, can_parse_controller_gains)
{
    const double Kp = 2.5;
    const double Ki = -0.105;
    const double Kd = 1;

    const std::map<std::string, double> states_coeff { { "x", -1 }, { "y", 2 } };
    const PIDController controller = PIDController(0.2, "propeller(rpm)", "propeller(rpm_co)", states_coeff, yaml_gains(Kp, Ki, Kd));

    ASSERT_DOUBLE_EQ(Kp, controller.yaml.Kp);
    ASSERT_DOUBLE_EQ(Ki, controller.yaml.Ki);
    ASSERT_DOUBLE_EQ(Kd, controller.yaml.Kd);
}

TEST_F(PIDControllerTest, can_get_the_controller_name)
{
    const double dt = 0.5;
    const double Kp = 2.5;
    const double Ki = 0.1;
    const double Kd = 0.314;
    const std::map<std::string, double> states_coeff { { "x", -1 }, { "y", 2 } };
    const PIDController controller(dt, "propeller(rpm)", "propeller(rpm_co)", states_coeff, yaml_gains(Kp, Ki, Kd));
    ASSERT_EQ("propeller(rpm)", controller.get_name());
}

TEST_F(PIDControllerTest, update_command_in_ds_example)
{
    const double dt = 0.5;
    const double Kp = 2.5;
    const double Ki = 0.1;
    const double Kd = 0.314;
    const std::map<std::string, double> states_coeff { { "x", 1 }, { "y", -1 } };
    PIDController controller(dt, "propeller(rpm)", "propeller(rpm_co)", states_coeff, yaml_gains(Kp, Ki, Kd));

    const double rpm_co = 5;
    const double x = 2 * rpm_co;
    const double y = rpm_co + 0.05;

    Sim sys = get_system(test_data::falling_ball_example(), 0);

    const double tstart = 0.1;
    ssc::solver::Scheduler scheduler(tstart, 10, dt);

    //! [callback example]
    // First time step
    sys.set_command_listener(std::map<std::string, double>({ { "t", tstart }, { "propeller(rpm_co)", rpm_co } }));
    StateType states = {x, y, 300.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 1, 0, 0, 0};
    sys.get_bodies().front()->update_body_states(states, tstart);

    controller.callback(scheduler, &sys);
    const double error = rpm_co - (x - y);
    ASSERT_NEAR(Kp * error, sys.get_command("propeller(rpm)"), 1e-6);

    //! [callback example]
}

TEST_F(PIDControllerTest, can_compute_PID_commands)
{
    const double dt = 0.5;
    const double Kp = 2.5;
    const double Ki = 0.1;
    const double Kd = 0.314;
    const std::map<std::string, double> states_coeff { { "x", 1 }, { "y", -1 } };
    PIDController controller(dt, "propeller(rpm)", "propeller(rpm_co)", states_coeff, yaml_gains(Kp, Ki, Kd));

    Sim sys = get_system(test_data::falling_ball_example(), 0);

    const double tstart = 0.1;
    ssc::solver::Scheduler scheduler(tstart, 2, dt);

    // First time step
    ASSERT_EQ(tstart, scheduler.get_time());
    const double error = 0.05;
    double rpm_co = 5;
    double x = 2 * rpm_co;
    double y = rpm_co + error;
    sys.set_command_listener(std::map<std::string, double>({ { "t", tstart }, { "propeller(rpm_co)", rpm_co } }));

    StateType states = {x, y, 300.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 1, 0, 0, 0};
    sys.get_bodies().front()->update_body_states(states, tstart);

    controller.callback(scheduler, &sys);
    const double first_expected_command = Kp * error;
    ASSERT_NEAR(first_expected_command, sys.get_command("propeller(rpm)"), 1e-6);

    // Second time step
    scheduler.advance_to_next_time_event();
    ASSERT_EQ(tstart + dt, scheduler.get_time());
    const double error2 = -0.03;
    rpm_co = 1.5;
    x = 2 * rpm_co;
    y = rpm_co + error2;
    sys.set_command_listener(std::map<std::string, double>({ { "t", tstart + dt }, { "propeller(rpm_co)", rpm_co } }));
    states = {x, y, 300.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 1, 0, 0, 0};
    sys.get_bodies().front()->update_body_states(states, tstart + dt);

    controller.callback(scheduler, &sys);
    const double second_expected_command = Kp * error2 + Ki * error2 * dt + Kd * (error2 - error) / dt;
    ASSERT_NEAR(second_expected_command, sys.get_command("propeller(rpm)"), 1e-6);

    // Third time step
    scheduler.advance_to_next_time_event();
    ASSERT_EQ(tstart + dt, scheduler.get_time()); // `tstart + dt` has been added by the previous callback command
    scheduler.advance_to_next_time_event();
    ASSERT_EQ(tstart + 2 * dt, scheduler.get_time());
    const double error3 = 0.01;
    rpm_co = 5.5;
    x = 2 * rpm_co;
    y = rpm_co + error3;
    sys.set_command_listener(std::map<std::string, double>({ { "t", tstart + 2 * dt }, { "propeller(rpm_co)", rpm_co } }));
    states = {x, y, 300.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 1, 0, 0, 0};
    sys.get_bodies().front()->update_body_states(states, tstart + 2 * dt);

    controller.callback(scheduler, &sys);
    const double third_expected_command = Kp * error3 + Ki * (error2 * dt + error3 * dt) + Kd * (error3 - error2) / dt;
    ASSERT_NEAR(third_expected_command, sys.get_command("propeller(rpm)"), 1e-6);
}

TEST_F(PIDControllerTest, can_compute_PID_commands_several_times_at_first_time_step)
{
    const double dt = 0.5;
    const double Kp = 2.5;
    const double Ki = 0.1;
    const double Kd = 0.314;
    const std::map<std::string, double> states_coeff { { "x", 1 }, { "y", -1 } };
    PIDController controller(dt, "propeller(rpm)", "propeller(rpm_co)", states_coeff, yaml_gains(Kp, Ki, Kd));

    Sim sys = get_system(test_data::falling_ball_example(), 0);

    const double tstart = 0.1;
    ssc::solver::Scheduler scheduler(tstart, 10, dt);

    // First time step
    ASSERT_EQ(tstart, scheduler.get_time());
    const double error = 0.05;
    const double rpm_co = 5;
    const double x = 2 * rpm_co;
    const double y = rpm_co + error;
    sys.set_command_listener(std::map<std::string, double>({ { "t", tstart }, { "propeller(rpm_co)", rpm_co } }));
    const StateType states = {x, y, 300.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 1, 0, 0, 0};
    sys.get_bodies().front()->update_body_states(states, tstart);
    controller.callback(scheduler, &sys);
    const double first_expected_command = Kp * error;
    ASSERT_NEAR(first_expected_command, sys.get_command("propeller(rpm)"), 1e-6);

    // First time step again should send back the same result
    ASSERT_EQ(tstart, scheduler.get_time());
    controller.callback(scheduler, &sys);
    ASSERT_NEAR(first_expected_command, sys.get_command("propeller(rpm)"), 1e-6);

    // A time step before the next meeting point should send back the same result
    scheduler.add_time_event(tstart + dt / 2);
    scheduler.advance_to_next_time_event();
    ASSERT_EQ(tstart + dt / 2, scheduler.get_time());
    controller.callback(scheduler, &sys);
    ASSERT_NEAR(first_expected_command, sys.get_command("propeller(rpm)"), 1e-6);
}

TEST_F(PIDControllerTest, can_use_euler_angles_in_states)
{
    const double dt = 0.5;
    const double Kp = 2.5;
    const double Ki = 0.1;
    const double Kd = 0.314;
    const std::map<std::string, double> states_coeff { { "psi", 1 } };
    PIDController controller(dt, "propeller(psi_co)", "propeller(psi)", states_coeff, yaml_gains(Kp, Ki, Kd));

    Sim sys = get_system(test_data::falling_ball_example(), 0);

    const double tstart = 0.1;
    ssc::solver::Scheduler scheduler(tstart, 10, dt);

    const double psi = 0.45;
    const double error = -0.05;
    sys.set_command_listener(std::map<std::string, double>({ { "t", tstart }, { "propeller(psi)", psi } }));

    const ssc::kinematics::EulerAngles angles(0.2, 0.9, psi - error);
    std::tuple<double, double, double, double> quaternion = sys.get_bodies().front()->get_quaternions(angles, sys.get_env().rot);
    const StateType states = {100.0, 200.0, 300.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, std::get<0>(quaternion), std::get<1>(quaternion), std::get<2>(quaternion), std::get<3>(quaternion)};
    sys.get_bodies().front()->update_body_states(states, tstart);

    controller.callback(scheduler, &sys);
    const double first_expected_command = Kp * error;
    ASSERT_NEAR(first_expected_command, sys.get_command("propeller(psi_co)"), 1e-6);
}
