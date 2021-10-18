# Checks forces and torques are outputted in the right frame.
#
# The resistance curve force in issue_202.yml is defined at the hydrodynamic
# forces calculation point in body frame. This point is at the origin of the
# BODY frame and the centre of inertia is ar (0,0,1). Fy, Fz, Mx, My & Mz
# should all be 0. Issue 202 was created because My was 1 (that is M = r x F
# with r the vector between COG and O (that is (0, 0, -1) and F the resistance
# force (that is (-1, 0, 0)). This meant that the forces were expressed in the
# COG reference frame, not in the body reference frame.

import csv


with open('issue_202.csv', newline='') as csvfile:
    csv_reader = csv.DictReader(csvfile)
    read_one_line = False
    for row in csv_reader:
        # Skip first line
        if read_one_line:
            assert abs(float(row['Fx(resistance curve TestShip TestShip)']) + 1) == 0, f"Fx should be equal to -1 N, but it's not: it's equal to {row['Fx(resistance curve TestShip TestShip)']} N."
            assert abs(float(row['Fy(resistance curve TestShip TestShip)'])) == 0, f"Fy should be equal to 0, but it's not: it's equal to {row['Fy(resistance curve TestShip TestShip)']}"
            assert abs(float(row['Fz(resistance curve TestShip TestShip)'])) == 0, f"Fz should be equal to 0, but it's not: it's equal to {row['Fz(resistance curve TestShip TestShip)']}"
            assert abs(float(row['Mx(resistance curve TestShip TestShip)'])) == 0, f"Mx should be equal to 0, but it's not: it's equal to {row['Mx(resistance curve TestShip TestShip)']}"
            assert abs(float(row['My(resistance curve TestShip TestShip)'])) == 0, f"My should be equal to 0, but it's not: it's equal to {row['My(resistance curve TestShip TestShip)']}"
            assert abs(float(row['Mz(resistance curve TestShip TestShip)'])) == 0, f"Mz should be equal to 0, but it's not: it's equal to {row['Mz(resistance curve TestShip TestShip)']}"
        read_one_line = True
    assert read_one_line
