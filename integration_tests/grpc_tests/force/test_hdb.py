import csv

def check_added_mass_matrix(results):
    """Verify the added mass matrix."""
    assert results['Ma(0 0)(TestShip)'] == '8.880216e+04'
    assert results['Ma(0 2)(TestShip)'] == '-2.053664e+05'
    assert results['Ma(0 4)(TestShip)'] == '-1.951857e+07'
    assert results['Ma(1 1)(TestShip)'] == '2.184364e+06'
    assert results['Ma(1 3)(TestShip)'] == '1.297799e+06'
    assert results['Ma(1 5)(TestShip)'] == '2.482503e+07'
    assert results['Ma(2 0)(TestShip)'] == '-2.014862e+05'
    assert results['Ma(2 2)(TestShip)'] == '9.694248e+06'
    assert results['Ma(2 4)(TestShip)'] == '6.754937e+07'
    assert results['Ma(3 1)(TestShip)'] == '1.280046e+06'
    assert results['Ma(3 3)(TestShip)'] == '3.911908e+07'
    assert results['Ma(3 5)(TestShip)'] == '-2.220968e+08'
    assert results['Ma(4 0)(TestShip)'] == '-1.969726e+07'
    assert results['Ma(4 2)(TestShip)'] == '6.959714e+07'
    assert results['Ma(4 4)(TestShip)'] == '9.173996e+09'
    assert results['Ma(5 1)(TestShip)'] == '2.482780e+07'
    assert results['Ma(5 3)(TestShip)'] == '-2.223681e+08'
    assert results['Ma(5 5)(TestShip)'] == '3.318626e+09'
    print("✓ Added mass matrix test passed!")


def check_diffraction_module_tables(results):
    """Verify the diffraction module tables."""
    # First line of [INCIDENCE_EFM_MOD_001]   0.000000
    assert results['diffX(0 0)(TestShip)'] == '3.010048e+04'
    assert results['diffY(0 0)(TestShip)'] == '0.000000e+00'
    assert results['diffZ(0 0)(TestShip)'] == '3.715063e+05'
    assert results['diffK(0 0)(TestShip)'] == '0.000000e+00'
    assert results['diffM(0 0)(TestShip)'] == '2.584082e+07'
    assert results['diffN(0 0)(TestShip)'] == '0.000000e+00'
    # Second line of [INCIDENCE_EFM_MOD_001]   0.000000
    assert results['diffX(1 0)(TestShip)'] == '8.075781e+04'
    assert results['diffY(1 0)(TestShip)'] == '0.000000e+00'
    assert results['diffZ(1 0)(TestShip)'] == '4.013992e+05'
    assert results['diffK(1 0)(TestShip)'] == '0.000000e+00'
    assert results['diffM(1 0)(TestShip)'] == '2.476660e+07'
    assert results['diffN(1 0)(TestShip)'] == '0.000000e+00'
    # Third line of [INCIDENCE_EFM_MOD_001]   0.000000
    assert results['diffX(2 0)(TestShip)'] == '3.708646e+04'
    assert results['diffY(2 0)(TestShip)'] == '0.000000e+00'
    assert results['diffZ(2 0)(TestShip)'] == '3.778403e+05'
    assert results['diffK(2 0)(TestShip)'] == '0.000000e+00'
    assert results['diffM(2 0)(TestShip)'] == '2.904906e+07'
    assert results['diffN(2 0)(TestShip)'] == '0.000000e+00'
    # First line of [INCIDENCE_EFM_MOD_001]   15.00000
    assert results['diffX(0 1)(TestShip)'] == '6.191774e+04'
    assert results['diffY(0 1)(TestShip)'] == '9.417432e+04'
    assert results['diffZ(0 1)(TestShip)'] == '3.924593e+05'
    assert results['diffK(0 1)(TestShip)'] == '3.232162e+05'
    assert results['diffM(0 1)(TestShip)'] == '2.506212e+07'
    assert results['diffN(0 1)(TestShip)'] == '6.068722e+06'
    # Second line of [INCIDENCE_EFM_MOD_001]   15.00000
    assert results['diffX(1 1)(TestShip)'] == '6.697170e+04'
    assert results['diffY(1 1)(TestShip)'] == '9.952259e+04'
    assert results['diffZ(1 1)(TestShip)'] == '3.976509e+05'
    assert results['diffK(1 1)(TestShip)'] == '4.678782e+05'
    assert results['diffM(1 1)(TestShip)'] == '2.646309e+07'
    assert results['diffN(1 1)(TestShip)'] == '7.431062e+06'
    # Third line of [INCIDENCE_EFM_MOD_001]   15.00000
    assert results['diffX(2 1)(TestShip)'] == '2.422100e+04'
    assert results['diffY(2 1)(TestShip)'] == '1.147413e+05'
    assert results['diffZ(2 1)(TestShip)'] == '3.886627e+05'
    assert results['diffK(2 1)(TestShip)'] == '6.107079e+05'
    assert results['diffM(2 1)(TestShip)'] == '2.996725e+07'
    assert results['diffN(2 1)(TestShip)'] == '8.392296e+06'
    print("✓ Diffraction module table test passed!")


