"""Force model using data from potential theory."""

import yaml
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



class HDBForceModel(force.Model):
    """Outputs data from HDB in extra_observations."""

    def __init__(self, _, body_name, pot):
        """Initialize parameters from gRPC's set_parameters."""
        self.body_name = body_name
        self.pot = pot

    def get_parameters(self):
        """Parameter k is stiffness and c is damping."""
        return {'max_history_length': 0, 'needs_wave_outputs': False,
                'frame': self.body_name, 'x': 0, 'y': 0, 'z': 0, 'phi': 0,
                'theta': 0, 'psi': 0, 'required_commands': []}

    def force(self, states, _, __):
        """Force model."""
        extra_observations = {}
        extra_observations['Ma(0,0)'] = self.pot.Ma[0][0]
        extra_observations['Ma(0,2)'] = self.pot.Ma[0][2]
        extra_observations['Ma(0,4)'] = self.pot.Ma[0][4]
        extra_observations['Ma(1,1)'] = self.pot.Ma[1][1]
        extra_observations['Ma(1,3)'] = self.pot.Ma[1][3]
        extra_observations['Ma(1,5)'] = self.pot.Ma[1][5]
        extra_observations['Ma(2,0)'] = self.pot.Ma[2][0]
        extra_observations['Ma(2,2)'] = self.pot.Ma[2][2]
        extra_observations['Ma(2,4)'] = self.pot.Ma[2][4]
        extra_observations['Ma(3,1)'] = self.pot.Ma[3][1]
        extra_observations['Ma(3,3)'] = self.pot.Ma[3][3]
        extra_observations['Ma(3,5)'] = self.pot.Ma[3][5]
        extra_observations['Ma(4,0)'] = self.pot.Ma[4][0]
        extra_observations['Ma(4,2)'] = self.pot.Ma[4][2]
        extra_observations['Ma(4,4)'] = self.pot.Ma[4][4]
        extra_observations['Ma(5,1)'] = self.pot.Ma[5][1]
        extra_observations['Ma(5,3)'] = self.pot.Ma[5][3]
        extra_observations['Ma(5,5)'] = self.pot.Ma[5][5]
        return {'Fx': 0,
                'Fy': 0,
                'Fz': 0,
                'Mx': 0,
                'My': 0,
                'Mz': 0,
                'extra_observations': extra_observations }


if __name__ == '__main__':
    force.serve(HDBForceModel)
