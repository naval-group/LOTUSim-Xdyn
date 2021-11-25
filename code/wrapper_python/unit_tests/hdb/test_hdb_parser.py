"""
Unit test for HDBParser
"""
import unittest

import numpy as np
from xdyn.data.test import big_hdb, bug_3238_hdb, hdb_issue_184, test_ship_hdb
from xdyn.exceptions import InvalidInputException
from xdyn.hdb import HDBParser


class HDBParserTest(unittest.TestCase):
    """Test class for HDBParser"""

    def test_can_get_added_mass(self):
        builder = HDBParser.from_string(test_ship_hdb())
        self.assertEqual(type(builder), HDBParser)
        Ma = builder.get_added_mass_array()
        self.assertEqual(6, len(Ma))
        self.assertEqual(1, Ma[0][0])
        self.assertEqual(6, len(Ma[0][1]))
        self.assertEqual(1.097184e04, Ma[0][1][0][0])
        self.assertEqual(-4.443533e01, Ma[0][1][3][4])

        self.assertEqual(2, Ma[1][0])
        self.assertEqual(6, len(Ma[1][1]))
        self.assertEqual(-2.102286e04, Ma[1][1][1][3])
        self.assertEqual(2.702315e05, Ma[1][1][2][4])

        self.assertEqual(3, Ma[2][0])
        self.assertEqual(6, len(Ma[2][1]))
        self.assertEqual(-7.183531e01, Ma[2][1][5][0])
        self.assertEqual(-8.938050e01, Ma[2][1][2][1])

        self.assertEqual(3.5, Ma[3][0])
        self.assertEqual(6, len(Ma[3][1]))

        self.assertEqual(3.8, Ma[4][0])
        self.assertEqual(6, len(Ma[4][1]))

        self.assertEqual(4, Ma[5][0])
        self.assertEqual(6, len(Ma[5][1]))

    def test_can_retrieve_radiation_damping(self):
        builder = HDBParser.from_string(test_ship_hdb())
        self.assertEqual(type(builder), HDBParser)
        Br = builder.get_radiation_damping_array()
        self.assertEqual(6, len(Br))
        self.assertEqual(1, Br[0][0])
        self.assertEqual(2, Br[1][0])
        self.assertEqual(3, Br[2][0])
        self.assertEqual(3.5, Br[3][0])
        self.assertEqual(3.8, Br[4][0])
        self.assertEqual(4, Br[5][0])

        self.assertEqual(6.771553e03, Br[0][1][0][0])
        self.assertEqual(2.194728e05, Br[1][1][1][1])
        self.assertEqual(1.488785e05, Br[2][1][2][2])
        self.assertEqual(8.694864e04, Br[3][1][3][3])
        self.assertEqual(5.476829e06, Br[4][1][4][4])
        self.assertEqual(4.374308e06, Br[5][1][5][5])

    def test_can_retrieve_initial_values(self):
        data = HDBParser.from_string(test_ship_hdb())
        M1 = data.get_added_mass(1)
        self.assertEqual(6, M1.shape[0])
        self.assertEqual(6, M1.shape[1])
        self.assertEqual(-1.503212e04, M1[2, 0])
        self.assertEqual(-1.618101e02, M1[2, 1])
        self.assertEqual(+1.781351e05, M1[2, 2])
        self.assertEqual(+1.053934e02, M1[2, 3])
        self.assertEqual(+3.267841e05, M1[2, 4])
        self.assertEqual(+6.774041e02, M1[2, 5])

        M2 = data.get_added_mass(2)
        self.assertEqual(6, M2.shape[0])
        self.assertEqual(6, M2.shape[1])
        self.assertEqual(+3.032333e01, M2[3, 0])
        self.assertEqual(-2.332197e04, M2[3, 1])
        self.assertEqual(+5.924372e01, M2[3, 2])
        self.assertEqual(+2.132689e05, M2[3, 3])
        self.assertEqual(-2.994200e02, M2[3, 4])
        self.assertEqual(-3.943479e04, M2[3, 5])
        #! [HDBDataTest expected output]

    def test_can_retrieve_added_mass_at_Tp_0(self):
        EPS = 1e-12
        data = HDBParser.from_string(test_ship_hdb())
        M = data.get_added_mass()
        self.assertEqual(6, M.shape[0])
        self.assertEqual(6, M.shape[1])
        self.assertAlmostEqual(-1.503212e04, M[2, 0], delta=EPS)
        self.assertAlmostEqual(-1.618101e02, M[2, 1], delta=EPS)
        self.assertAlmostEqual(+1.781351e05, M[2, 2], delta=EPS)
        self.assertAlmostEqual(+1.053934e02, M[2, 3], delta=EPS)
        self.assertAlmostEqual(+3.267841e05, M[2, 4], delta=EPS)
        self.assertAlmostEqual(+6.774041e02, M[2, 5], delta=EPS)

    def test_can_retrieve_angular_frequencies_for_radiation_damping(self):
        data = HDBParser.from_string(test_ship_hdb())
        angular_frequencies = data.get_angular_frequencies()
        self.assertEqual(6, len(angular_frequencies))
        self.assertEqual(2 * np.pi / 1.0, angular_frequencies[5])
        self.assertEqual(2 * np.pi / 2.0, angular_frequencies[4])
        self.assertEqual(2 * np.pi / 3.0, angular_frequencies[3])
        self.assertEqual(2 * np.pi / 3.5, angular_frequencies[2])
        self.assertEqual(2 * np.pi / 3.8, angular_frequencies[1])
        self.assertEqual(2 * np.pi / 4.0, angular_frequencies[0])

    def test_can_retrieve_vectors_for_each_element_in_radiation_damping_matrix(self):
        data = HDBParser.from_string(test_ship_hdb())
        for i in range(6):
            for j in range(6):
                self.assertEqual(6, len(data.get_radiation_damping_coeff(i, j)))
        v = data.get_radiation_damping_coeff(1, 2)
        self.assertEqual(-1.590935e02, v[5])
        self.assertEqual(2.595528e02, v[4])
        self.assertEqual(-1.614637e02, v[3])
        self.assertEqual(-1.376756e02, v[2])
        self.assertEqual(-1.215545e02, v[1])
        self.assertEqual(-1.083372e02, v[0])

    def test_can_retrieve_vector_of_vectors_for_RAOs(self):
        data = HDBParser.from_string(test_ship_hdb())
        module = data.get_total_excitation_force_module_tables()
        phase = data.get_total_excitation_force_phase_tables()
        self.assertEqual(6, len(module[0]))
        self.assertEqual(6, len(module[1]))
        self.assertEqual(6, len(module[2]))
        self.assertEqual(6, len(module[3]))
        self.assertEqual(6, len(module[4]))
        self.assertEqual(6, len(module[5]))
        self.assertEqual(6, len(phase[0]))
        self.assertEqual(6, len(phase[1]))
        self.assertEqual(6, len(phase[2]))
        self.assertEqual(6, len(phase[3]))
        self.assertEqual(6, len(phase[4]))
        self.assertEqual(6, len(phase[5]))
        for i in range(6):
            for j in range(6):
                self.assertEqual(13, len(module[i][j]))
                self.assertEqual(13, len(phase[i][j]))
        self.assertEqual(3.098978e5, module[2][4][3])
        self.assertEqual(7.774210e4, module[1][3][2])
        self.assertEqual(1.459181e4, module[5][2][6])
        self.assertEqual(-2.004334, phase[2][4][3])
        self.assertEqual(3.041773, phase[1][3][2])
        self.assertEqual(8.036613e-3, phase[5][2][6])

    def test_can_retrieve_omegas_for_RAOs(self):
        data = HDBParser.from_string(test_ship_hdb())
        Tps1 = data.get_total_excitation_force_phase_periods()
        Tps2 = data.get_total_excitation_force_module_periods()
        self.assertEqual(6, len(Tps1))
        self.assertEqual(6, len(Tps2))
        self.assertEqual(1.0, Tps1[0])
        self.assertEqual(2.0, Tps1[1])
        self.assertEqual(3.0, Tps1[2])
        self.assertEqual(3.5, Tps1[3])
        self.assertEqual(3.8, Tps1[4])
        self.assertEqual(4.0, Tps1[5])

        self.assertEqual(1.0, Tps2[0])
        self.assertEqual(2.0, Tps2[1])
        self.assertEqual(3.0, Tps2[2])
        self.assertEqual(3.5, Tps2[3])
        self.assertEqual(3.8, Tps2[4])
        self.assertEqual(4.0, Tps2[5])

    def test_can_retrieve_psis_for_RAOs(self):
        data = HDBParser.from_string(test_ship_hdb())
        psi1 = data.get_total_excitation_force_phase_psis()
        psi2 = data.get_total_excitation_force_module_psis()
        self.assertEqual(13, len(psi1))
        self.assertEqual(13, len(psi2))
        for i in range(13):
            self.assertAlmostEqual(np.pi / 180.0 * 15.0 * i, psi1[i], delta=1e-12)
            self.assertAlmostEqual(np.pi / 180.0 * 15.0 * i, psi2[i], delta=1e-12)

    def test_bug_3238(self):
        data = HDBParser.from_string(bug_3238_hdb())
        expected_msg = (
            "Unable to find section 'DIFFRACTION_FORCES_AND_MOMENTS' in the RAO file."
        )
        with self.assertRaises(InvalidInputException) as pcm:
            data.get_diffraction_module()
        self.assertTrue(expected_msg in str(pcm.exception), str(pcm.exception))
        with self.assertRaises(InvalidInputException) as pcm:
            data.get_diffraction_phase()
        self.assertTrue(expected_msg in str(pcm.exception), str(pcm.exception))

    def test_can_get_forward_speed(self):
        data = HDBParser.from_string(test_ship_hdb())
        self.assertEqual(data.get_forward_speed(), 0.0)

    def test_should_be_able_to_have_different_periods_for_added_mass_and_List_calculated_periods(
        self,
    ):
        HDBParser.from_string(hdb_issue_184())
        HDBParser.from_string(hdb_issue_184())

    def test_can_retrieve_froude_krylov_modules_and_phases(self):
        data = HDBParser.from_string(big_hdb())
        module = data.get_froude_krylov_module_tables()
        phase = data.get_froude_krylov_phase_tables()
        self.assertEqual(47, len(module[0]))
        self.assertEqual(47, len(module[1]))
        self.assertEqual(47, len(module[2]))
        self.assertEqual(47, len(module[3]))
        self.assertEqual(47, len(module[4]))
        self.assertEqual(47, len(module[5]))
        self.assertEqual(47, len(phase[0]))
        self.assertEqual(47, len(phase[1]))
        self.assertEqual(47, len(phase[2]))
        self.assertEqual(47, len(phase[3]))
        self.assertEqual(47, len(phase[4]))
        self.assertEqual(47, len(phase[5]))
        for i in range(6):
            for j in range(6):
                self.assertEqual(13, len(module[i][j]))
                self.assertEqual(13, len(phase[i][j]))
        # column (axis), pulsation, incidence
        self.assertEqual(4.143975e05, module[2][4][3])
        self.assertEqual(-1.987963e00, phase[1][3][2])


if __name__ == "__main__":

    unittest.main()
