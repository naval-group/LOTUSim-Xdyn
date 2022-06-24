"""Force model using filtered states."""

import numpy as np
import force
import logging
from typing import Any, Dict

SERVICE_NAME = "grpc-force"

logging.basicConfig(
    format='%(asctime)s,%(msecs)d ['
    + SERVICE_NAME
    + '] - %(levelname)-4s [%(filename)s:%(lineno)d] %(message)s',
    datefmt='%d-%m-%Y:%H:%M:%S')
LOGGER = logging.getLogger(__name__)
LOGGER.setLevel(logging.INFO)



class ForceModel(force.Model):
    """Using filtered states."""

    def __init__(self, _, body_name: str, __):
        """Initialize parameters from gRPC's set_parameters."""
        self.body_name = body_name

    def get_parameters(self) -> Dict[str, Any]:
        """
        Return a dictionary with all parameters needed by xdyn, with keys:

        - `max_history_length` (float) How far back (in seconds) should the history values in
           ForceRequest go?
        - `needs_wave_outputs` (bool) Should the force model be queried at each time step using
           the `required_wave_information` rpc method to know what wave information it requires?
        - `frame` (str) Reference frame from which we define the reference in which the forces
           and torques are expressed.
        - `x` (float) Position along the x-axis of 'frame' of the point of application of the force.
        - `y` (float) Position along the y-axis of 'frame' of the point of application of the force.
        - `z` (float) Position along the z-axis of 'frame' of the point of application of the force.
        - `phi` (float) First Euler angle in radian.
        - `theta` (float) Second Euler angle in radian.
        - `psi` (float) Third Euler angle in radian.
        - `required_commands` (List[str]) List of commands needed by this model, without the model
           name (e.g. ['beta1', 'beta2'])

        `phi`, `theta`, `psi` are the three Euler angles defining the rotation from 'frame' to
        the reference frame in which the forces and torques are expressed. Depends on the angle
        convention chosen in the 'rotations convention' section of xdyn's input file.
        See xdyn's documentation for details.

        Just using defaults.
        """
        # Initialize random seed once per simulation to get predictable results
        np.random.seed(0)
        return {'max_history_length': 0, 'needs_wave_outputs': False,
                'frame': self.body_name, 'x': 0, 'y': 0, 'z': 0, 'phi': 0,
                'theta': 0, 'psi': 0, 'required_commands': []}

    def force(self, states, _, __, filtered_states):
        """Force model."""
        Fx = 1 + np.random.uniform(low=-0.01, high=0.01)
        Fz = np.random.uniform(low=-0.01, high=0.01)
        return {'Fx': Fx,
                'Fy': 0,
                'Fz': Fz,
                'Mx': 0,
                'My': 0,
                'Mz': 0,
                'extra_observations': {'xf': filtered_states.x, 'x_': states.x[0]}}


if __name__ == '__main__':
    force.serve(ForceModel)
