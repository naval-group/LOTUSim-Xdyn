"""Extra observation model"""

from xdyngrpc.forces import force
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


class Model(force.AbstractForceModel):
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

    def required_wave_information(self, t: float, x: float, y: float, z: float):
        """Give the list of wave data needed by this model.

        Only called if "needs_wave_outputs" is True.

        Parameters
        ----------
        t : float
            Simulation time (in seconds)
        x : float
            Projection on axis X of the NED frame of the vector between the
            origin of the NED frame and the origin of the BODY frame
        y : float
            Projection on axis Y of the NED frame of the vector between the
            origin of the NED frame and the origin of the BODY frame
        z : float
            Projection on axis Z of the NED frame of the vector between the
            origin of the NED frame and the origin of the BODY frame

        Returns
        -------
        dict
            Should contain the following fields:
            - elevations (dict): Each element should contain
                - x (list of float) Points at which the force model requires
                  wave elevations (projection on X-axis, in meters, expressed
                  in the Earth-centered, Earth-fixed North-East-Down reference
                  frame.
                - y (list of float) Points at which the force model requires
                  wave elevations (projection on Y-axis, in meters, expressed
                  in the Earth-centered, Earth-fixed North-East-Down reference
                  frame.
                - t (double) Time at which the force model requires wave
                  elevations (in seconds)
            - dynamic_pressures (dict): Each element should contain
                - x (list of float) Points at which the force model requires
                  dynamic pressures (projection on X-axis, in meters, expressed
                  in the Earth-centered, Earth-fixed North-East-Down reference
                  frame.
                - y (list of float) Points at which the force model requires
                  dynamic pressures (projection on Y-axis, in meters, expressed
                  in the Earth-centered, Earth-fixed North-East-Down reference
                  frame.
                - z (list of float) Points at which the force model requires
                  dynamic pressures (projection on Z-axis, in meters, expressed
                  in the Earth-centered, Earth-fixed North-East-Down reference
                  frame.
                - t (double) Time at which the force model requires dynamic
                  pressures (in seconds)
            - orbital_velocities (dict): Each element should contain
                - x (list of float) Points at which the force model requires
                  orbital velocities (projection on X-axis, in meters,
                  expressed in the Earth-centered, Earth-fixed North-East-Down
                  reference frame.
                - y (list of float) Points at which the force model requires
                  orbital velocities (projection on Y-axis, in meters,
                  expressed in the Earth-centered, Earth-fixed North-East-Down
                  reference frame.
                - z (list of float) Points at which the force model requires
                  orbital velocities (projection on Z-axis, in meters,
                  expressed in the Earth-centered, Earth-fixed North-East-Down
                  reference frame.
                - t (double) Time at which the force model requires orbital
                  velocities (in seconds)
            - spectrum (dict or None): If it's a dict, should contain:
                - x (double) Projection on the X axis of the NED frame of the
                  position at which we want the linearized wave spectrum
                - y (double) Projection on the Y axis of the NED frame of the
                  position at which we want the linearized wave spectrum
                - t (double) Simulation time at which we want the linearized
                  wave spectrum
            - angular_frequencies_for_rao (bool): True if the force model
              requires the angular frequencies at which the wave spectrum is
              discretized
            - directions_for_rao (bool): True if the force mode requires the
              incidences at which the wave spectrum is discretized

        """
        return {'spectrum': {'t':t, 'x':x, 'y': y},
                'angular_frequencies_for_rao': True,
                'directions_for_rao': True}

    def force(self, _states, _commands, wave_information, _filtered_states):
        """Calculate the force & torque.

        Parameters
        ----------
        - states : dict
            Should contain the following fields:
            - t (list of float): dates at which the state values are given.
              max(t)-min(t) <= max_history_length returned by set_parameters.
            - x (list of float): Value of x for each t. x[0] is the
              current (at t) projection on axis X of the NED frame of the
              vector between the origin of the NED frame and the origin of the
              BODY frame.
            - y (list of float): Value of y for each t. y[0] is the
              current (at t) projection on axis Y of the NED frame of the
              vector between the origin of the NED frame and the origin of the
              BODY frame.
            - z (list of float): Value of z for each t. z[0] is the
              current (at t) projection on axis Z of the NED frame of the
              vector between the origin of the NED frame and the origin of the
              BODY frame.
            - u (list of float): Value of u for each t. u[0] is the current
              (at t) projection on axis X of the NED frame of the vector of the
              ship's speed relative to the ground (BODY/NED).
            - v (list of float): Value of v for each t. v[0] is the current
              (at t) projection on axis Y of the NED frame of the vector of the
              ship's speed relative to the ground (BODY/NED).
            - w (list of float): Value of w for each t. w[0] is the current
              (at t) projection on axis Z of the NED frame of the vector of the
              ship's speed relative to the ground (BODY/NED).
            - p (list of float): Value of p for each t. p[0] is the
              current (at t) of the projection on axis X of the NED frame of
              the vector of the ship's rotation speed relative to the ground
              (BODY/NED).
            - q (list of float): Value of q for each t. q[0] is the
              current (at t) of the projection on axis Y of the NED frame of
              the vector of the ship's rotation speed relative to the ground
              (BODY/NED).
            - r (list of float): Value of r for each t. r[0] is the
              current (at t) of the projection on axis Z of the NED frame of
              the vector of the ship's rotation speed relative to the ground
              (BODY/NED).
            - qr (list of float): Value of qr for each t. qr[0] is the
              current (at t) value of the real part of the quaternion defining
              the rotation from the NED frame to the ship's BODY frame.
            - qi (list of float): Value of qi for each t. qi[0] is the
              current (at t) value of the first imaginary part of the
              quaternion defining the rotation from the NED frame to the ship's
              BODY frame.
            - qj (list of float): Value of qj for each t. qj[0] is the
              current (at t) value of the second imaginary part of the
              quaternion defining the rotation from the NED frame to the ship's
              BODY frame.
            - qk (list of float): Value of qk for each t. qk[0] is the
              current (at t) value of the third imaginary part of the
              quaternion defining the rotation from the NED frame to the ship's
              BODY frame.
            - phi (list of float): Value of phi for each t. phi[0] is the
              current value (at t) of one of the three Euler angles, calculated
              using the angle convention chosen in the 'rotations convention'
              section of xdyn's input file. See xdyn's documentation for details.
            - theta (list of float): Value of theta for each t. theta[0] is
              the current value (at t) of one of the three Euler angles, calcualted
              using the angle convention chosen in the 'rotations convention'
              section of xdyn's input file. See xdyn's documentation for details.
            - psi (list of float): Value of psi for each t. psi[0] is the
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
              - vx (list of float): Projection on the X-axis of the
                Earth-centered, Earth-fixed North-East-Down reference frame of
                the velocity of each wave partical relative to the ground. In
                meters per second. Same size and ordering as input from rpc
                'required_wave_information' (orbital_velocities).
              - vy (list of float): Projection on the Y-axis of the
                Earth-centered, Earth-fixed North-East-Down reference frame of
                the velocity of each wave partical relative to the ground. In
                meters per second. Same size and ordering as input from rpc
                'required_wave_information' (orbital_velocities).
              - vz (list of float): Projection on the Z-axis of the
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
        return {'Fx': 0.0,
                'Fy': 0.0,
                'Fz': 0.0,
                'Mx': 0.0,
                'My': 0.0,
                'Mz': 0.0,
                'extra_observations': {'HF': 666.0, 'phase0': wave_information.spectrum.spectrum[0].phase[0].phase[0]}}


if __name__ == '__main__':
    force.serve(Model)
