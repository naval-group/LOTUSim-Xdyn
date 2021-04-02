"""Simple PID controller."""

from typing import Dict, List
import yaml
from controller import StatesEuler, StatesQuaternion
import controller


class PIDController(controller.Model):
    """Restoring force F proportional to the displacement x."""

    def __init__(self, parameters: str, t0: float):
        """Initialize parameters from gRPC's set_parameters."""
        param = yaml.safe_load(parameters)
        self.timestep = param["dt"]
        super(PIDController, self).__init__(t0, self.timestep)
