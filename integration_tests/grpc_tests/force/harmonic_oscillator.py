"""Damped harmonic oscillator model."""

from typing import Any, Dict

import yaml

from xdyngrpc.forces import force


class HarmonicOscillator(force.AbstractForceModel):
    """Restoring force F proportional to the displacement x."""

    def __init__(self, parameters: str, body_name: str, _):
        """Initialize parameters from gRPC's set_parameters.

        Parameter k is stiffness and c is damping.
        """
        param = yaml.safe_load(parameters)
        self.k = param["k"]
        self.c = param["c"]
        self.body_name = body_name

    def get_parameters(self) -> Dict[str, Any]:
        """
        Return a dictionary with all parameters needed by xdyn, with keys:

        Returns
        -------
        dict
            Should contain the following fields:
            - `max_history_length` (float): How far back (in seconds) should the
              history values in ForceRequest go?
            - `needs_wave_outputs` (bool): Should the force model be queried at
              each time step using the 'required_wave_information' rpc method
              to know what wave information it requires?
            - `required_commands` (List[str]): List of commands needed by this
              model, without the model name (e.g. ['beta1', 'beta2'])
            - `frame` (str): Reference frame from which we define the
              reference in which the forces and torques are expressed.
            - `x` (float): Position along the x-axis of 'frame' of the point of
              application of the force.
            - `y` (float): Position along the y-axis of 'frame' of the point of
              application of the force.
            - `z` (float): Position along the z-axis of 'frame' of the point of
              application of the force.
            - `phi` (float): First Euler angle defining the rotation from
              'frame' to the reference frame in which the forces and torques
              are expressed. Depends on the angle convention chosen in the
              'rotations convention' section of xdyn's input file. See xdyn's
              documentation for details.
            - `theta` (float): Second Euler angle defining the rotation from
              'frame' to the reference frame in which the forces and torques
              are expressed. Depends on the angle convention chosen in the
              'rotations convention' section of xdyn's input file. See xdyn's
              documentation for details.
            - `psi` (float): Third Euler angle defining the rotation from
              'frame' to the reference frame in which the forces and torques
              are expressed. Depends on the angle convention chosen in the
              'rotations convention' section of xdyn's input file.
              See xdyn's documentation for details.
        """
        return {
            "max_history_length": 0,
            "needs_wave_outputs": False,
            "frame": self.body_name,
            "x": 0,
            "y": 0,
            "z": 0,
            "phi": 0,
            "theta": 0,
            "psi": 0,
            "required_commands": ["omega"],
        }

    def force(self, states, commands, wave_information, filtered_states):
        """Calculate the force & torque.

        Parameters
        ----------
        - states : dict
            Should contain the following fields:
            - t (list of doubles): dates at which the state values are given.
              max(t)-min(t) <= max_history_length returned by set_parameters.
            - x (list of doubles): Value of x for each t. x[0] is the
              current (at t) projection on axis X of the NED frame of the
              vector between the origin of the NED frame and the origin of the
              BODY frame.
            - y (list of doubles): Value of y for each t. y[0] is the
              current (at t) projection on axis Y of the NED frame of the
              vector between the origin of the NED frame and the origin of the
              BODY frame.
            - z (list of doubles): Value of z for each t. z[0] is the
              current (at t) projection on axis Z of the NED frame of the
              vector between the origin of the NED frame and the origin of the
              BODY frame.
            - u (list of doubles): Value of u for each t. u[0] is the current
              (at t) projection on axis X of the NED frame of the vector of the
              ship's speed relative to the ground (BODY/NED).
            - v (list of doubles): Value of v for each t. v[0] is the current
              (at t) projection on axis Y of the NED frame of the vector of the
              ship's speed relative to the ground (BODY/NED).
            - w (list of doubles): Value of w for each t. w[0] is the current
              (at t) projection on axis Z of the NED frame of the vector of the
              ship's speed relative to the ground (BODY/NED).
            - p (list of doubles): Value of p for each t. p[0] is the
              current (at t) of the projection on axis X of the NED frame of
              the vector of the ship's rotation speed relative to the ground
              (BODY/NED).
            - q (list of doubles): Value of q for each t. q[0] is the
              current (at t) of the projection on axis Y of the NED frame of
              the vector of the ship's rotation speed relative to the ground
              (BODY/NED).
            - r (list of doubles): Value of r for each t. r[0] is the
              current (at t) of the projection on axis Z of the NED frame of
              the vector of the ship's rotation speed relative to the ground
              (BODY/NED).
            - qr (list of doubles): Value of qr for each t. qr[0] is the
              current (at t) value of the real part of the quaternion defining
              the rotation from the NED frame to the ship's BODY frame.
            - qi (list of doubles): Value of qi for each t. qi[0] is the
              current (at t) value of the first imaginary part of the
              quaternion defining the rotation from the NED frame to the ship's
              BODY frame.
            - qj (list of doubles): Value of qj for each t. qj[0] is the
              current (at t) value of the second imaginary part of the
              quaternion defining the rotation from the NED frame to the ship's
              BODY frame.
            - qk (list of doubles): Value of qk for each t. qk[0] is the
              current (at t) value of the third imaginary part of the
              quaternion defining the rotation from the NED frame to the ship's
              BODY frame.
            - phi (list of doubles): Value of phi for each t. phi[0] is the
              current value (at t) of one of the three Euler angles, calculated
              using the angle convention chosen in the 'rotations convention'
              section of xdyn's input file. See xdyn's documentation for details.
            - theta (list of doubles): Value of theta for each t. theta[0] is
              the current value (at t) of one of the three Euler angles, calcualted
              using the angle convention chosen in the 'rotations convention'
              section of xdyn's input file. See xdyn's documentation for details.
            - psi (list of doubles): Value of psi for each t. psi[0] is the
              current value (at t) of one of the three Euler angles, calculated
              using the angle convention chosen in the 'rotations convention'
              section of xdyn's input file. See xdyn's documentation for
              details.
            - rotations_convention (string): Angle convention chosen in xdyn's
              YAML file. Use it to check the convention is what you are
              expecting! Format: ["psi", "theta'", "phi''"].
        - commands (dict): contains the current (at t) values of all the
          commands available to all the controlled forces of the simulation.
          The name of the commands is: model_name(command_name) e.g.
          PropRudd(rpm)
        - wave_information (dict): wave information requested by the force
          model (rpc method 'required_wave_information'). Should contain the
          following fields:
            - elevations (dict):
              Should contain the following fields:
            - dynamic_pressures (dict):
              Should contain the following fields:
            - orbital_velocities (dict): Should contain the following fields:
              - vx (list of doubles): Projection on the X-axis of the
                Earth-centered, Earth-fixed North-East-Down reference frame of
                the velocity of each wave partical relative to the ground. In
                meters per second. Same size and ordering as input from rpc
                'required_wave_information' (orbital_velocities).
              - vy (list of doubles): Projection on the Y-axis of the
                Earth-centered, Earth-fixed North-East-Down reference frame of
                the velocity of each wave partical relative to the ground. In
                meters per second. Same size and ordering as input from rpc
                'required_wave_information' (orbital_velocities).
              - vz (list of doubles): Projection on the Z-axis of the
                Earth-centered, Earth-fixed North-East-Down reference frame of
                the velocity of each wave partical relative to the ground. In
                meters per second. Same size and ordering as input from rpc
                'required_wave_information' (orbital_velocities).
            - spectrum (dict): Should contain the following fields:
              - si (list of floats): Discretized spectral density for each
                omega (should therefore be the same size as omega).
                In s m^2/rad.
              - dj (list of floats): Spatial spreading for each psi (should
                therefore be the same size as psi. In 1/rad.
              - omega (list of floats): Angular frequencies the spectrum was
                discretized at. In rad/s.
              - psi (list of floats): Directions between 0 & 2pi the spatial
                spreading was discretized at. In rad.
              - k (list of floats): Discretized wave number for each
                frequency (should therefore be the same size as omega).
                In rad/m.
              - phase (list of dict): Random phases, for each
                (direction,frequency) couple (but time invariant), should
                have the same size as psi. In radian.
                Each element is a dict containing the following fields:
                - phase (list of floats): Random phase. Should have the same
                  size as omega in parent dict. In radian.
            - angular_frequencies_for_rao (list of floats):
                Angular frequencies the spectrum was discretized at (in rad/s).
                Used, for example, when interpolating the wave RAOs.
            - directions_for_rao (list of floats):
                Wave incidences the spectrum was discretized at (in rad).
                0° is for waves coming from the South and propagating to the North.
                90° is for waves coming from the West and propagating to the East.
                Used, for example, when interpolating the wave RAOs.
        - filtered_states : class
            Should contain the following fields:
            - x (float): Current filtered projection on axis X of the NED frame of the
              vector between the origin of the NED frame and the origin of the
              BODY frame.
            - y (float): Current filtered projection on axis Y of the NED frame of the
              vector between the origin of the NED frame and the origin of the
              BODY frame.
            - z (float): Current filtered projection on axis Z of the NED frame of the
              vector between the origin of the NED frame and the origin of the
              BODY frame.
            - u (float): Current filtered projection on axis X of the NED frame
              of the vector of the ship's speed relative to the ground
              (BODY/NED).
            - v (float): Current filtered projection on axis Y of the NED frame
              of the vector of the ship's speed relative to the ground
              (BODY/NED).
            - w (float): Current filtered projection on axis Z of the NED frame
              of the vector of the ship's speed relative to the ground
              (BODY/NED).
            - p (float): Current filtered projection on axis X of the NED frame
              of the vector of the ship's rotation speed relative to the ground
              (BODY/NED).
            - q (float): Current filtered projection on axis Y of the NED frame
              of the vector of the ship's rotation speed relative to the ground
              (BODY/NED).
            - r (float): Current filtered projection on axis Z of the NED frame
              of the vector of the ship's rotation speed relative to the ground
              (BODY/NED).
            - phi (float): Current filtered value of one of the three Euler
              angles, calculated using the angle convention chosen in the
              'rotations convention' section of xdyn's input file. See xdyn's
              documentation for details.
            - theta (float): Current filtered value of one of the three Euler
              angles, calculated using the angle convention chosen in the
              'rotations convention' section of xdyn's input file. See xdyn's
              documentation for details.
            - psi (float): Current filtered value of one of the three Euler
              angles, calculated using the angle convention chosen in the
              'rotations convention' section of xdyn's input file. See xdyn's
              documentation for details.
            - rotations_convention (string): Angle convention chosen in xdyn's
              YAML file. Use it to check the convention is what you are
              expecting! Format: ["psi", "theta'", "phi''"].


        Returns
        -------
        dict
            Should contain the following fields:
            - Fx (double) Projection of the force acting on "BODY" on the
              X-axis of the body frame (in N).
            - Fy (double) Projection of the force acting on "BODY" on the
              Y-axis of the body frame (in N).
            - Fz (double) Projection of the force acting on "BODY" on the
              Z-axis of the body frame (in N).
            - Mx (double) Projection of the torque acting on "BODY" on the
              X-axis of the body frame (in N.m).
            - My (double) Projection of the torque acting on "BODY" on the
              Y-axis of the body frame (in N.m).
            - Mz (double) Projection of the torque acting on "BODY" on the
              Z-axis of the body frame (in N.m).
            - extra_observations (dict string -> double): Anything we wish
              to serialize. Specific to each force model. Not taken into
              account in the numerical integration & not available to other
              force or environment models.

        """
        omega = commands["omega"]
        return {
            "Fx": omega * (-self.k * states.x[0] - self.c * states.u[0]),
            "Fy": 0,
            "Fz": 0,
            "Mx": 0,
            "My": 0,
            "Mz": 0,
            "extra_observations": {"k": 2, "harmonic_oscillator_time": states.t[0]},
        }


if __name__ == "__main__":
    force.serve(HarmonicOscillator)
