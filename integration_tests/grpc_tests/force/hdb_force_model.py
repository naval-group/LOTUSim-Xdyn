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
        extra_observations['diffX(0,0)'] = self.pot.diffraction_module_tables.X[0][0]
        extra_observations['diffX(0,1)'] = self.pot.diffraction_module_tables.X[0][1]
        extra_observations['diffX(1,0)'] = self.pot.diffraction_module_tables.X[1][0]
        extra_observations['diffX(1,1)'] = self.pot.diffraction_module_tables.X[1][1]
        extra_observations['diffX(2,0)'] = self.pot.diffraction_module_tables.X[2][0]
        extra_observations['diffX(2,1)'] = self.pot.diffraction_module_tables.X[2][1]
        extra_observations['diffY(0,0)'] = self.pot.diffraction_module_tables.Y[0][0]
        extra_observations['diffY(0,1)'] = self.pot.diffraction_module_tables.Y[0][1]
        extra_observations['diffY(1,0)'] = self.pot.diffraction_module_tables.Y[1][0]
        extra_observations['diffY(1,1)'] = self.pot.diffraction_module_tables.Y[1][1]
        extra_observations['diffY(2,0)'] = self.pot.diffraction_module_tables.Y[2][0]
        extra_observations['diffY(2,1)'] = self.pot.diffraction_module_tables.Y[2][1]
        extra_observations['diffZ(0,0)'] = self.pot.diffraction_module_tables.Z[0][0]
        extra_observations['diffZ(0,1)'] = self.pot.diffraction_module_tables.Z[0][1]
        extra_observations['diffZ(1,0)'] = self.pot.diffraction_module_tables.Z[1][0]
        extra_observations['diffZ(1,1)'] = self.pot.diffraction_module_tables.Z[1][1]
        extra_observations['diffZ(2,0)'] = self.pot.diffraction_module_tables.Z[2][0]
        extra_observations['diffZ(2,1)'] = self.pot.diffraction_module_tables.Z[2][1]
        extra_observations['diffK(0,0)'] = self.pot.diffraction_module_tables.K[0][0]
        extra_observations['diffK(0,1)'] = self.pot.diffraction_module_tables.K[0][1]
        extra_observations['diffK(1,0)'] = self.pot.diffraction_module_tables.K[1][0]
        extra_observations['diffK(1,1)'] = self.pot.diffraction_module_tables.K[1][1]
        extra_observations['diffK(2,0)'] = self.pot.diffraction_module_tables.K[2][0]
        extra_observations['diffK(2,1)'] = self.pot.diffraction_module_tables.K[2][1]
        extra_observations['diffM(0,0)'] = self.pot.diffraction_module_tables.M[0][0]
        extra_observations['diffM(0,1)'] = self.pot.diffraction_module_tables.M[0][1]
        extra_observations['diffM(1,0)'] = self.pot.diffraction_module_tables.M[1][0]
        extra_observations['diffM(1,1)'] = self.pot.diffraction_module_tables.M[1][1]
        extra_observations['diffM(2,0)'] = self.pot.diffraction_module_tables.M[2][0]
        extra_observations['diffM(2,1)'] = self.pot.diffraction_module_tables.M[2][1]
        extra_observations['diffN(0,0)'] = self.pot.diffraction_module_tables.N[0][0]
        extra_observations['diffN(0,1)'] = self.pot.diffraction_module_tables.N[0][1]
        extra_observations['diffN(1,0)'] = self.pot.diffraction_module_tables.N[1][0]
        extra_observations['diffN(1,1)'] = self.pot.diffraction_module_tables.N[1][1]
        extra_observations['diffN(2,0)'] = self.pot.diffraction_module_tables.N[2][0]
        extra_observations['diffN(2,1)'] = self.pot.diffraction_module_tables.N[2][1]

        extra_observations['diffpX(0,0)'] = self.pot.diffraction_phase_tables.X[0][0]
        extra_observations['diffpX(0,1)'] = self.pot.diffraction_phase_tables.X[0][1]
        extra_observations['diffpX(1,0)'] = self.pot.diffraction_phase_tables.X[1][0]
        extra_observations['diffpX(1,1)'] = self.pot.diffraction_phase_tables.X[1][1]
        extra_observations['diffpX(2,0)'] = self.pot.diffraction_phase_tables.X[2][0]
        extra_observations['diffpX(2,1)'] = self.pot.diffraction_phase_tables.X[2][1]
        extra_observations['diffpY(0,0)'] = self.pot.diffraction_phase_tables.Y[0][0]
        extra_observations['diffpY(0,1)'] = self.pot.diffraction_phase_tables.Y[0][1]
        extra_observations['diffpY(1,0)'] = self.pot.diffraction_phase_tables.Y[1][0]
        extra_observations['diffpY(1,1)'] = self.pot.diffraction_phase_tables.Y[1][1]
        extra_observations['diffpY(2,0)'] = self.pot.diffraction_phase_tables.Y[2][0]
        extra_observations['diffpY(2,1)'] = self.pot.diffraction_phase_tables.Y[2][1]
        extra_observations['diffpZ(0,0)'] = self.pot.diffraction_phase_tables.Z[0][0]
        extra_observations['diffpZ(0,1)'] = self.pot.diffraction_phase_tables.Z[0][1]
        extra_observations['diffpZ(1,0)'] = self.pot.diffraction_phase_tables.Z[1][0]
        extra_observations['diffpZ(1,1)'] = self.pot.diffraction_phase_tables.Z[1][1]
        extra_observations['diffpZ(2,0)'] = self.pot.diffraction_phase_tables.Z[2][0]
        extra_observations['diffpZ(2,1)'] = self.pot.diffraction_phase_tables.Z[2][1]
        extra_observations['diffpK(0,0)'] = self.pot.diffraction_phase_tables.K[0][0]
        extra_observations['diffpK(0,1)'] = self.pot.diffraction_phase_tables.K[0][1]
        extra_observations['diffpK(1,0)'] = self.pot.diffraction_phase_tables.K[1][0]
        extra_observations['diffpK(1,1)'] = self.pot.diffraction_phase_tables.K[1][1]
        extra_observations['diffpK(2,0)'] = self.pot.diffraction_phase_tables.K[2][0]
        extra_observations['diffpK(2,1)'] = self.pot.diffraction_phase_tables.K[2][1]
        extra_observations['diffpM(0,0)'] = self.pot.diffraction_phase_tables.M[0][0]
        extra_observations['diffpM(0,1)'] = self.pot.diffraction_phase_tables.M[0][1]
        extra_observations['diffpM(1,0)'] = self.pot.diffraction_phase_tables.M[1][0]
        extra_observations['diffpM(1,1)'] = self.pot.diffraction_phase_tables.M[1][1]
        extra_observations['diffpM(2,0)'] = self.pot.diffraction_phase_tables.M[2][0]
        extra_observations['diffpM(2,1)'] = self.pot.diffraction_phase_tables.M[2][1]
        extra_observations['diffpN(0,0)'] = self.pot.diffraction_phase_tables.N[0][0]
        extra_observations['diffpN(0,1)'] = self.pot.diffraction_phase_tables.N[0][1]
        extra_observations['diffpN(1,0)'] = self.pot.diffraction_phase_tables.N[1][0]
        extra_observations['diffpN(1,1)'] = self.pot.diffraction_phase_tables.N[1][1]
        extra_observations['diffpN(2,0)'] = self.pot.diffraction_phase_tables.N[2][0]
        extra_observations['diffpN(2,1)'] = self.pot.diffraction_phase_tables.N[2][1]

        extra_observations['diffT0'] = self.pot.diffraction_module_periods[0]
        extra_observations['diffT1'] = self.pot.diffraction_module_periods[1]
        extra_observations['diffT2'] = self.pot.diffraction_module_periods[2]

        extra_observations['diffTp0'] = self.pot.diffraction_phase_periods[0]
        extra_observations['diffTp1'] = self.pot.diffraction_phase_periods[1]
        extra_observations['diffTp2'] = self.pot.diffraction_phase_periods[2]

        extra_observations['diffpsi0'] = self.pot.diffraction_module_psis[0]
        extra_observations['diffpsi1'] = self.pot.diffraction_module_psis[1]

        extra_observations['diffppsi0'] = self.pot.diffraction_phase_psis[0]
        extra_observations['diffppsi1'] = self.pot.diffraction_phase_psis[1]

        extra_observations['FKFMX(0,0)'] = self.pot.froude_krylov_module_tables.X[0][0]
        extra_observations['FKFMX(0,1)'] = self.pot.froude_krylov_module_tables.X[0][1]
        extra_observations['FKFMX(1,0)'] = self.pot.froude_krylov_module_tables.X[1][0]
        extra_observations['FKFMX(1,1)'] = self.pot.froude_krylov_module_tables.X[1][1]
        extra_observations['FKFMX(2,0)'] = self.pot.froude_krylov_module_tables.X[2][0]
        extra_observations['FKFMX(2,1)'] = self.pot.froude_krylov_module_tables.X[2][1]
        extra_observations['FKFMY(0,0)'] = self.pot.froude_krylov_module_tables.Y[0][0]
        extra_observations['FKFMY(0,1)'] = self.pot.froude_krylov_module_tables.Y[0][1]
        extra_observations['FKFMY(1,0)'] = self.pot.froude_krylov_module_tables.Y[1][0]
        extra_observations['FKFMY(1,1)'] = self.pot.froude_krylov_module_tables.Y[1][1]
        extra_observations['FKFMY(2,0)'] = self.pot.froude_krylov_module_tables.Y[2][0]
        extra_observations['FKFMY(2,1)'] = self.pot.froude_krylov_module_tables.Y[2][1]
        extra_observations['FKFMZ(0,0)'] = self.pot.froude_krylov_module_tables.Z[0][0]
        extra_observations['FKFMZ(0,1)'] = self.pot.froude_krylov_module_tables.Z[0][1]
        extra_observations['FKFMZ(1,0)'] = self.pot.froude_krylov_module_tables.Z[1][0]
        extra_observations['FKFMZ(1,1)'] = self.pot.froude_krylov_module_tables.Z[1][1]
        extra_observations['FKFMZ(2,0)'] = self.pot.froude_krylov_module_tables.Z[2][0]
        extra_observations['FKFMZ(2,1)'] = self.pot.froude_krylov_module_tables.Z[2][1]
        extra_observations['FKFMK(0,0)'] = self.pot.froude_krylov_module_tables.K[0][0]
        extra_observations['FKFMK(0,1)'] = self.pot.froude_krylov_module_tables.K[0][1]
        extra_observations['FKFMK(1,0)'] = self.pot.froude_krylov_module_tables.K[1][0]
        extra_observations['FKFMK(1,1)'] = self.pot.froude_krylov_module_tables.K[1][1]
        extra_observations['FKFMK(2,0)'] = self.pot.froude_krylov_module_tables.K[2][0]
        extra_observations['FKFMK(2,1)'] = self.pot.froude_krylov_module_tables.K[2][1]
        extra_observations['FKFMM(0,0)'] = self.pot.froude_krylov_module_tables.M[0][0]
        extra_observations['FKFMM(0,1)'] = self.pot.froude_krylov_module_tables.M[0][1]
        extra_observations['FKFMM(1,0)'] = self.pot.froude_krylov_module_tables.M[1][0]
        extra_observations['FKFMM(1,1)'] = self.pot.froude_krylov_module_tables.M[1][1]
        extra_observations['FKFMM(2,0)'] = self.pot.froude_krylov_module_tables.M[2][0]
        extra_observations['FKFMM(2,1)'] = self.pot.froude_krylov_module_tables.M[2][1]
        extra_observations['FKFMN(0,0)'] = self.pot.froude_krylov_module_tables.N[0][0]
        extra_observations['FKFMN(0,1)'] = self.pot.froude_krylov_module_tables.N[0][1]
        extra_observations['FKFMN(1,0)'] = self.pot.froude_krylov_module_tables.N[1][0]
        extra_observations['FKFMN(1,1)'] = self.pot.froude_krylov_module_tables.N[1][1]
        extra_observations['FKFMN(2,0)'] = self.pot.froude_krylov_module_tables.N[2][0]
        extra_observations['FKFMN(2,1)'] = self.pot.froude_krylov_module_tables.N[2][1]

        extra_observations['FKFMpX(0,0)'] = self.pot.froude_krylov_phase_tables.X[0][0]
        extra_observations['FKFMpY(0,0)'] = self.pot.froude_krylov_phase_tables.X[0][0]
        extra_observations['FKFMpZ(0,0)'] = self.pot.froude_krylov_phase_tables.X[0][0]
        extra_observations['FKFMpK(0,0)'] = self.pot.froude_krylov_phase_tables.X[0][0]
        extra_observations['FKFMpM(0,0)'] = self.pot.froude_krylov_phase_tables.X[0][0]
        extra_observations['FKFMpN(0,0)'] = self.pot.froude_krylov_phase_tables.X[0][0]

        extra_observations['FKFMpX(1,0)'] = self.pot.froude_krylov_phase_tables.X[1][0]
        extra_observations['FKFMpY(1,0)'] = self.pot.froude_krylov_phase_tables.X[1][0]
        extra_observations['FKFMpZ(1,0)'] = self.pot.froude_krylov_phase_tables.X[1][0]
        extra_observations['FKFMpK(1,0)'] = self.pot.froude_krylov_phase_tables.X[1][0]
        extra_observations['FKFMpM(1,0)'] = self.pot.froude_krylov_phase_tables.X[1][0]
        extra_observations['FKFMpN(1,0)'] = self.pot.froude_krylov_phase_tables.X[1][0]

        extra_observations['FKT0'] = self.pot.froude_krylov_module_periods[0]
        extra_observations['FKT1'] = self.pot.froude_krylov_module_periods[1]
        extra_observations['FKT2'] = self.pot.froude_krylov_module_periods[2]

        extra_observations['FKTp0'] = self.pot.froude_krylov_phase_periods[0]
        extra_observations['FKTp1'] = self.pot.froude_krylov_phase_periods[1]
        extra_observations['FKTp2'] = self.pot.froude_krylov_phase_periods[2]
        extra_observations['FKTp46'] = self.pot.froude_krylov_phase_periods[46]

        extra_observations['FKpsi0'] = self.pot.froude_krylov_module_psis[0]
        extra_observations['FKpsi1'] = self.pot.froude_krylov_module_psis[1]
        extra_observations['FKpsi2'] = self.pot.froude_krylov_module_psis[2]

        extra_observations['FKppsi0'] = self.pot.froude_krylov_phase_psis[0]
        extra_observations['FKppsi1'] = self.pot.froude_krylov_phase_psis[1]
        extra_observations['FKppsi2'] = self.pot.froude_krylov_phase_psis[2]

        extra_observations['omega0'] = self.pot.angular_frequencies[0]
        extra_observations['omega1'] = self.pot.angular_frequencies[1]
        extra_observations['omega2'] = self.pot.angular_frequencies[2]

        extra_observations['forwardSpeed'] = self.pot.forward_speed

        return {'Fx': 0,
                'Fy': 0,
                'Fz': 0,
                'Mx': 0,
                'My': 0,
                'Mz': 0,
                'extra_observations': extra_observations }


if __name__ == '__main__':
    force.serve(HDBForceModel)
