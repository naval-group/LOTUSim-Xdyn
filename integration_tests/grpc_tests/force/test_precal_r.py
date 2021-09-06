import csv


def check_added_mass_matrix(results):
    """Verify the added mass matrix."""
    assert results['Ma(0 0)(TestShip)'] == '1.100000e+05', results['Ma(0 0)(TestShip)']
    assert results['Ma(0 2)(TestShip)'] == '-2.260000e+05', results['Ma(0 2)(TestShip)']
    assert results['Ma(0 4)(TestShip)'] == '-2.700000e+07', results['Ma(0 4)(TestShip)']
    assert results['Ma(1 1)(TestShip)'] == '3.440000e+06', results['Ma(1 1)(TestShip)']
    assert results['Ma(1 3)(TestShip)'] == '1.130000e+06', results['Ma(1 3)(TestShip)']
    print("✓ Added mass matrix test passed!")


def check_diffraction_module_tables(results):
    """Verify the diffraction module tables."""
    # First line of [INCIDENCE_EFM_MOD_001]   0.000000
    assert results['diffX(0 0)(TestShip)'] == '1.759320e+05', results['diffX(0 0)(TestShip)']
    assert results['diffY(0 0)(TestShip)'] == '2.615940e+06', results['diffY(0 0)(TestShip)']
    assert results['diffZ(0 0)(TestShip)'] == '1.177650e+07', results['diffZ(0 0)(TestShip)']
    # First line of [INCIDENCE_EFM_MOD_001]   15.00000
    assert results['diffX(0 1)(TestShip)'] == '2.488560e+05', results['diffX(0 1)(TestShip)']
    assert results['diffY(0 1)(TestShip)'] == '2.050860e-01', results['diffY(0 1)(TestShip)']
    assert results['diffZ(0 1)(TestShip)'] == '1.278260e+06', results['diffZ(0 1)(TestShip)']
    print("✓ Diffraction module table test passed!")


def check_diffraction_phase_tables(results):
    """Verify the diffraction phase tables."""
    # First line of [INCIDENCE_EFM_PH_001]   0.000000
    assert results['diffpX(0 0)(TestShip)'] == '2.851699e+00', results['diffpX(0 0)(TestShip)']
    assert results['diffpY(0 0)(TestShip)'] == '9.257872e-01', results['diffpY(0 0)(TestShip)']
    assert results['diffpZ(0 0)(TestShip)'] == '2.246679e+00', results['diffpZ(0 0)(TestShip)']
    # First line of [INCIDENCE_EFM_PH_001]   15
    assert results['diffpX(0 1)(TestShip)'] == '-2.551149e+00', results['diffpX(0 1)(TestShip)']
    assert results['diffpY(0 1)(TestShip)'] == '-2.956560e+00', results['diffpY(0 1)(TestShip)']
    assert results['diffpZ(0 1)(TestShip)'] == '-2.000946e+00', results['diffpZ(0 1)(TestShip)']
    print("✓ Diffraction phase table test passed!")


def check_diffraction_module_periods(results):
    assert results['diffT0(TestShip)'] == '6.283185e+00', results['diffT0(TestShip)']
    assert results['diffT1(TestShip)'] == '6.981317e+00', results['diffT1(TestShip)']
    assert results['diffT2(TestShip)'] == '7.853982e+00', results['diffT2(TestShip)']
    print("✓ Diffraction module periods test passed!")


def check_diffraction_phase_periods(results):
    assert results['diffTp0(TestShip)'] == '6.283185e+00', results['diffTp0(TestShip)']
    assert results['diffTp1(TestShip)'] == '6.981317e+00', results['diffTp1(TestShip)']
    assert results['diffTp2(TestShip)'] == '7.853982e+00', results['diffTp2(TestShip)']
    print("✓ Diffraction phase periods test passed!")

def check_diffraction_module_psis(results):
    assert results['diffpsi0(TestShip)'] == '1.570796e+00', results['diffpsi0(TestShip)']
    assert results['diffpsi1(TestShip)'] == '3.141593e+00', results['diffpsi1(TestShip)']
    print("✓ Diffraction module incidences test passed!")

def check_diffraction_phase_psis(results):
    assert results['diffppsi0(TestShip)'] == '1.570796e+00', results['diffppsi0(TestShip)']
    assert results['diffppsi1(TestShip)'] == '3.141593e+00', results['diffppsi1(TestShip)']
    print("✓ Diffraction phase incidences test passed!")

def check_froude_krylov_module_tables(results):
    # First line of [INCIDENCE_FKFM_MOD_001]   0.000000
    assert results['FKFMX(0 0)(TestShip)'] == '3.894510e+04', results['FKFMX(0 0)(TestShip)']
    assert results['FKFMY(0 0)(TestShip)'] == '6.161670e+06', results['FKFMY(0 0)(TestShip)']
    assert results['FKFMZ(0 0)(TestShip)'] == '1.245830e+07', results['FKFMZ(0 0)(TestShip)']
    print("✓ Froude-Krylov module tables test passed!")

