% wave2d.m
%
% Solve the 2D wave equation
%
% u_tt = C*(u_xx + u_yy)
%
% using an explicit Euler method for time integration and array operations
% as much as possible.

% housekeeping
clear
close all

% Parameters, to be set by the user
nSteps = 1000;            % Number of timesteps
Dt = 1e-3;                % Length of a timestep
C = 15;                   % Stiffness constant
nx = 200;                 % Number of support points in x
ny = 200;                 % Number of support points in y
w = 20;                   % Initial wave width

% Domain size (-xmax<x<xmax, -ymax<y<ymax)
xmax = 1;
ymax = 1;

% Initialise the arrays with initial values.
% Use vectorized operations and meshgrid. Create y before x to take into
% accound the order used by meshgrid, and ease plotting later.
Dx = 2*xmax/nx;
Dy = 2*ymax/ny;
[y,x] = meshgrid(-ymax:Dy:ymax,-xmax:Dx:xmax);
r = x.^2 + y.^2;
v = zeros(size(x));
u = zeros(size(x));
pos = (r<1);
u(pos) = exp(-4*w^2*r(pos).^2) - 0.75*exp(-w^2*r(pos).^2);
u(pos) = u(pos) + 0.75*exp(-w^2) - exp(-4*w^2);
u(pos) = u(pos).*(r(pos).^4-1);

% Loop over timesteps
tic
for step = 0:nSteps
    % Compute the new velocity v and solution u using array operations
    % Only operate on internal points since boundary points are fixed to
    % zero velocity and u
    u(2:nx,2:ny) = u(2:nx,2:ny) + Dt.*v(2:nx,2:ny);
    v(2:nx,2:ny) = v(2:nx,2:ny) ...
        +(C*Dt/Dx^2).*( u(1:nx-1,2:ny) - 2.*u(2:nx,2:ny) + u(3:nx+1,2:ny) ) ...
        +(C*Dt/Dy^2).*( u(2:nx,1:ny-1) - 2.*u(2:nx,2:ny) + u(2:nx,3:ny+1) );
    
    % Plot every 5 steps, otherwise too slow
    if (mod(step,5)==0)
        surf(x(1:5:end,1:5:end),y(1:5:end,1:5:end),u(1:5:end,1:5:end))
        colormap winter
        % shading interp
        % light
        axis equal;
        xlabel('x')
        ylabel('y')
        zlabel('u(t,x,y)')
        caxis([-.2, .2])
        axis([-xmax, xmax, -ymax ymax -0.2 0.5])
        hold on
        text(.8, 1, 0.2, strcat(num2str(floor(1/toc)), " fps"), 'FontSize', 12)
        tic
        hold off
        drawnow
    end
end
