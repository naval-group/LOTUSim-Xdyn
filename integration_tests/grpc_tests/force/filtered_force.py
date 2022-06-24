"""Force model using filtered states."""

import numpy as np
import force
import logging
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

    def __init__(self, _, body_name, __):
        """Initialize parameters from gRPC's set_parameters."""
        self.body_name = body_name

    def get_parameters(self):
        """Just using defaults."""
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
