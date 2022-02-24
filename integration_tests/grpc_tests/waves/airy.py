"""Airy wave model. As implemented in xdyn."""

import logging
import math
from typing import Any, Dict, List
import yaml
import numpy as np
import waves


SERVICE_NAME = "airy"

logging.basicConfig(
    format='%(asctime)s,%(msecs)d ['
    + SERVICE_NAME
    + '] - %(levelname)-4s [%(filename)s:%(lineno)d] %(message)s',
    datefmt='%d-%m-%Y:%H:%M:%S')
LOGGER = logging.getLogger(__name__)
LOGGER.setLevel(logging.INFO)


def pdyn_factor(k: float, z: float, eta: float) -> float:
    """exp(-kz) factor used for the dynamic pressure calculations.

    Parameters
    ----------
    k : float
        Wave number (in metres^-1).
    z : float
        Position (in meters) at which we want the elevation. Projected on
        the Z-axis of the Earth-centered, Earth-fixed North-East-Down
        reference frame.
    eta : float
        Wave elevation (in meters) in the North-East-Down reference frame.

    Returns
    -------
    float
        dynamic pressure factor.

    """
    return math.exp(-k * z) if z >= eta else 1


def create_vector_delta_omega(omega_vector: np.ndarray) -> np.ndarray:
    """Create the delta omega vector from an omega discretization

    omega vector has to be sorted in an increasing or decreasing order.


    Parameters
    ----------
    omega_vector : np.ndarray.
        omega wave vector (in rad/s).

    Returns
    -------
    np.ndarray
        Vector of floats containing the size of each bin in rad/s.
        Each element is positive.

    """
    delta_omega = np.zeros_like(omega_vector, dtype=float)
    if len(omega_vector) == 1:
        delta_omega[0] = 1
    else:
        for i, _ in enumerate(omega_vector):
            if i == 0:
                delta_omega[i] = abs(omega_vector[1] - omega_vector[0]) / 2
            elif i == len(omega_vector) - 1:
                delta_omega[i] = abs(omega_vector[i] - omega_vector[i - 1]) / 2
            else:
                delta_omega[i] = (
                    abs(omega_vector[i] - omega_vector[i - 1]) / 2
                    + abs(omega_vector[i + 1] - omega_vector[i]) / 2
                )
    return delta_omega


