"""Simple PID controller."""

from typing import Dict, List
import yaml
from controller import StatesEuler
import controller
import logging

SERVICE_NAME = "pid-controller"

logging.basicConfig(
    format='%(asctime)s,%(msecs)d ['
    + SERVICE_NAME
    + '] - %(levelname)-4s [%(filename)s:%(lineno)d] %(message)s',
    datefmt='%d-%m-%Y:%H:%M:%S')
LOGGER = logging.getLogger(__name__)
LOGGER.setLevel(logging.INFO)


class PIDController(controller.Model):
    """Cf. https://en.wikipedia.org/wiki/PID_controller"""

    def __init__(self, parameters: str, t0: float):
        """Initialize parameters from gRPC's set_parameters."""
        param = yaml.safe_load(parameters)
        self.derivative_gain = param["gains"]["Kd"]
        self.proportional_gain = param["gains"]["Kp"]
        self.integral_gain = param["gains"]["Ki"]
        self.setpoint_name = param["setpoint"]
        self.command_name = param["command"]
        self.weights = param["state weights"]
        self.t_start = t0
        self.previous_error = 0
        self.integral = 0
        self.can_use_integrator_and_derivative = False
        super(PIDController, self).__init__(t0, param['dt'])

    def get_angle_representation(self) -> str:
        """Which method should we call to get the commands computed by the controller?

        'QUATERNION' -> get_commands_quaternion
        'EULER_321' -> get_commands_euler_321
        """
        return 'EULER_321'

    def get_command_names(self) -> List[str]:
        """Return the name(s) of the controller outputs (commands).

        This will be used by the solver (e.g., xdyn) to set the value of the corresponding variables.
        The names are assumed to be in the same order as the numerical values computed by 'get_commands*'

        Returns
        -------
        - commands (List[str]): commands computed by this controller
        """
        return [self.command_name]


    def get_setpoint_names(self) -> List[str]:
        """Return the name(s) of the controller inputs (setpoints).

        These setpoints are given by the simulator

        Returns
        -------
        - commands (List[str]): commands used by xdyn's controlled forces
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

    def get_commands_euler_321(
        self,
        states: StatesEuler,
        dstates_dt: StatesEuler,
        setpoints: List[float],
    ) -> Dict[str, float]:
        """Calculate the commands using angles in Rned2body = Rz(ψ).Ry(θ).Rx(ϕ)
            rotation convention

            Parameters
            ----------
            - states (StatesEuler): latest ship states
            - dstates_dt (StatesEuler): ship states derivative at the
                                             previous timestep
            - setpoints (List[float]): controller inputs (setpoints)

            Returns
            -------
            - commands (Dict[str,float]): commands used by xdyn's controlled
                                          forces
            """
        error = setpoints[0] - self.get_plant_output(states)
        # Proportional term
        proportional_term = self.proportional_gain * error
        integral_term = 0
        derivative_term = 0

        assert states.t < self.t_start, f"Expected t >= {self.t_start} but got {states.t}"

        if self.can_use_integrator_and_derivative:
            # Integral term
            self.integral += error * self.dt
            integral_term = self.integral_gain * self.integral

            # Derivative term
            derivative_term = (
                self.derivative_gain *
                (error - self.previous_error) / self.dt
            )

        # Store error for next time step
        self.previous_error = error

        # As integrator and derivative have been initialized, we can now use
        # them
        self.can_use_integrator_and_derivative = True
        return {
            self.command_name: proportional_term
            + integral_term
            + derivative_term
        }


# Start the gRPC server loop
if __name__ == "__main__":
    LOGGER.info('Starting gRPC PID controller')
    controller.serve(PIDController)
