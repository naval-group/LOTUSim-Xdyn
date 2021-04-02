"""Integration tests for gRPC wave models."""

import pid_controller
import math
import yaml
import controller


def get_yaml(dt):
    """Generate the YAML expected by the PID controller."""
    return yaml.dump(
        {
            "command": "beta",
            "type": "PID",
            "dt": dt,
            "setpoint": "psi_co",
            "weights": {"x": 2, "phi": 2, "p": -1},
            "gains": {"Kp": -1, "Ki": 0, "Kd": -1}
        }
    )


def test_can_call_pid_controller():
    dt = 2.3
    pid_controller.PIDController(get_yaml(dt), dt)


def test_should_get_the_right_number_of_inputs():
    dt = 2.3
    controller = pid_controller.PIDController(get_yaml(dt), dt)
    assert controller.get_setpoint_names() == ["psi_co"]


def test_can_get_plant_output():
    dt = 2.3
    pid = pid_controller.PIDController(get_yaml(dt), dt)
    states = controller.StatesEuler()
    states.x = 1.3
    states.phi = 2.7
    states.p = -2
    assert pid.get_plant_output(states) == 10


def test_can_get_pid_commands():
    dt = 0.5
    Kp = 2.5
    Ki = 0.1
    Kd = 0.314
    pid_config = yaml.dump(
        {
            "command": "beta",
            "type": "PID",
            "dt": dt,
            "setpoint": "rpm_co",
            "weights": {"x": 1, "y": -1},
            "gains": {"Kp": Kp, "Ki": Ki, "Kd": Kd}
        })
    pid = pid_controller.PIDController(pid_config, dt)

    # First time step
    error = 0.05
    rpm_co = 5
    states = controller.StatesEuler(
        x=2 * rpm_co, y=rpm_co + error, z=300.0, u=1.0, v=2.0, w=3.0, p=4.0,
        q=5.0, r=6.0)
    dstates_dt = controller.StatesEuler()

    first_expected_command = Kp * error
    assert abs(pid.get_commands_euler_321(states, dstates_dt,
               [rpm_co])['beta'] - first_expected_command) < 1E-6

    # Second time step
    error2 = -0.03
    rpm_co = 1.5
    states.x = 2 * rpm_co
    states.y = rpm_co + error2
    second_expected_command = Kp * error2 + Ki * \
        error2 * dt + Kd * (error2 - error) / dt
    assert abs(pid.get_commands_euler_321(states, dstates_dt,
               [rpm_co])['beta'] - second_expected_command) < 1E-6

    # Third time step
    error3 = 0.01
    rpm_co = 5.5
    states.x = 2 * rpm_co
    states.y = rpm_co + error3
    third_expected_command = Kp * error3 + Ki * \
        (error2 * dt + error3 * dt) + Kd * (error3 - error2) / dt

    assert abs(pid.get_commands_euler_321(states, dstates_dt,
               [rpm_co])['beta'] - third_expected_command) < 1E-6
