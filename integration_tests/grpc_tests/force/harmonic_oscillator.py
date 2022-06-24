"""Damped harmonic oscillator model."""

import yaml
import force
from typing import Any, Dict


class HarmonicOscillator(force.Model):
    """Restoring force F proportional to the displacement x."""

    def __init__(self, parameters: str, body_name: str, _):
        """Initialize parameters from gRPC's set_parameters

        Parameter k is stiffness and c is damping
        """
        param = yaml.safe_load(parameters)
        self.k = param['k']
        self.c = param['c']
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
        """
        return {'max_history_length': 0, 'needs_wave_outputs': False,
                'frame': self.body_name, 'x': 0, 'y': 0, 'z': 0, 'phi': 0,
                'theta': 0, 'psi': 0, 'required_commands': ['omega']}

    def force(self, states, commands, _, __):
        """Force model."""
        omega = commands['omega']
        return {'Fx': omega*(-self.k*states.x[0] - self.c*states.u[0]),
                'Fy': 0,
                'Fz': 0,
                'Mx': 0,
                'My': 0,
                'Mz': 0,
                'extra_observations': {'k': 2, 'harmonic_oscillator_time': states.t[0]}}


if __name__ == '__main__':
    force.serve(HarmonicOscillator)
