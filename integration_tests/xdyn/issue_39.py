# Checks Fx is equal to the command read in the CSV file.

import csv


def row_vals_match(row, val):
    """Make sure Xco and Fx in the row match the given value."""
    msg = f"Values of Xco and Fx do not match for t={row['t']}: "
    msg += f"was expecting Xco == Fx == {val}, but got "
    msg += f"Xco = {row['Xco']} and Fx = {row['Fx(F1 TestShip TestShip)']}"
    assert float(row['Xco']) == val and float(row['Fx(F1 TestShip TestShip)']) == val, msg


def check(row, dates):
    """Checks the values in the row match the commands in the CSV."""
    t = float(row['t'])
    if t < dates[0]:
        return row_vals_match(row, 0)
    if t < dates[1]:
        return row_vals_match(row, 23)
    if t < dates[2]:
        return row_vals_match(row, 42)
    return row_vals_match(row, 65)


without_shift = [0.2, 0.26, 10.01]
with_shift = [0, 0.06, 9.81]

with open('issue_39.out.with_shift.csv', newline='') as csvfile:
    csv_reader = csv.DictReader(csvfile)
    rows = [row for row in csv_reader]
    assert len(rows) > 1, "There should be at least one row of data in output"
    is_first_row = True
    for row in rows:
        if is_first_row:
            is_first_row = False
            continue
        check(row, with_shift)


with open('issue_39.out.without_shift.csv', newline='') as csvfile:
    csv_reader = csv.DictReader(csvfile)
    rows = [row for row in csv_reader]
    assert len(rows) > 1, "There should be at least one row of data in output"
    is_first_row = True
    for row in rows:
        if is_first_row:
            is_first_row = False
            continue
        check(row, without_shift)
