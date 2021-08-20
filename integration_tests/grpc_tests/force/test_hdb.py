import csv

def check_added_mass_matrix(results):
    """Verify the added mass matrix."""
    assert results['Ma(0 0)(TestShip)'] == '8.770898e+04'
    assert results['Ma(0 2)(TestShip)'] == '-2.122348e+05'
    assert results['Ma(0 4)(TestShip)'] == '-1.844677e+07'
    assert results['Ma(1 1)(TestShip)'] == '2.406631e+06'
    assert results['Ma(1 3)(TestShip)'] == '1.298266e+06'
    assert results['Ma(1 5)(TestShip)'] == '2.894931e+07'
    assert results['Ma(2 0)(TestShip)'] == '-2.124077e+05'
    assert results['Ma(2 2)(TestShip)'] == '9.329103e+06'
    assert results['Ma(2 4)(TestShip)'] == '6.872522e+07'
    assert results['Ma(3 1)(TestShip)'] == '1.301950e+06'
    assert results['Ma(3 3)(TestShip)'] == '4.088904e+07'
    assert results['Ma(3 5)(TestShip)'] == '-2.408104e+08'
    assert results['Ma(4 0)(TestShip)'] == '-1.875139e+07'
    assert results['Ma(4 2)(TestShip)'] == '6.937881e+07'
    assert results['Ma(4 4)(TestShip)'] == '8.948366e+09'
    assert results['Ma(5 1)(TestShip)'] == '2.894596e+07'
    assert results['Ma(5 3)(TestShip)'] == '-2.408350e+08'
    assert results['Ma(5 5)(TestShip)'] == '3.803237e+09'
    print("✓ Added mass matrix test passed!")


def check_diffraction_module_tables(results):
    """Verify the diffraction module tables."""
    # First line of [INCIDENCE_EFM_MOD_001]   0.000000
    assert results['diffX(0 0)(TestShip)'] == '5.084407e+04'
    assert results['diffY(0 0)(TestShip)'] == '0.000000e+00'
    assert results['diffZ(0 0)(TestShip)'] == '3.997774e+05'
    assert results['diffK(0 0)(TestShip)'] == '0.000000e+00'
    assert results['diffM(0 0)(TestShip)'] == '3.209051e+07'
    assert results['diffN(0 0)(TestShip)'] == '0.000000e+00'
    # Second line of [INCIDENCE_EFM_MOD_001]   0.000000
    assert results['diffX(1 0)(TestShip)'] == '3.378373e+03'
    assert results['diffY(1 0)(TestShip)'] == '0.000000e+00'
    assert results['diffZ(1 0)(TestShip)'] == '3.282703e+05'
    assert results['diffK(1 0)(TestShip)'] == '0.000000e+00'
    assert results['diffM(1 0)(TestShip)'] == '2.267054e+06'
    assert results['diffN(1 0)(TestShip)'] == '0.000000e+00'
    # Third line of [INCIDENCE_EFM_MOD_001]   0.000000
    assert results['diffX(2 0)(TestShip)'] == '9.472648e+02'
    assert results['diffY(2 0)(TestShip)'] == '0.000000e+00'
    assert results['diffZ(2 0)(TestShip)'] == '8.336677e+04'
    assert results['diffK(2 0)(TestShip)'] == '0.000000e+00'
    assert results['diffM(2 0)(TestShip)'] == '5.817311e+05'
    assert results['diffN(2 0)(TestShip)'] == '0.000000e+00'
    # First line of [INCIDENCE_EFM_MOD_001]   30.00000
    assert results['diffX(0 1)(TestShip)'] == '8.567009e+04'
    assert results['diffY(0 1)(TestShip)'] == '3.284304e+05'
    assert results['diffZ(0 1)(TestShip)'] == '5.820626e+05'
    assert results['diffK(0 1)(TestShip)'] == '1.460688e+06'
    assert results['diffM(0 1)(TestShip)'] == '3.140660e+07'
    assert results['diffN(0 1)(TestShip)'] == '2.301511e+07'
    # Second line of [INCIDENCE_EFM_MOD_001]   30.00000
    assert results['diffX(1 1)(TestShip)'] == '3.275670e+03'
    assert results['diffY(1 1)(TestShip)'] == '3.409648e+04'
    assert results['diffZ(1 1)(TestShip)'] == '3.283568e+05'
    assert results['diffK(1 1)(TestShip)'] == '1.685674e+04'
    assert results['diffM(1 1)(TestShip)'] == '2.255748e+06'
    assert results['diffN(1 1)(TestShip)'] == '4.594584e+05'
    # Third line of [INCIDENCE_EFM_MOD_001]   30.00000
    assert results['diffX(2 1)(TestShip)'] == '9.067188e+02'
    assert results['diffY(2 1)(TestShip)'] == '8.953133e+03'
    assert results['diffZ(2 1)(TestShip)'] == '8.339660e+04'
    assert results['diffK(2 1)(TestShip)'] == '4.420171e+03'
    assert results['diffM(2 1)(TestShip)'] == '5.772251e+05'
    assert results['diffN(2 1)(TestShip)'] == '1.199319e+05'
    print("✓ Diffraction module table test passed!")


