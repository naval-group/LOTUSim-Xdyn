"""Integration tests for gRPC wave models."""

import pid_controller
import math
import yaml


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

