"""Integration tests for gRPC wave models."""

import pid_controller
import math
import yaml
import controller


def test_can_call_pid_controller():
    psi_co = math.pi / 3
    dt = 2.3
    yaml_string = yaml.dump(
        {
            "command": "beta",
            "type": "PID",
            "dt": dt,
            "setpoint": psi_co,
            "weights": {"psi": 1},
            "gains": {"Kp": -1, "Ki": 0, "Kd": -1}
        }
    )
    pid_controller.PIDController(yaml_string, dt)


def test_should_get_the_right_number_of_inputs():
    dt = 2.3
    controller = pid_controller.PIDController(get_yaml(dt), dt)
    assert controller.get_setpoint_names() == ["psi_co"]


def test_can_get_plant_output():
    dt = 2.3
    yaml_string = yaml.dump(
        {
            "command": "beta",
            "type": "PID",
            "dt": dt,
            "setpoint": "psi_co",
            "weights": {"x": 2, "phi": 2, "p": -1},
            "gains": {"Kp": -1, "Ki": 0, "Kd": -1}
        })
    pid = pid_controller.PIDController(yaml_string, dt)
    states = controller.StatesEuler()
    states.x = 1.3
    states.phi = 2.7
    states.p = -2
    assert pid.get_plant_output(states) == 10
