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
        self.weights = param["weights"]
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

    def get_plant_output(self, states: StatesEuler) -> float:
        """Calculates the linear combination of states used to compute the
        command."""
        return (
            self.weights.get("x", 0) * states.x
            + self.weights.get("y", 0) * states.y
            + self.weights.get("z", 0) * states.z
            + self.weights.get("u", 0) * states.u
            + self.weights.get("v", 0) * states.v
            + self.weights.get("w", 0) * states.w
            + self.weights.get("p", 0) * states.p
            + self.weights.get("q", 0) * states.q
            + self.weights.get("r", 0) * states.r
            + self.weights.get("phi", 0) * states.phi
            + self.weights.get("theta", 0) * states.theta
            + self.weights.get("psi", 0) * states.psi
        )

