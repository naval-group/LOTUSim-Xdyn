# Checks filtered states (with duration 0) are equal to non-filtered states.
# This ensures that:
# - the output section concerning filtered states is properly parsed
# - the filtered states are properly initialized
# - there is no time shift or off-by-one errors

import csv

with open('filtered_states.csv', newline='') as csvfile:
    csv_reader = csv.DictReader(csvfile)
    read_one_line = False
    for row in csv_reader:
        # Skip first line
        if read_one_line:
            assert row['x(TestShip)'] == row['x_filtered(TestShip)']
            assert row['z(TestShip)'] != row['z_filtered(TestShip)']
        read_one_line = True
    assert read_one_line
