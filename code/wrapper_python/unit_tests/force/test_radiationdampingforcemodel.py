"""
Unit test for RadiationDampingForceModel
"""
import unittest
import os
from xdyn.core.io import TypeOfQuadrature, YamlRadiationDamping
from xdyn.data.yaml import radiation_damping
from xdyn.force import RadiationDampingForceModel
from xdyn.data.test import bug_3210
from xdyn.ssc.random import DataGenerator


def get_yaml_data(show_debug: bool = False) -> YamlRadiationDamping:
    ret = YamlRadiationDamping()
    ret.nb_of_points_for_retardation_function_discretization = 50
    ret.omega_min = 0
    ret.omega_max = 30
    ret.output_Br_and_K = show_debug
    ret.tau_min = 0.2094395
    ret.tau_max = 10
    ret.type_of_quadrature_for_convolution = TypeOfQuadrature.SIMPSON
    ret.type_of_quadrature_for_cos_transform = TypeOfQuadrature.SIMPSON
    return ret


class RadiationDampingForceModelTest(unittest.TestCase):
    """Test class for RadiationDampingForceModel"""

    def setUp(self):
        self.rng = DataGenerator(666)
        hdb_filename = "test_ship.hdb"
        with open(hdb_filename, "w", encoding="utf-8") as fid:
            fid.write(bug_3210())

    def tearDown(self) -> None:
        hdb_filename = "test_ship.hdb"
        os.remove(hdb_filename)

    def test_can_parse(self):
        """Check that parse function produces a valid RadiationDampingForceModelTest object"""
        data = RadiationDampingForceModel.parse(radiation_damping()).yaml
        self.assertEqual("test_ship.hdb", data.hdb_filename)
        self.assertEqual(50, data.nb_of_points_for_retardation_function_discretization)
        self.assertEqual(30, data.omega_max)
        self.assertEqual(0, data.omega_min)
        self.assertTrue(data.output_Br_and_K)
        self.assertEqual(10, data.tau_max)
        self.assertEqual(0.2094395, data.tau_min)
        self.assertEqual(
            TypeOfQuadrature.CLENSHAW_CURTIS, data.type_of_quadrature_for_convolution
        )
        self.assertEqual(
            TypeOfQuadrature.SIMPSON, data.type_of_quadrature_for_cos_transform
        )
        self.assertEqual(0.696, data.calculation_point_in_body_frame.x)
        self.assertEqual(0, data.calculation_point_in_body_frame.y)
        self.assertEqual(1.418, data.calculation_point_in_body_frame.z)
        self.assertTrue(data.forward_speed_correction)

    def test_example(self):
        pass

    def test_results_are_zero_for_constant_velocity(self):
        pass

    def test_velocity_offset_should_not_change_the_result(self):
        pass

    def test_should_print_debugging_information_if_required_by_yaml_data(self):
        pass

    def test_should_not_print_debugging_information_if_not_required_by_yaml_data(self):
        pass

    def test_force_model_knows_history_length(self):
        pass

    def test_matrix_product_should_be_done_properly(self):
        pass

    def test_cannot_specify_both_hdb_and_precal_r_files(self):
        pass

    def test_should_be_able_to_use_precal_r_files_only(self):
        pass

    def test_need_one_of_precal_r_or_hdb(self):
        pass

    def test_precal_r_filename_is_read_properly(self):
        pass

    def test_can_use_data_from_precal_r(self):
        pass


if __name__ == "__main__":

    unittest.main()