def check_diffraction_phase_tables(results):
    """Verify the diffraction phase tables."""
    # First line of [INCIDENCE_EFM_PH_001]   0.000000
    assert results['diffpX(0 0)(TestShip)'] == '-1.135123e+00'
    assert results['diffpY(0 0)(TestShip)'] == '1.570796e+00'
    assert results['diffpZ(0 0)(TestShip)'] == '-8.389206e-01'
    assert results['diffpK(0 0)(TestShip)'] == '1.570796e+00'
    assert results['diffpM(0 0)(TestShip)'] == '-8.356066e-01'
    assert results['diffpN(0 0)(TestShip)'] == '1.570796e+00'
    # Second line of [INCIDENCE_EFM_PH_001]   0.000000
    assert results['diffpX(1 0)(TestShip)'] == '2.088816e+00'
    assert results['diffpY(1 0)(TestShip)'] == '1.570796e+00'
    assert results['diffpZ(1 0)(TestShip)'] == '1.636404e+00'
    assert results['diffpK(1 0)(TestShip)'] == '1.570796e+00'
    assert results['diffpM(1 0)(TestShip)'] == '1.789263e+00'
    assert results['diffpN(1 0)(TestShip)'] == '1.570796e+00'
    # Third line of [INCIDENCE_EFM_PH_001]   0.000000
    assert results['diffpX(2 0)(TestShip)'] == '2.189364e+00'
    assert results['diffpY(2 0)(TestShip)'] == '1.570796e+00'
    assert results['diffpZ(2 0)(TestShip)'] == '1.596137e+00'
    assert results['diffpK(2 0)(TestShip)'] == '1.570796e+00'
    assert results['diffpM(2 0)(TestShip)'] == '1.825686e+00'
    assert results['diffpN(2 0)(TestShip)'] == '1.570796e+00'
    # First line of [INCIDENCE_EFM_PH_001]   30.00000
    assert results['diffpX(0 1)(TestShip)'] == '2.077326e+00'
    assert results['diffpY(0 1)(TestShip)'] == '-5.459499e-01'
    assert results['diffpZ(0 1)(TestShip)'] == '1.525810e+00'
    assert results['diffpK(0 1)(TestShip)'] == '-6.670656e-01'
    assert results['diffpM(0 1)(TestShip)'] == '1.375271e+00'
    assert results['diffpN(0 1)(TestShip)'] == '-5.846877e-01'
    # Second line of [INCIDENCE_EFM_PH_001]   30.00000
    assert results['diffpX(1 1)(TestShip)'] == '2.028876e+00'
    assert results['diffpY(1 1)(TestShip)'] == '-3.130008e+00'
    assert results['diffpZ(1 1)(TestShip)'] == '1.636047e+00'
    assert results['diffpK(1 1)(TestShip)'] == '-1.350179e-01'
    assert results['diffpM(1 1)(TestShip)'] == '1.768062e+00'
    assert results['diffpN(1 1)(TestShip)'] == '-3.053413e+00'
    # Third line of [INCIDENCE_EFM_PH_001]   30.00000
    assert results['diffpX(2 1)(TestShip)'] == '2.123063e+00'
    assert results['diffpY(2 1)(TestShip)'] == '-3.138567e+00'
    assert results['diffpZ(2 1)(TestShip)'] == '1.595086e+00'
    assert results['diffpK(2 1)(TestShip)'] == '-3.533363e-02'
    assert results['diffpM(2 1)(TestShip)'] == '1.794767e+00'
    assert results['diffpN(2 1)(TestShip)'] == '-3.118377e+00'
    print("✓ Diffraction phase table test passed!")



def check(results):
    """Make sure the results contain all necessary data."""
    check_added_mass_matrix(results)
    check_diffraction_module_tables(results)
    check_diffraction_phase_tables(results)


with open('hdb_output.csv', newline='') as csvfile:
    csv_reader = csv.DictReader(csvfile)
    results = next(csv_reader)
    check(results)
