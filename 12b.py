'''
-----------------------------
python script to simulate the 2D wave function
-----------------------------
feature:
   solve the two-dimensional wave equation 
   u_tt = C*(u_xx + u_yy)
-----------------------------
parameters:
    Steps:  Number of timeSteps
    Dt:     Length of a timeStep
    C:      Stiffness constant
    nx      Number of x support points
    ny      Number of y support points
    w:      Initial wave width
output:
    animated pic
-----------------------------
notes:
    Central finite difference method is used
-----------------------------
Author: Songrui LI
Date: 2021.9.26
-----------------------------
'''
import matplotlib.pyplot as plt
import numpy as np
import time

# define parameters
nSteps = 1000  # Number of timesteps
Dt = 1e-3  # Length of a timestep
C = 15  # Stiffness constant
nx = 300  # Number of x support points
ny = 300  # Number of y support points
w = 20  # Initial wave width

# Domain size (-xmax<x<xmax, -ymax<y<ymax)
xmax = 1
ymax = 1

# Initialise the arrays with initial values.
Dx = 2 * xmax / nx
Dy = 2 * ymax / ny
[y, x] = np.meshgrid(np.arange(-ymax, ymax + Dy, Dy),
                     np.arange(-xmax, xmax + Dx, Dx))
r = np.square(x) + np.square(y)
v = np.zeros(np.shape(x))
u = np.zeros(np.shape(x))
pos = (r < 1)
u[pos] = np.exp(
    -4 * w**2 * np.square(r[pos])) - 0.75 * np.exp(-w**2 * np.square(r[pos]))
u[pos] = u[pos] + 0.75 * np.exp(-w**2) - np.exp(-4 * w**2)
u[pos] = u[pos] * (np.power(r[pos], 4) - 1)

# Loop over timesteps
plt.ion()
fig, ax = plt.subplots(figsize=(7, 7), subplot_kw={"projection": "3d"})
fig.canvas.draw()

tic = time.perf_counter()
for step in range(nSteps):
    # Compute the new velocity v and solution u using array operations
    # Only operate on internal points since boundary points are fixed to
    # zero velocity and u
    u[1:-1, 1:-1] = u[1:-1, 1:-1] + Dt * v[1:-1, 1:-1]
    v[1:-1,1:-1]  = v[1:-1,1:-1]+ \
                (C*Dt/(Dx**2))*( u[0:-2,1:-1] - 2.*u[1:-1,1:-1] + u[2:,1:-1] )+ \
                (C*Dt/(Dx**2))*( u[1:-1,0:-2] - 2.*u[1:-1,1:-1] + u[1:-1,2:] )

    if step % 5 == 0:
        ax.clear()
        ax.plot_surface(x, y, u, cmap=plt.cm.winter,vmin=-0.1, vmax=0.1)
        toc = time.perf_counter()
        ax.text(-1, 1, 0.2, str(np.floor(1/(toc-tic)))+" fps",fontsize=12)
        tic = time.perf_counter()
        ax.set_xlim([-1, 1])
        ax.set_ylim([-1, 1])
        ax.set_zlim([-0.2, 0.5])
        ax.set_xlabel('x')
        ax.set_ylabel('y')
        ax.set_zlabel('u(t,x)')
        plt.pause(0.001)
        fig.canvas.flush_events()

# toc = time.perf_counter()
# print(str(toc - tic)+" s")

plt.ioff()