class Airy(waves.AbstractWaveModel):
    """Linear irregular waves in infinite depth.

    Using JONSWAP spectrum and single propagation direction. No stretching.
    """

    def __init__(self):
        """Constructor."""
        self.psi0 = None
        self.jonswap_parameters = {'sigma_a': 0.07, 'sigma_b': 0.09}
        self.directional_spectrum = {}
        self.grav = 9.81
        self.rho = 1026

    def set_parameters(self, parameters: str):
        """Initialize the wave model with YAML parameters.

        Parameters
        ----------
        parameters : string
            YAML string containing the parameters of this model. The YAML
            should have the following form:
                waves propagating to: 90
                Hs: 5
                Tp: 15
                gamma: 1.2
                omega: [1,2,3]
            `waves propagating to`: direction, in degrees,  the waves are
                                    propagating to. 0 for waves coming from the
                                    South and propagating to the North, 90 deg
                                    for waves coming from the West and
                                    propagating to the East.
            `Hs`: significant wave height in meters, used in the JONSWAP
                  spectrum.
            `Tp`: peak wave period, in seconds, used in the JONSWAP spectrum.
            `gamma`: JONSWAP shape parameter
            `omegas`: discretization of the angular frequency. In rad/s.
            `psis`: discretization of the incidence. In rad.


        Returns
        -------
        Nothing

        """
        param = yaml.safe_load(parameters)
        self.jonswap_parameters['t_p'] = param['Tp']
        self.jonswap_parameters['gamma'] = param['gamma']
        self.directional_spectrum['omega'] = param['omega']
        self.directional_spectrum['psi'] = \
            [param['waves propagating to']*math.pi/180]
        self.jonswap_parameters['hs_square'] = param['Hs']*param['Hs']
        self.jonswap_parameters['omega0'] = 2*math.pi/param['Tp']
        self.jonswap_parameters['coeff'] = 1-0.287*math.log(param['gamma'])
        self.directional_spectrum['si'] = [self.jonswap(omega) for omega in
                                           param['omega']]
        self.directional_spectrum['dj'] = [1]
        self.directional_spectrum['k'] = [omega*omega/self.grav for omega in
                                          param['omega']]
        nb_directions = 1
        phases = np.random.uniform(low=0,
                                   high=2*math.pi,
                                   size=(len(param['omega']), nb_directions))
        self.directional_spectrum['phase'] = phases.tolist()
        # Amplitude evaluation
        delta_omega = create_vector_delta_omega(param['omega'])
        self.directional_spectrum['ampl'] = \
            [math.sqrt(2 * domega_i * s_i)
             for s_i, domega_i in zip(self.directional_spectrum['si'], delta_omega)]


    def jonswap(self, omega: float) -> float:
        r"""Joint North Sea Project spectrum.

        ```math
        S(\omega)=(1-0.287 \log(\gamma))\frac{5}{16}\frac{\alpha}{\omega}H_S^2
        e^{-1.25\left(\frac{\omega_0}{\omega}\right)^4}\gamma^r
        ```

        with

        ```math
        r=e^{-0.5\left(\frac{\omega-\omega_0}{\sigma\omega_0}\right)^2}
        ```

        and

        ```math
        \sigma=\left\{\begin{array}{l}0.07,\omega\leq\omega_0\\0.09,
        \omega>\omega_0\end{array}\right.
        ```
        """
        sigma_a = self.jonswap_parameters['sigma_a']
        sigma_b = self.jonswap_parameters['sigma_b']
        omega0 = self.jonswap_parameters['omega0']
        hs_square = self.jonswap_parameters['hs_square']
        coeff = self.jonswap_parameters['coeff']
        gamma = self.jonswap_parameters['gamma']
        sigma = sigma_a if omega <= omega0 else sigma_b
        ratio = omega0/omega
        alpha = ratio*ratio*ratio*ratio
        awm_5 = coeff*5.0/16.0*alpha/omega*hs_square
        bwm_4 = 1.25*alpha
        kappa = (omega-omega0)/(sigma*omega0)
        return awm_5*math.exp(-bwm_4)*math.pow(gamma,
                                               math.exp(-0.5*kappa*kappa))

    def elevation(self, x: float, y: float, t: float) -> float:
        """Calculate the elevations of the free surface at any point in time.

        Parameters
        ----------
        x : float
            Position (in meters) at which we want the elevation. Projected on
            the X-axis of the Earth-centered, Earth-fixed North-East-Down
            reference frame.
        y : float
            Position (in meters) at which we want the elevation. Projected on
            the Y-axis of the Earth-centered, Earth-fixed North-East-Down
            reference frame.
        t : float
            Simulation time (in seconds). All values are calculated at that
            instant. The documentation of each waves model should describe how
            the phases are defined.

        Returns
        -------
        float
            Free surface height along the Z-axis (oriented downwards) in
            meters.

        """
        zeta = 0
        dir_spec = self.directional_spectrum
        psi = dir_spec['psi'][0]
        for a_i, k, omega, phase in zip(dir_spec['ampl'],
                                        dir_spec['k'],
                                        dir_spec['omega'],
                                        dir_spec['phase']):
            k_x_cos_psi_y_sin_psi = k * (x * math.cos(psi) + y * math.sin(psi))
            # The first element of each list of dir_spec['phase'] is associated
            # to the first and only propagation direction. We use this format to
            # match the gRPC phase format.
            zeta -= a_i * math.sin(-omega*t + k_x_cos_psi_y_sin_psi + phase[0])
        return zeta

    def dynamic_pressure(self, x: float, y: float, z: float, t: float) -> float:
        """Calculate the dynamic pressure due to waves.

        Parameters
        ----------
        x : float
            Position (in meters) at which we want the elevation. Projected on
            the X-axis of the Earth-centered, Earth-fixed North-East-Down
            reference frame.
        y : float
            Position (in meters) at which we want the elevation. Projected on
            the Y-axis of the Earth-centered, Earth-fixed North-East-Down
            reference frame.
        z : float
            Position (in meters) at which we want the elevation. Projected on
            the Z-axis of the Earth-centered, Earth-fixed North-East-Down
            reference frame.
        t : float
            Simulation time (in seconds). All values are calculated at that
            instant. The documentation of each waves model should describe how
            the phases are defined.

        Returns
        -------
        float
            Dynamic pressure (in Pascal) at (x,y,z,t).

        """
        dir_spec = self.directional_spectrum
        eta = self.elevation(x, y, t)
        acc = 0
        psi = dir_spec['psi'][0]
        for a_i, k, omega, phase in zip(dir_spec['ampl'],
                                        dir_spec['k'],
                                        dir_spec['omega'],
                                        dir_spec['phase']):
            k_x_cos_psi_y_sin_psi = k * (x * math.cos(psi)
                                         + y * math.sin(psi))
            acc -= a_i * pdyn_factor(k, z, eta) * math.sin(-omega * t
                                                           + k_x_cos_psi_y_sin_psi
                                                           + phase[0])
        return self.rho * self.grav * acc

    def orbital_velocity(self, x: float, y: float, z: float, t: float) -> Dict[str, float]:
        """Calculate the orbital velocity of the wave particles.

        Parameters
        ----------
        x : float
            Position (in meters) at which we want the elevation. Projected on
            the X-axis of the Earth-centered, Earth-fixed North-East-Down
            reference frame.
        y : float
            Position (in meters) at which we want the elevation. Projected on
            the Y-axis of the Earth-centered, Earth-fixed North-East-Down
            reference frame.
        z : float
            Position (in meters) at which we want the elevation. Projected on
            the Z-axis of the Earth-centered, Earth-fixed North-East-Down
            reference frame.
        t : float
            Simulation time (in seconds). All values are calculated at that
            instant. The documentation of each waves model should describe how
            the phases are defined.

        Returns
        -------
        dict
            Should contain the following fields:
            - vx (float): Projection on the X-axis of the Earth-centered,
                          Earth-fixed North-East-Down reference frame of the
                          velocity of each wave partical relative to the
                          ground. In meters per second.
            - vy (float): Projection on the Y-axis of the Earth-centered,
                          Earth-fixed North-East-Down reference frame of the
                          velocity of each wave partical relative to the
                          ground. In meters per second.
            - vz (float): Projection on the Z-axis of the Earth-centered,
                          Earth-fixed North-East-Down reference frame of the
                          velocity of each wave partical relative to the
                          ground. In meters per second.

        """
        dir_spec = self.directional_spectrum
        eta = self.elevation(x, y, t)
        v_x = 0
        v_y = 0
        v_z = 0
        psi = dir_spec['psi'][0]
        for a_i, k, omega, phase in zip(dir_spec['ampl'],
                                        dir_spec['k'],
                                        dir_spec['omega'],
                                        dir_spec['phase']):
            pdyn_factor_sh = pdyn_factor(k, z, eta)
            k_x_cos_psi_y_sin_psi = k * (x * math.cos(psi)
                                         + y * math.sin(psi))
            theta = -omega * t + k_x_cos_psi_y_sin_psi + phase[0]
            cos_theta = math.cos(theta)
            sin_theta = math.sin(theta)
            a_k_omega = a_i * k / omega
            a_k_omega_pdyn_factor_sin_theta = a_k_omega * pdyn_factor_sh \
                * sin_theta
            v_x += a_k_omega_pdyn_factor_sin_theta * math.cos(psi)
            v_y += a_k_omega_pdyn_factor_sin_theta * math.sin(psi)
            v_z += a_k_omega * pdyn_factor_sh * cos_theta

        v_x *= self.grav
        v_y *= self.grav
        v_z *= self.grav
        return {'vx': v_x, 'vy': v_y, 'vz': v_z}

    def angular_frequencies_for_rao(self) -> List[List[float]]:
        """Get angular frequencies the wave spectrum is discretized at.

        Returns
        -------
        list of list of floats
            Angular frequencies the spectrum was discretized at (in rad/s).
            Used, for example, when interpolating the wave RAOs.

        """
        return [self.directional_spectrum['omega']]

    def directions_for_rao(self) -> List[List[float]]:
        """Get the incidences the directional spreading is discretized at
        in the North-East-Down reference frame.

        Returns
        -------
        list of list of floats
          Wave incidences the spectrum was discretized at (in rad).
          0° is for waves coming from the South and propagating to the North.
          90° is for waves coming from the West and propagating to the East.
          Used, for example, when interpolating the wave RAOs.

        """
        return [self.directional_spectrum['psi']]

    def spectrum(self, _x: float, _y: float, _t: float) -> List[Dict[str, Any]]:
        """Linear spectrum that can be used by xdyn's diffraction module.

        Parameters
        ----------
        x : float
            Position (in meters) at which we want the (linearized) spectrum.
            Projected on the X-axis of the Earth-centered, Earth-fixed
            North-East-Down reference frame.
        y : float
            Position (in meters) at which we want the (linearized) spectrum.
            Projected on the Y-axis of the Earth-centered, Earth-fixed
            North-East-Down reference frame.
        t : float
            Simulation time (in seconds). Date at which we want the
            (linearized) spectrum.

        Returns
        -------
        list of dict
            Each dict should contain the following fields:
            - si (list of floats): Discretized spectral density for each
              omega (should therefore be the same size as omega).
              In s m^2/rad.
            - dj (list of floats): Spatial spreading for each psi (should
              therefore be the same size as psi). In 1/rad.
            - omega (list of floats): Angular frequencies the spectrum was
              discretized at. In rad/s.
            - psi (list of floats): Directions between 0 & 2pi the spatial
              spreading was discretized at. In rad.
            - k (list of floats): Discretized wave number for each
              frequency (should therefore be the same size as omega).
              In rad/m.
            - phase (list of lists): Random phases, for each
              (frequency, direction) couple (but time invariant), should
              have the same size as omega, each sublist should have the
              same size as psi.
              Each element should have the same size as omega. In radian.

        """
        keys = ("si", "dj", "omega", "psi", "k", "phase")
        d = {'si': self.directional_spectrum['si'],
             'dj': self.directional_spectrum['dj'],
             'omega': self.directional_spectrum['omega'],
             'psi': self.directional_spectrum['psi'],
             'k': self.directional_spectrum['k'],
             'phase': self.directional_spectrum['phase']}
        return [d,d]


if __name__ == '__main__':
    LOGGER.info('Starting Airy waves server...')
    waves.serve(Airy())
    LOGGER.info('Stopped Airy waves server')
