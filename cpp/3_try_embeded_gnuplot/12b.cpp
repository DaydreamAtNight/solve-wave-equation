#define _USE_MATH_DEFINES
#include <iostream>
#include <vector> 
#include <cmath>
#include "utils.hpp"

using namespace std;

int main()
{
    // define parameters
	int nSteps = 1000;
    double Dt = 0.001;
    double C = 15;
    int nx = 300, ny = 300;
    double w = 20;

    // Domain size (-xmax<x<xmax, -ymax<y<ymax)
    double xmax = 1.0, ymax = 1.0;

    // Initialise the arrays with initial values.
    double Dx = 2 * xmax / nx;
    double Dy = 2 * ymax / ny;

	vector<double> x = linespace( -xmax,  xmax,  nx+1);
	vector<double> y = linespace( -ymax,  ymax,  ny+1);

	vector<vector<double> > X, Y, r, v, u;
	r.assign(y.size(), vector<double>(x.size()));
	v.assign(y.size(), vector<double>(x.size()));
	u.assign(y.size(), vector<double>(x.size()));

    meshgrid(x, y, X, Y);

	// initialization
	for (int j=0; j < ny+1; j++)
    {
    	for (int i=0; i < nx+1; i++)
        {
			r[j][i] = pow(X[j][i],2) + pow(Y[j][i],2);
			if(r[j][i]<1)
			{
				u[j][i] = exp(-4 * pow(w,2) * pow(r[j][i],2)) - 0.75 * exp(-pow(w,2) * pow(r[j][i],2));
				u[j][i] = u[j][i] + 0.75 * exp(-pow(w,2)) - exp(-4 * pow(w,2));
				u[j][i] = u[j][i]*(pow(r[j][i], 4) - 1);
			}
        }
    }

	// Loop over timesteps
    GnuPlotWrapper<double> GPW;
	for (int step = 0; step < nSteps; step++)
	{
		for (int j=1; j < ny; j++)
		{
			for (int i=1; i < nx; i++)
			{
				u[j][i] = u[j][i] + Dt * v[j][i];
			}
		}

		for (int j=1; j < ny; j++)
		{
			for (int i=1; i < nx; i++)
			{
				v[j][i] = v[j][i] + 
				(C*Dt/(pow(Dx,2)))*(u[j][i-1]-2*u[j][i]+u[j][i+1])+ 
				(C*Dt/(pow(Dy,2)))*(u[j-1][i]-2*u[j][i]+u[j+1][i]);
			}
		}

        if (step%5 ==0)
        {
            GPW.update(x,y,u);
            usleep(1);
        }
	}
}

// g++ --std=c++17 -O2  12b.cpp  -o 12b.out -Ilib -I/opt/homebrew/Cellar/boost/1.76.0/include/ -L/opt/homebrew/Cellar/boost/1.76.0/lib -lutil -lboost_iostreams -lboost_system -lboost_filesystem
