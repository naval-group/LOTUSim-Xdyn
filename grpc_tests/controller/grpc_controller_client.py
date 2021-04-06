"""gRPC sample client for controllers."""

from typing import Dict, List
from concurrent import futures
import json
import time
import logging

import grpc
import controller
import controller_pb2
import controller_pb2_grpc

_ONE_DAY_IN_SECONDS = 60 * 60 * 24

SERVICE_NAME = "controller-client"

logging.basicConfig(
    format="%(asctime)s,%(msecs)d ["
    + SERVICE_NAME
    + "] - %(levelname)-4s [%(filename)s:%(lineno)d] %(message)s",
    datefmt="%d-%m-%Y:%H:%M:%S",
)
LOGGER = logging.getLogger(__name__)
LOGGER.setLevel(logging.INFO)


def to_grpc_states(
    states: controller.StatesEuler
) -> controller_pb2.ControllerStatesEuler:
    """Convert Python dataclass to protobuf."""
    ret = controller_pb2.ControllerStatesEuler()
    ret.t = states.t
    ret.x = states.x
    ret.y = states.y
    ret.z = states.z
    ret.u = states.u
    ret.v = states.v
    ret.w = states.w
    ret.p = states.p
    ret.q = states.q
    ret.r = states.r
    ret.phi = states.phi
    ret.theta = states.theta
    ret.psi = states.psi
    return ret


class Controller:
    """This class wraps the gRPC calls for use with "normal" python types."""

    def __init__(self, channel, parameters, t0=0):
        """Constructor."""
        self.stub = controller_pb2_grpc.ControllerStub(channel)
        self.date_of_first_callback = None
        self.setpoint_names = None
        self.angle_representation = None
        self.has_extra_observations = None
        self.set_parameters(parameters, t0)

    def set_parameters(self, yaml_string: str, t0: float) -> str:
        """Set the controller's YAML parameters."""
        request = controller_pb2.SetParametersRequest(
            parameters=yaml_string, t0=t0)
        response = self.stub.set_parameters(request)
        self.date_of_first_callback = response.date_of_first_callback
        self.setpoint_names = response.setpoint_names
        self.angle_representation = response.angle_representation
        self.has_extra_observations = response.has_extra_observations

    def get_commands_euler_321(
        self,
        states: controller.StatesEuler,
        dstates_dt: controller.StatesEuler,
        setpoints: List[float],
    ) -> Dict[str, float]:
        """Calculates the controller outputs (the commands)."""
        request = controller_pb2.ControllerRequestEuler()
        grpc_states = to_grpc_states(states)
        grpc_dstates_dt = to_grpc_states(dstates_dt)
        request.states = grpc_states
        request.dstates_dt = grpc_dstates_dt
        request.setpoints[:] = setpoints
        response = self.stub.get_commands_euler_321(request)
        return response.commands


def run():
    """Launch the server & run some gRPC calls."""
    with grpc.insecure_channel("controller:50051") as channel:
        yaml_parameters = """
            command: beta,
            dt: 0.5
            setpoint: rpm_co
            weights:
                x: 1
                y: -1
            gains:
                Kp: 2.5
                Ki: 0.1
                Kd: 0.314
        """
        LOGGER.info("Creating PID controller instance")
        pid = Controller(channel, yaml_parameters)
        # First time step
        error = 0.05
        rpm_co = 5
        states = controller.StatesEuler(
            x=2 * rpm_co,
            y=rpm_co + error,
            z=300.0,
            u=1.0,
            v=2.0,
            w=3.0,
            p=4.0,
            q=5.0,
            r=6.0,
        )
        dstates_dt = controller.StatesEuler()

        first_expected_command = Kp * error
        LOGGER.info(pid.get_commands_euler_321(states, dstates_dt, [rpm_co]))


if __name__ == "__main__":
    LOGGER.info("Starting client")
    run()
