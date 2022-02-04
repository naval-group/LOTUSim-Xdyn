"""
Unit test for PrecalParser
"""
import os
import unittest

import numpy as np
from xdyn.data.test.precal import (
    added_mass_damping_matrix_inf_freq,
    general,
    precal,
    raos,
    ship_particulars,
)
from xdyn.hdb import (
    PrecalFile,
    PrecalParser,
    parse_precal_from_file,
    parse_precal_from_string,
    parse_rao_attributes,
)


class PrecalParserTest(unittest.TestCase):
    """Test class for PrecalParser"""
    def test_can_parse_empty_section(self):
        data = parse_precal_from_string("[some section]")
        section = data.sections[0]
        self.assertEqual(1, len(data.sections))
        self.assertEqual("some section", section.title)

    def test_can_parse_general_section(self):
        data = parse_precal_from_string(general())
        section = data.sections[0]
        self.assertEqual(1, len(data.sections))
        self.assertEqual("General", section.title)
        self.assertEqual("NOT SPECIFIED", section.string_values["userName"])
        self.assertEqual("NOT SPECIFIED", section.string_values["projectNumber"])
        self.assertEqual("DTMB_5415_without_fins", section.string_values["projectName"])
        self.assertEqual("NOT SPECIFIED", section.string_values["caseName"])
        self.assertEqual("PRECAL - version 18.1.3", section.string_values["sourceName"])
        self.assertEqual("2021-07-13", section.string_values["date"])
        self.assertEqual("12:22:18", section.string_values["time"])
        self.assertEqual("Windows", section.string_values["OS"])
        self.assertEqual(-2, section.scalar_values["numOmpThreads"])
        self.assertEqual(4, section.scalar_values["NrOmpThreads"])
        self.assertEqual("18.1.3", section.string_values["VersionNumber"])
        self.assertEqual("Tue Dec 10 10:07:44 2019", section.string_values["CreationDate"])
        self.assertEqual(1800, section.scalar_values["IfortVersion"])

    def test_can_parse_vectors(self):
        data = parse_precal_from_string(
        """
        [Particulars-ship]
        COB              = {1.519,0.000,0.066}  (ship center of buoyancy w.r.t. aft
        perpendicular - centerline - keel line, calculated from geometry)
        """)
        self.assertEqual(type(data), PrecalFile)
        section = data.sections[0]
        self.assertEqual(1, len(data.sections))
        self.assertEqual("Particulars-ship", section.title)
        self.assertEqual(3, len(section.vector_values["COB"]))
        self.assertEqual(1.519, section.vector_values["COB"][0])
        self.assertEqual(0.000, section.vector_values["COB"][1])
        self.assertEqual(0.066, section.vector_values["COB"][2])

    def test_can_parse_ship_particulars(self):
        data = parse_precal_from_string(ship_particulars())
        section = data.sections[0]
        self.assertEqual(3, len(section.vector_values["COB"]))
        self.assertEqual(70.217, section.vector_values["COB"][0])
        self.assertEqual(0.000, section.vector_values["COB"][1])
        self.assertEqual(3.659, section.vector_values["COB"][2])
        self.assertEqual(6.150, section.scalar_values["T_mean"])
        self.assertEqual(7.510, section.vector_values["COG"][2])

    def test_can_parse_rao_titles(self):
        data_raos = parse_precal_from_string(raos()).raos
        self.assertEqual("Signal 1: surge motion at (70.015,0.000,7.510), h=-1.000m, phi_a=0.500deg, U=0.000kn, mu=180.000deg (amplitude unit = m/m, phase unit = deg)",
            data_raos[0].title_line)
        self.assertEqual("Signal 5: pitch motion at (70.015,0.000,7.510), h=-1.000m, phi_a=0.500deg, U=20.000kn, mu=90.000deg (amplitude unit = deg/m, phase unit = deg)",
            data_raos[29].title_line)

    def test_can_parse_rao_attributes(self):
        rao_attributes = parse_rao_attributes(
            "Signal 1: surge motion at (70.015,0.000,7.510), h=-1.000m, phi_a=0.500deg, "
            "U=12.000kn, mu=180.000deg (amplitude unit = m/m, phase unit = deg)")
        self.assertEqual("surge motion", rao_attributes.name)
        self.assertTrue(np.allclose([70.015, 0.000, 7.510], rao_attributes.position))
        self.assertEqual(-1, rao_attributes.h)
        self.assertEqual("m", rao_attributes.h_unit)
        self.assertEqual(0.5, rao_attributes.phi_a)
        self.assertEqual("deg", rao_attributes.phi_a_unit)
        self.assertEqual(12, rao_attributes.U)
        self.assertEqual("kn", rao_attributes.U_unit)
        self.assertEqual(180, rao_attributes.mu)
        self.assertEqual("deg", rao_attributes.mu_unit)
        self.assertEqual("m/m", rao_attributes.amplitude_unit)
        self.assertEqual("deg", rao_attributes.phase_unit)

    def test_full_rao_parse_test(self):
        data_raos = parse_precal_from_string(raos()).raos
        # Amplitudes and phases - Signal 4: roll motion, U=20.000kn, mu=90.000deg
        self.assertEqual("heave motion", data_raos[15].attributes.name)
        self.assertTrue(np.allclose([70.015, 0.000, 7.510], data_raos[15].attributes.position))
        self.assertEqual(-1, data_raos[15].attributes.h)
        self.assertEqual("m", data_raos[15].attributes.h_unit)
        self.assertEqual(0.5, data_raos[15].attributes.phi_a)
        self.assertEqual("deg", data_raos[15].attributes.phi_a_unit)
        self.assertEqual(12, data_raos[15].attributes.U)
        self.assertEqual("kn", data_raos[15].attributes.U_unit)
        self.assertEqual(90, data_raos[15].attributes.mu)
        self.assertEqual("deg", data_raos[15].attributes.mu_unit)
        self.assertEqual("m/m", data_raos[15].attributes.amplitude_unit)
        self.assertEqual("deg", data_raos[15].attributes.phase_unit)
        self.assertEqual(0.100835E+01, data_raos[15].left_column[1])
        self.assertEqual(0.105814E+01, data_raos[15].left_column[6])
        self.assertEqual(-0.081168, data_raos[15].right_column[1])
        self.assertEqual(-15.504014, data_raos[15].right_column[6])

        # No phases - Signal 10: F_drift_m1_c4, U=20.000kn, mu=180.000deg
        self.assertEqual("kN/m2", data_raos[38].attributes.amplitude_unit)
        self.assertEqual(-0.655426E+01, data_raos[38].left_column[0])
        self.assertEqual(-0.690853E+02, data_raos[38].left_column[4])
        self.assertEqual("N.A.", data_raos[38].attributes.phase_unit)
        self.assertEqual(0, data_raos[38].right_column[0])
        self.assertEqual(0, data_raos[38].right_column[4])

    def test_can_parse_from_file(self):
        filename = "data.ini"
        with open(filename, "w", encoding="utf-8") as fid:
            fid.write(precal())
        data = parse_precal_from_file(filename)
        raos_15 = data.raos[15]
        self.assertEqual("heave motion", raos_15.attributes.name)
        os.remove(filename)

    def test_can_parse_added_mass_multi_line_vector(self):
        data = parse_precal_from_string(added_mass_damping_matrix_inf_freq())
        section = data.sections[0]
        self.assertEqual(1, len( data.sections))
        self.assertTrue("total_added_mass_matrix_inf_freq_U1_mu1" in section.vector_values)
        self.assertEqual(36,
            len(section.vector_values["total_added_mass_matrix_inf_freq_U1_mu1"]))
        self.assertEqual(0.110E+06,
            section.vector_values["total_added_mass_matrix_inf_freq_U1_mu1"][0])
        self.assertEqual(-0.888E-01,
            section.vector_values["total_added_mass_matrix_inf_freq_U1_mu1"][1])
        self.assertEqual(0.612E+01,
            section.vector_values["total_added_mass_matrix_inf_freq_U1_mu1"][5])

        self.assertTrue("total_added_mass_matrix_inf_freq_U1_mu2" in section.vector_values)
        self.assertEqual(36,
            len(section.vector_values["total_added_mass_matrix_inf_freq_U1_mu2"]))
        self.assertEqual(0.174E+02,
            section.vector_values["total_added_mass_matrix_inf_freq_U1_mu2"][29])

        self.assertTrue("total_added_mass_matrix_inf_freq_U2_mu1" in section.vector_values)
        self.assertEqual(36,
            len(section.vector_values["total_added_mass_matrix_inf_freq_U2_mu1"]))
        self.assertEqual(0.570E+01,
            section.vector_values["total_added_mass_matrix_inf_freq_U2_mu1"][5])

        self.assertTrue("total_added_mass_matrix_inf_freq_U2_mu2" in section.vector_values)
        self.assertEqual(36,
                len(section.vector_values["total_added_mass_matrix_inf_freq_U2_mu2"]))
        self.assertEqual(-0.119E+03,
            section.vector_values["total_added_mass_matrix_inf_freq_U2_mu2"][29])

    def test_can_parse_added_mass_matrix(self):
        """
        FROM PRECAL_R
                 0.110E+06  -0.888E-01   0.226E+06  -0.144E+00  0.270E+08   0.612E+01
                -0.122E-01   0.344E+07  -0.563E-02  -0.113E+07  0.169E+02   0.497E+08
                 0.227E+06  -0.898E+00   0.129E+08   0.763E+01  0.844E+08   0.512E+01
                 0.183E+00  -0.123E+07   0.251E+01   0.498E+08  0.854E+02   0.338E+09
                 0.270E+08   0.106E+01   0.845E+08  -0.431E+02  0.119E+11   0.174E+02
                 0.164E+01   0.497E+08   0.101E+02   0.345E+09  0.214E+03   0.522E+10


        Ma(0,1) = -Ma(0,1)
        Ma(0,2) = -Ma(0,2)
        Ma(0,4) = -Ma(0,4)
        Ma(0,5) = -Ma(0,5)
        Ma(1,0) = -Ma(1,0)
        Ma(1,3) = -Ma(1,3)
        Ma(2,0) = -Ma(2,0)
        Ma(2,3) = -Ma(2,3)
        Ma(3,1) = -Ma(3,1)
        Ma(3,2) = -Ma(3,2)
        Ma(3,4) = -Ma(3,4)
        Ma(3,5) = -Ma(3,5)
        Ma(4,0) = -Ma(4,0)
        Ma(4,3) = -Ma(4,3)
        Ma(5,0) = -Ma(5,0)
        Ma(5,3) = -Ma(5,3)
        """
        data = PrecalParser.from_string(precal())
        Ma = data.get_added_mass()
        self.assertEqual(+0.110E+06, +Ma[0, 0])
        self.assertEqual(-0.888E-01, -Ma[0, 1])
        self.assertEqual(+0.226E+06, -Ma[0, 2])
        self.assertEqual(-0.144E+00, +Ma[0, 3])
        self.assertEqual(+0.270E+08, -Ma[0, 4])
        self.assertEqual(+0.612E+01, -Ma[0, 5])

        self.assertEqual(-0.122E-01, -Ma[1, 0])
        self.assertEqual(+0.344E+07, +Ma[1, 1])
        self.assertEqual(-0.563E-02, +Ma[1, 2])
        self.assertEqual(-0.113E+07, -Ma[1, 3])
        self.assertEqual(+0.169E+02, +Ma[1, 4])
        self.assertEqual(+0.497E+08, +Ma[1, 5])

        self.assertEqual(+0.227E+06, -Ma[2, 0])
        self.assertEqual(-0.898E+00, +Ma[2, 1])
        self.assertEqual(+0.129E+08, +Ma[2, 2])
        self.assertEqual(+0.763E+01, -Ma[2, 3])
        self.assertEqual(+0.844E+08, +Ma[2, 4])
        self.assertEqual(+0.512E+01, +Ma[2, 5])

        self.assertEqual(+0.183E+00, +Ma[3, 0])
        self.assertEqual(-0.123E+07, -Ma[3, 1])
        self.assertEqual(+0.251E+01, -Ma[3, 2])
        self.assertEqual(+0.498E+08, +Ma[3, 3])
        self.assertEqual(+0.854E+02, -Ma[3, 4])
        self.assertEqual(+0.338E+09, -Ma[3, 5])

        self.assertEqual(+0.270E+08, -Ma[4, 0])
        self.assertEqual(+0.106E+01, +Ma[4, 1])
        self.assertEqual(+0.845E+08, +Ma[4, 2])
        self.assertEqual(-0.431E+02, -Ma[4, 3])
        self.assertEqual(+0.119E+11, +Ma[4, 4])
        self.assertEqual(+0.174E+02, +Ma[4, 5])

        self.assertEqual(0.164E+01, -Ma[5, 0])
        self.assertEqual(0.497E+08, +Ma[5, 1])
        self.assertEqual(0.101E+02, +Ma[5, 2])
        self.assertEqual(0.345E+09, -Ma[5, 3])
        self.assertEqual(0.214E+03, +Ma[5, 4])
        self.assertEqual(0.522E+10, +Ma[5, 5])

    def test_can_parse_angular_frequencies(self):
        data = PrecalParser.from_string(precal())
        omegas = data.get_angular_frequencies()
        self.assertEqual(7, len(omegas))
        self.assertEqual(0.400, omegas[0])
        self.assertEqual(0.500, omegas[1])
        self.assertEqual(0.600, omegas[2])
        self.assertEqual(0.700, omegas[3])
        self.assertEqual(0.800, omegas[4])
        self.assertEqual(0.900, omegas[5])
        self.assertEqual(1.000, omegas[6])

    def test_can_get_forward_speed(self):
        data = PrecalParser.from_string(precal())
        self.assertEqual(0, data.get_forward_speed())

    def test_can_get_added_mass_coefficients_for_each_frequency(self):
        data = PrecalParser.from_string(precal())
        A_11 = data.get_added_mass_coeff(0, 0)
        self.assertEqual(7, len(A_11))
        self.assertEqual(0.275560E+03, A_11[0])
        self.assertEqual(0.271498E+03, A_11[1])
        self.assertEqual(0.233842E+03, A_11[2])
        self.assertEqual(0.183239E+03, A_11[3])
        self.assertEqual(0.147226E+03, A_11[4])
        self.assertEqual(0.128148E+03, A_11[5])
        self.assertEqual(0.114148E+03, A_11[6])

    def test_can_get_radiation_damping_matrix_coefficients_for_each_frequency(self):
        data = PrecalParser.from_string(precal())
        Br_34 = data.get_radiation_damping_coeff(2, 3)
        self.assertEqual(7, len(Br_34))
        self.assertEqual(-0.223749E-03, Br_34[0])
        self.assertEqual(-0.214409E-03, Br_34[1])
        self.assertEqual( 0.123597E-02, Br_34[2])
        self.assertEqual( 0.176562E-02, Br_34[3])
        self.assertEqual( 0.260162E-03, Br_34[4])
        self.assertEqual(-0.352648E-03, Br_34[5])
        self.assertEqual(-0.102490E-02, Br_34[6])

    def test_can_parse_diffraction_module_raos(self):
        data = PrecalParser.from_string(precal())

        periods = data.get_diffraction_module_periods()
        self.assertEqual(7, len(periods))
        self.assertEqual(2 * np.pi / 0.4, periods[6])
        self.assertEqual(2 * np.pi / 0.5, periods[5])
        self.assertEqual(2 * np.pi / 0.6, periods[4])
        self.assertEqual(2 * np.pi / 0.7, periods[3])
        self.assertEqual(2 * np.pi / 0.8, periods[2])
        self.assertEqual(2 * np.pi / 0.9, periods[1])
        self.assertEqual(2 * np.pi / 1.0, periods[0])

        psis = data.get_diffraction_module_psis()
        self.assertEqual(2, len(psis))
        self.assertEqual(np.pi, psis[1])
        self.assertEqual(np.pi / 2, psis[0])

        table = data.get_diffraction_module_tables()
        for mod_idx in range(6):
            self.assertEqual(7, len(table[mod_idx]))
            for period_idx in range(7):
                self.assertEqual(2, len(table[mod_idx][period_idx]))

        # Multiplied by 1e3 to convert the table data to N/m
        # column (axis), pulsation, incidence
        self.assertEqual(0.138050E+03 * 1e3, table[0][6][1]) # X (F_dif_m1), first line, 180°
        self.assertEqual(0.117473E-02 * 1e3, table[1][6][1]) # Y (F_dif_m2), first line, 180°
        self.assertEqual(0.847017E+04 * 1e3, table[2][4][0]) # Z (F_dif_m3), third line, 90°
        self.assertEqual(0.360716E+04 * 1e3, table[3][1][0]) # K (F_dif_m4), sixth line, 90°
        self.assertEqual(0.898604E+05 * 1e3, table[4][1][0]) # M (F_dif_m5), sixth line, 90°
        self.assertEqual(0.291656E-01 * 1e3, table[5][0][1]) # N (F_dif_m6), seventh line, 180°

    def test_can_parse_diffraction_phase_raos(self):
        data = PrecalParser.from_string(precal())

        periods = data.get_diffraction_phase_periods()
        self.assertEqual(7, len(periods))
        self.assertEqual(2 * np.pi / 0.4, periods[6])
        self.assertEqual(2 * np.pi / 0.5, periods[5])
        self.assertEqual(2 * np.pi / 0.6, periods[4])
        self.assertEqual(2 * np.pi / 0.7, periods[3])
        self.assertEqual(2 * np.pi / 0.8, periods[2])
        self.assertEqual(2 * np.pi / 0.9, periods[1])
        self.assertEqual(2 * np.pi / 1.0, periods[0])

        psis = data.get_diffraction_phase_psis()
        self.assertEqual(2, len(psis))
        self.assertEqual(np.pi, psis[1])
        self.assertEqual(np.pi / 2, psis[0])

        table = data.get_diffraction_phase_tables()
        for mod_idx in range(6):
            self.assertEqual(7, len(table[mod_idx]))
            for period_idx in range(7):
                self.assertEqual(2, len(table[mod_idx][period_idx]))

        # column (axis), pulsation, incidence
        self.assertAlmostEqual(  90.317017 * np.pi / 180, table[0][6][1]) # X (F_dif_m1), first line, 180°
        self.assertAlmostEqual( -48.947906 * np.pi / 180, table[1][6][1]) # Y (F_dif_m2), first line, 180°
        self.assertAlmostEqual( 131.552856 * np.pi / 180, table[2][4][0]) # Z (F_dif_m3), third line, 90°
        self.assertAlmostEqual(-128.995148 * np.pi / 180, table[3][4][0]) # K (F_dif_m4), third line, 90°
        self.assertAlmostEqual( 137.499237 * np.pi / 180, table[4][1][0]) # M (F_dif_m5), sixth line, 90°
        self.assertAlmostEqual( -50.890854 * np.pi / 180, table[5][0][1]) # N (F_dif_m6), seventh line, 180°

    def test_can_parse_froude_krylov_module_raos(self):
        # sim > parRES > exnp.pincWaveFrc
        data = PrecalParser.from_string(precal())
        periods = data.get_froude_krylov_module_periods()
        self.assertEqual(7, len(periods))
        self.assertEqual(2 * np.pi / 0.4, periods[6])
        self.assertEqual(2 * np.pi / 0.5, periods[5])
        self.assertEqual(2 * np.pi / 0.6, periods[4])
        self.assertEqual(2 * np.pi / 0.7, periods[3])
        self.assertEqual(2 * np.pi / 0.8, periods[2])
        self.assertEqual(2 * np.pi / 0.9, periods[1])
        self.assertEqual(2 * np.pi / 1.0, periods[0])

        psis = data.get_froude_krylov_module_psis()
        self.assertEqual(2, len(psis))
        self.assertEqual(np.pi, psis[1])
        self.assertEqual(np.pi / 2, psis[0])

        table = data.get_froude_krylov_module_tables()
        for mod_idx in range(6):
            self.assertEqual(7, len(table[mod_idx]))
            for period_idx in range(7):
                self.assertEqual(2, len(table[mod_idx][period_idx]))

        # Multiplied by 1e3 to convert the table data to N/m
        # column (axis), pulsation, incidence
        self.assertEqual(0.419735E+02 * 1E3, table[0][6][0]) # X (F_inc_m1), first line, 90°
        self.assertEqual(0.988816E-03 * 1E3, table[1][5][1]) # Y (F_inc_m2), second line, 180°
        self.assertEqual(0.179547E+05 * 1E3, table[2][4][0]) # Z (F_inc_m3), third line, 90°
        self.assertEqual(0.658274E-02 * 1E3, table[3][3][1]) # K (F_inc_m4), fourth line, 180°
        self.assertEqual(0.114842E+06 * 1E3, table[4][2][0]) # M (F_inc_m5), fifth line, 90°
        self.assertEqual(0.208950E-01 * 1E3, table[5][1][1]) # N (F_inc_m6), sixth line, 180°

    def test_can_parse_froude_krylov_phase_raos(self):
        data = PrecalParser.from_string(precal())
        periods = data.get_froude_krylov_phase_periods()
        self.assertEqual(7, len(periods))
        self.assertEqual(2 * np.pi / 0.4, periods[6])
        self.assertEqual(2 * np.pi / 0.5, periods[5])
        self.assertEqual(2 * np.pi / 0.6, periods[4])
        self.assertEqual(2 * np.pi / 0.7, periods[3])
        self.assertEqual(2 * np.pi / 0.8, periods[2])
        self.assertEqual(2 * np.pi / 0.9, periods[1])
        self.assertEqual(2 * np.pi / 1.0, periods[0])

        psis = data.get_froude_krylov_phase_psis()
        self.assertEqual(2, len(psis))
        self.assertEqual(np.pi, psis[1])
        self.assertEqual(np.pi / 2, psis[0])

        table = data.get_froude_krylov_phase_tables()
        for mod_idx in range(6):
            self.assertEqual(7, len(table[mod_idx]))
            for period_idx in range(7):
                self.assertEqual(2, len(table[mod_idx][period_idx]))
        # column (axis), pulsation, incidence
        self.assertAlmostEqual(-179.999939 * np.pi/180, table[0][6][0]) # X (F_inc_m1), first line, 90°
        self.assertAlmostEqual( 171.732681 * np.pi/180, table[1][5][1]) # Y (F_inc_m2), second line, 180°
        self.assertAlmostEqual(  -0.000003 * np.pi/180, table[2][4][0]) # Z (F_inc_m3), third line, 90°
        self.assertAlmostEqual( -26.438093 * np.pi/180, table[3][3][1]) # K (F_inc_m4), fourth line, 180°
        self.assertAlmostEqual(  -0.000017 * np.pi/180, table[4][2][0]) # M (F_inc_m5), fifth line, 90°
        self.assertAlmostEqual(  -0.018264 * np.pi/180, table[5][1][1]) # N (F_inc_m6), sixth line, 180°


if __name__ == "__main__":

    unittest.main()
