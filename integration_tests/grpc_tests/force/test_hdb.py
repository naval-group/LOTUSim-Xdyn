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


def check(results):
    """Make sure the results contain all necessary data."""
    check_added_mass_matrix(results)


with open('hdb_output.csv', newline='') as csvfile:
    csv_reader = csv.DictReader(csvfile)
    results = next(csv_reader)
    check(results)
