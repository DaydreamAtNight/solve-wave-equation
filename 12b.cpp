#define _USE_MATH_DEFINES
#include <vector> 
#include <cmath>
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;
using namespace std;

int main()
{
    // define parameters
	int nSteps = 1000;
    double Dt = 1e-3;
    int C = 15;
    int nx = 300, ny = 300;;
    double w = 20

    // Domain size (-xmax<x<xmax, -ymax<y<ymax)
    double xmax = 1.0, ymax = 1.0;

    // Initialise the arrays with initial values.
    double Dx = 2 * xmax / nx;
    double Dy = 2 * ymax / ny;



	vector<vector<int>> x, y, u

	for(int i=0; i<n; i++) {
		x.push_back(i*i);
		y.push_back(sin(2*M_PI*i/360.0));
		z.push_back(log(i));

		if (i % 10 == 0) {
			// Clear previous plot
			plt::clf();
			// Plot line from given x and y data. Color is selected automatically.
			plt::plot(x, y);
			// Plot a line whose name will show up as "log(x)" in the legend.
			plt::named_plot("log(x)", x, z);

			// Set x-axis to interval [0,1000000]
			plt::xlim(0, n*n);

			// Add graph title
			plt::title("Sample figure");
			// Enable legend.
			plt::legend();
			// Display plot continuously
			plt::pause(0.00001);
		}
	}
}

// g++-11 --std=c++11 -W -o animation.out animation.cpp -I../lib -I/opt/homebrew/Caskroom/miniforge/base/include/python3.9/ -I/opt/homebrew/Caskroom/miniforge/base/lib/python3.9/site-packages/numpy/core/include/ -L/opt/homebrew/Caskroom/miniforge/base/lib -lpython3.9 -lpthread -rpath/opt/homebrew/Caskroom/miniforge/base/lib