"""Extra observation model"""

import force
import logging
from typing import Any, Dict
SERVICE_NAME = "model.py"

logging.basicConfig(
    format='%(asctime)s,%(msecs)d ['
    + SERVICE_NAME
    + '] - %(levelname)-4s [%(filename)s:%(lineno)d] %(message)s',
    datefmt='%d-%m-%Y:%H:%M:%S')
LOGGER = logging.getLogger(__name__)
LOGGER.setLevel(logging.INFO)


class Model(force.Model):
    """
    """
    def __init__(self, _, body_name: str, __):
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
        return {'max_history_length': 0, 'needs_wave_outputs': True,
                'required_commands': [],
                'frame': self.body_name,
                'x': 0, 'y': 0, 'z': 0, 'phi': 0, 'theta': 0, 'psi': 0}

    def required_wave_information(self, t, x, y, z):
        return {'spectrum': {'t':t, 'x':x, 'y': y},
                'angular_frequencies_for_rao': True,
                'directions_for_rao': True}

    def force(self, _, __, wave_information, ___):
        """Force model."""

        return {'Fx': 0.0,
                'Fy': 0.0,
                'Fz': 0.0,
                'Mx': 0.0,
                'My': 0.0,
                'Mz': 0.0,
                'extra_observations': {'HF': 666.0, 'phase0': wave_information.spectrum.spectrum[0].phase[0].phase[0]}}


if __name__ == '__main__':
    force.serve(Model)
