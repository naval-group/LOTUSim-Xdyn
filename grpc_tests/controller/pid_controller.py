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
        self.setpoint_name = param["setpoint"]
        super(PIDController, self).__init__(t0, self.timestep)

    def get_setpoint_names(self) -> List[str]:
        """Return the name(s) of the controller inputs (setpoints).

        These setpoints are given by the simulator

        Parameters
        ----------
        - states (StatesQuaternion): latest ship states
        - dstates_dt (StatesQuaternion): ship states derivative at the previous
                                         timestep

        Returns
        -------
        - commands (Dict[str,float]): commands used by xdyn's controlled forces
        """
        return [self.setpoint_name]

