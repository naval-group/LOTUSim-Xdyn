# Checks Fx is equal to the command read in the CSV file.

import csv

def row_vals_match(row, val):
    """Make sure Xco and Fx in the row match the given value."""
    msg = f"Values of Xco and Fx do not match for t={row['t']}: "
    msg += f"was expecting Xco == Fx == {val}, but got "
    msg += f"Xco = {row['Xco']} and Fx = {row['Fx(F1 TestShip TestShip)']}"
    assert float(row['Xco']) == val and float(row['Fx(F1 TestShip TestShip)']) == val, msg


def check(row):
    """Checks the values in the row mathc the commands in the CSV."""
    t = float(row['t'])
    if t < 0.2:
        return row_vals_match(row, 0)
    if t < 0.26:
        return row_vals_match(row, 23)
    if t < 10.01:
        return row_vals_match(row, 42)
    return row_vals_match(row, 65)



with open('issue_39.out.csv', newline='') as csvfile:
    csv_reader = csv.DictReader(csvfile)
    rows = [row for row in csv_reader]
    assert len(rows)>1,"There should be at least one row of data in the output"
    is_first_row = True
    for row in rows:
        if is_first_row:
            is_first_row = False
            continue
        check(row)