def check_diffraction_phase_tables(results):
    """Verify the diffraction phase tables."""
    # First line of [INCIDENCE_EFM_PH_001]   0.000000
    assert results['diffpX(0 0)(TestShip)'] == '-1.913590e+00'
    assert results['diffpY(0 0)(TestShip)'] == '-3.141593e+00'
    assert results['diffpZ(0 0)(TestShip)'] == '-1.633579e+00'
    assert results['diffpK(0 0)(TestShip)'] == '-3.141593e+00'
    assert results['diffpM(0 0)(TestShip)'] == '-1.475407e+00'
    assert results['diffpN(0 0)(TestShip)'] == '-3.141593e+00'
    # Second line of [INCIDENCE_EFM_PH_001]   0.
    assert results['diffpX(1 0)(TestShip)'] == '-2.698030e+00'
    assert results['diffpY(1 0)(TestShip)'] == '-3.141593e+00'
    assert results['diffpZ(1 0)(TestShip)'] == '-2.832685e+00'
    assert results['diffpK(1 0)(TestShip)'] == '-3.141593e+00'
    assert results['diffpM(1 0)(TestShip)'] == '-2.815060e+00'
    assert results['diffpN(1 0)(TestShip)'] == '-3.141593e+00'
    # Third line of [INCIDENCE_EFM_PH_001]   0.0
    assert results['diffpX(2 0)(TestShip)'] == '3.042987e+00'
    assert results['diffpY(2 0)(TestShip)'] == '-3.141593e+00'
    assert results['diffpZ(2 0)(TestShip)'] == '2.256659e+00'
    assert results['diffpK(2 0)(TestShip)'] == '-3.141593e+00'
    assert results['diffpM(2 0)(TestShip)'] == '2.305372e+00'
    assert results['diffpN(2 0)(TestShip)'] == '-3.141593e+00'
    # First line of [INCIDENCE_EFM_PH_001]   15
    assert results['diffpX(0 1)(TestShip)'] == '-2.610210e+00'
    assert results['diffpY(0 1)(TestShip)'] == '-1.654227e+00'
    assert results['diffpZ(0 1)(TestShip)'] == '-2.427624e+00'
    assert results['diffpK(0 1)(TestShip)'] == '-1.975984e+00'
    assert results['diffpM(0 1)(TestShip)'] == '-2.245154e+00'
    assert results['diffpN(0 1)(TestShip)'] == '-1.745885e+00'
    # Second line of [INCIDENCE_EFM_PH_001]   15
    assert results['diffpX(1 1)(TestShip)'] == '-3.095945e+00'
    assert results['diffpY(1 1)(TestShip)'] == '-4.902737e-01'
    assert results['diffpZ(1 1)(TestShip)'] == '2.732047e+00'
    assert results['diffpK(1 1)(TestShip)'] == '-2.358229e-01'
    assert results['diffpM(1 1)(TestShip)'] == '2.686907e+00'
    assert results['diffpN(1 1)(TestShip)'] == '-4.208167e-01'
    # Third line of [INCIDENCE_EFM_PH_001]   15
    assert results['diffpX(2 1)(TestShip)'] == '1.369427e+00'
    assert results['diffpY(2 1)(TestShip)'] == '7.410035e-01'
    assert results['diffpZ(2 1)(TestShip)'] == '1.499941e+00'
    assert results['diffpK(2 1)(TestShip)'] == '5.354755e-01'
    assert results['diffpM(2 1)(TestShip)'] == '1.678369e+00'
    assert results['diffpN(2 1)(TestShip)'] == '6.210995e-01'
    print("✓ Diffraction phase table test passed!")


def check_diffraction_module_periods(results):
    assert results['diffT0(TestShip)'] == '3.500000e+00'
    assert results['diffT1(TestShip)'] == '3.600000e+00'
    assert results['diffT2(TestShip)'] == '3.700000e+00'
    print("✓ Diffraction module periods test passed!")


def check_diffraction_phase_periods(results):
    assert results['diffTp0(TestShip)'] == '3.500000e+00'
    assert results['diffTp1(TestShip)'] == '3.600000e+00'
    assert results['diffTp2(TestShip)'] == '3.700000e+00'
    print("✓ Diffraction phase periods test passed!")

def check_diffraction_module_psis(results):
    assert results['diffpsi0(TestShip)'] == '0.000000e+00'
    assert results['diffpsi1(TestShip)'] == '2.617994e-01'
    print("✓ Diffraction module incidences test passed!")

def check_diffraction_phase_psis(results):
    assert results['diffppsi0(TestShip)'] == '0.000000e+00'
    assert results['diffppsi1(TestShip)'] == '2.617994e-01'
    print("✓ Diffraction phase incidences test passed!")

