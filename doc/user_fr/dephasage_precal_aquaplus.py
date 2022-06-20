import matplotlib.pyplot as plt
import numpy as np
from numpy import cos, pi, sin

Tp = 4
omega = 2*pi/Tp
phi = pi/4
t = np.arange(0, 2*Tp, 0.1)   # start,stop,step
aqua_plus_wave_elevation = -sin(omega*t)
aqua_plus_response = -2*sin(omega*t + phi)
precal_r_wave_elevation = cos(omega*t)
precal_r_wave_response = 2*cos(omega*t + phi)

plt.plot(t, aqua_plus_wave_elevation, 'r-')
plt.plot(t, aqua_plus_response, 'r--')
plt.plot(t, precal_r_wave_elevation, 'b-')
plt.plot(t, precal_r_wave_response, 'b--')
plt.xlabel('Temps (s)')
plt.title('Déphasage entre PRECAL_R et AQUA+')
plt.legend(['Elévations AQUA+', 'Réponse AQUA+', 'Elévations PRECAL_R', 'Réponse PRECAL_R'])
plt.arrow(2.5, 1, 0.5, 0, shape='left', color='red', linestyle='-')
plt.axvline(x=2.5, color='red', linestyle='--', lw=1)
plt.arrow(3.5, 1, 0.5, 0, shape='left', color='blue', linestyle='-')
plt.axvline(x=3.5, color='blue', linestyle='--', lw=1)
plt.text(2.65, 1.1, r'$\varphi$')
plt.text(3.65, 1.1, r'$\varphi$')
plt.show()
