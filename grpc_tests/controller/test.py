import csv
import math

with open('tutorial_11.csv', newline='') as csvfile:
    csv_reader = csv.DictReader(csvfile)
    t = []
    psi = []
    for row in csv_reader:
        t = float(row['t'])
        psi_in_deg = float(row['psi(dtmb)'])*180/math.pi
        # Should converge to 30° before changing setpoint at t=250s
        if t > 200 and t < 250:
            assert abs(psi_in_deg - 30) < 0.25
        # Should converge to 456° at the end of the simulation
        if t > 430:
            assert abs(psi_in_deg - 45) < 0.25

