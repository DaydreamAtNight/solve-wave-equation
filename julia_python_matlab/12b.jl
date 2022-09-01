#=
-----------------------------
julia script to simulate the 2D wave function
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
=#
import PyPlot; # matplolib.PyPlot API is used for julia
const plt = PyPlot;
using Dates;

# define parameters
nSteps = 1000;          # Number of timeSteps
Dt = 1e-3;               # Length of a timeStep
C = 15;                  # Stiffness constant
nx = 300;                # Number of x support points
ny = 300;                # Number of y support points
w = 20;                  # Initial wave width

# Domain size (-xmax<x<xmax, -ymax<y<ymax)
xmax = 1;
ymax = 1;

# Initialise the arrays with initial values.
Dx = 2 * xmax / nx;
Dy = 2 * ymax / ny;
x = collect(-xmax:Dx:xmax) .* ones(ny + 1)'; # broadcasting is used to generate 2D array
y = collect(-ymax:Dy:ymax)' .* ones(nx + 1);
r = x .^ 2 + y .^ 2;
v = zeros(size(x));
u = zeros(size(x));
pos = r .< 1;
u[pos] = exp.(-4 * w^2 * r[pos] .^ 2) - 0.75 * exp.(-w^2 * r[pos] .^ 2);
u[pos] = u[pos] .+ 0.75 .* exp.(-w^2) .- exp.(-4 .* w^2);
u[pos] = u[pos] .* (r[pos] .^ 4 .- 1);

# Loop over timesteps
plt.ion();
fig, ax = plt.subplots(figsize=(7, 7), subplot_kw=Dict("projection" => "3d"));
fig.canvas.draw();

global tic = now();
for step in 0:nSteps
    u[2:nx, 2:ny] = u[2:nx, 2:ny] + Dt .* v[2:nx, 2:ny];
    v[2:nx, 2:ny] = v[2:nx, 2:ny] + 
                    (C * Dt / Dx^2) .* (u[1:nx-1, 2:ny] .- 2 .* u[2:nx, 2:ny] + u[3:nx+1, 2:ny]) + 
                    (C * Dt / Dy^2) .* (u[2:nx, 1:ny-1] .- 2 .* u[2:nx, 2:ny] + u[2:nx, 3:ny+1]);
    if step % 5 == 0
        ax.clear();
        ax.plot_surface(x, y, u, cmap=plt.cm.jet, vmin=-0.1, vmax=0.1);
        toc = now();
        ax.text(-1, 1, 0.2, string(floor(1000 / (toc - tic).value)) * " fps", fontsize=12);
        global tic = now();
        ax.set_xlim([-1, 1]);
        ax.set_ylim([-1, 1]);
        ax.set_zlim([-0.2, 0.5]);
        ax.set_xlabel("x");
        ax.set_ylabel("y");
        ax.set_zlabel("u(t,x)");
        plt.pause(0.001);
        fig.canvas.flush_events();
    end
end
# toc = now();
# println(string((toc - tic).value/1000)*" s");

plt.ioff();