def check_froude_krylov_module_tables(results):
    # First line of [INCIDENCE_FKFM_MOD_001]   0.000000
    assert results['FKFMX(0 0)(TestShip)'] == '4.832189e+04'
    assert results['FKFMY(0 0)(TestShip)'] == '0.000000e+00'
    assert results['FKFMZ(0 0)(TestShip)'] == '2.475141e+05'
    assert results['FKFMK(0 0)(TestShip)'] == '0.000000e+00'
    assert results['FKFMM(0 0)(TestShip)'] == '2.139539e+07'
    assert results['FKFMN(0 0)(TestShip)'] == '0.000000e+00'
    # Second line of [INCIDENCE_FKFM_MOD_001]   0.000000
    assert results['FKFMX(1 0)(TestShip)'] == '4.103861e+04'
    assert results['FKFMY(1 0)(TestShip)'] == '0.000000e+00'
    assert results['FKFMZ(1 0)(TestShip)'] == '3.251938e+05'
    assert results['FKFMK(1 0)(TestShip)'] == '0.000000e+00'
    assert results['FKFMM(1 0)(TestShip)'] == '1.861110e+07'
    assert results['FKFMN(1 0)(TestShip)'] == '0.000000e+00'
    print("✓ Froude-Krylov module tables test passed!")

def check_froude_krylov_phase_tables(results):
    # First line of [INCIDENCE_FKFM_PH_001]   0.000000
    assert results['FKFMpX(0 0)(TestShip)'] == ''
    assert results['FKFMpY(0 0)(TestShip)'] == ''
    assert results['FKFMpZ(0 0)(TestShip)'] == ''
    assert results['FKFMpK(0 0)(TestShip)'] == ''
    assert results['FKFMpM(0 0)(TestShip)'] == ''
    assert results['FKFMpN(0 0)(TestShip)'] == ''
    # Second line of [INCIDENCE_FKFM_PH_001]   0.000000
    assert results['FKFMpX(1 0)(TestShip)'] == ''
    assert results['FKFMpY(1 0)(TestShip)'] == ''
    assert results['FKFMpZ(1 0)(TestShip)'] == ''
    assert results['FKFMpK(1 0)(TestShip)'] == ''
    assert results['FKFMpM(1 0)(TestShip)'] == ''
    assert results['FKFMpN(1 0)(TestShip)'] == ''
    print("✓ Froude-Krylov phase tables test passed!")

def check_froude_krylov_module_periods(results):
    assert results['FKT0(TestShip)'] == '3.500000e+00'
    assert results['FKT1(TestShip)'] == '3.600000e+00'
    assert results['FKT2(TestShip)'] == '3.700000e+00'
    print("✓ Froude-Krylov module periods test passed!")

def check_froude_krylov_phase_periods(results):
    assert results['FKTp0(TestShip)'] == '3.500000e+00'
    assert results['FKTp1(TestShip)'] == '3.600000e+00'
    assert results['FKTp2(TestShip)'] == '3.700000e+00'
    assert results['FKTp46(TestShip)'] == '1.250000e+02'
    print("✓ Froude-Krylov phase periods test passed!")

def check_froude_krylov_module_psis(results):
    assert results['FKpsi0(TestShip)'] == '0.000000e+00'
    assert results['FKpsi1(TestShip)'] == '2.617994e-01'
    assert results['FKpsi2(TestShip)'] == '5.235988e-01'
    print("✓ Froude-Krylov module incidence test passed!")

def check_froude_krylov_phase_psis(results):
    assert results['FKppsi0(TestShip)'] == '0.000000e+00'
    assert results['FKppsi1(TestShip)'] == '2.617994e-01'
    assert results['FKppsi2(TestShip)'] == '5.235988e-01'
    print("✓ Froude-Krylov phase incidence test passed!")

def check_angular_frequencies(results):
    assert results['omega0(TestShip)'] == '5.026548e-02'
    assert results['omega1(TestShip)'] == '1.000507e-01'
    assert results['omega2(TestShip)'] == '1.499567e-01'
    print("✓ Angular frequencies test passed!")

def check(results):
    """Make sure the results contain all necessary data."""
    check_added_mass_matrix(results)
    check_diffraction_module_tables(results)
    check_diffraction_phase_tables(results)
    check_diffraction_module_periods(results)
    check_diffraction_phase_periods(results)
    check_diffraction_module_psis(results)
    check_diffraction_phase_psis(results)
    check_froude_krylov_module_tables(results)
    check_froude_krylov_module_periods(results)
    check_froude_krylov_phase_periods(results)
    check_froude_krylov_module_psis(results)
    check_froude_krylov_phase_psis(results)
    check_angular_frequencies(results)


with open('hdb_output.csv', newline='') as csvfile:
    csv_reader = csv.DictReader(csvfile)
    results = next(csv_reader)
    check(results)