def check_froude_krylov_phase_tables(results):
    # First line of [INCIDENCE_FKFM_PH_001]   0.000000
    assert results['FKFMpX(0 0)(TestShip)'] == '-3.141587e+00', results['FKFMpX(0 0)(TestShip)']
    assert results['FKFMpY(0 0)(TestShip)'] == '-3.141587e+00', results['FKFMpY(0 0)(TestShip)']
    assert results['FKFMpZ(0 0)(TestShip)'] == '-3.141587e+00', results['FKFMpZ(0 0)(TestShip)']
    print("✓ Froude-Krylov phase tables test passed!")

def check_froude_krylov_module_periods(results):
    assert results['FKT0(TestShip)'] == '6.283185e+00', results['FKT0(TestShip)']
    assert results['FKT1(TestShip)'] == '6.981317e+00', results['FKT1(TestShip)']
    assert results['FKT2(TestShip)'] == '7.853982e+00', results['FKT2(TestShip)']
    print("✓ Froude-Krylov module periods test passed!")

def check_froude_krylov_phase_periods(results):
    assert results['FKTp0(TestShip)'] == '6.283185e+00', results['FKTp0(TestShip)']
    assert results['FKTp1(TestShip)'] == '6.981317e+00', results['FKTp1(TestShip)'] 
    assert results['FKTp2(TestShip)'] == '7.853982e+00', results['FKTp2(TestShip)']
    print("✓ Froude-Krylov phase periods test passed!")

def check_froude_krylov_module_psis(results):
    assert results['FKpsi0(TestShip)'] == '1.570796e+00', results['FKpsi0(TestShip)'] 
    assert results['FKpsi1(TestShip)'] == '3.141593e+00', results['FKpsi1(TestShip)']
    print("✓ Froude-Krylov module incidence test passed!")

def check_froude_krylov_phase_psis(results):
    assert results['FKppsi0(TestShip)'] == '1.570796e+00', results['FKppsi0(TestShip)']
    assert results['FKppsi1(TestShip)'] == '3.141593e+00', results['FKppsi1(TestShip)']
    print("✓ Froude-Krylov phase incidence test passed!")

def check_angular_frequencies(results):
    assert results['omega0(TestShip)'] == '4.000000e-01', results['omega0(TestShip)']
    assert results['omega1(TestShip)'] == '5.000000e-01', results['omega1(TestShip)']
    assert results['omega2(TestShip)'] == '6.000000e-01', results['omega2(TestShip)']
    print("✓ Angular frequencies test passed!")

def check_added_mass_coeff(results):
    assert results['Ma000(TestShip)'] == '2.755600e+02', results['Ma000(TestShip)']
    assert results['Ma001(TestShip)'] == '2.714980e+02', results['Ma001(TestShip)']
    assert results['Ma002(TestShip)'] == '2.338420e+02', results['Ma002(TestShip)']
    assert results['Ma110(TestShip)'] == '8.305330e+03', results['Ma110(TestShip)']
    assert results['Ma111(TestShip)'] == '8.924980e+03', results['Ma111(TestShip)']
    assert results['Ma112(TestShip)'] == '9.331230e+03', results['Ma112(TestShip)']
    print("✓ Added mass coefficient test passed!")

def check_radiation_damping_coeff(results):
    assert results['Br_0_0_0(TestShip)']  == '1.744400e+01', results['Br_0_0_0(TestShip)']
    assert results['Br_0_0_6(TestShip)'] == '1.081630e+02', results['Br_0_0_6(TestShip)']
    assert results['Br_5_0_0(TestShip)']  == '1.880490e-03', results['Br_5_0_0(TestShip)']
    assert results['Br_5_0_6(TestShip)'] == '5.640630e-03', results['Br_5_0_6(TestShip)']
    assert results['Br_0_5_0(TestShip)']  == '-1.886460e-04', results['Br_0_5_0(TestShip)']
    assert results['Br_0_5_6(TestShip)'] == '-6.570310e-03', results['Br_0_5_6(TestShip)']
    assert results['Br_5_5_0(TestShip)']  == '1.192720e+04', results['Br_5_5_0(TestShip)']
    assert results['Br_5_5_6(TestShip)'] == '7.054820e+06', results['Br_5_5_6(TestShip)']
    print("✓ Radiation damping coefficient test passed!")


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
    assert results['forwardSpeed(TestShip)'] == '0.000000e+00'
    print("✓ Forward speed test passed!")
    check_added_mass_coeff(results)
    check_radiation_damping_coeff(results)


with open('precal_r_output.csv', newline='') as csvfile:
    csv_reader = csv.DictReader(csvfile)
    results = next(csv_reader)
    check(results)
