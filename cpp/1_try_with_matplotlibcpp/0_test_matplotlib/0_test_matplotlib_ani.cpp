#define _USE_MATH_DEFINES
#include <cmath>
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

int main()
{
	int n = 1000;
	std::vector<double> x, y, z;

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
			plt::pause(0.01);
		}
	}
}

// g++-11 --std=c++11 -W -o animation.out test.cpp -I../lib -I/opt/homebrew/Caskroom/miniforge/base/include/python3.9/ -I/opt/homebrew/Caskroom/miniforge/base/lib/python3.9/site-packages/numpy/core/include/ -L/opt/homebrew/Caskroom/miniforge/base/lib -lpython3.9 -lpthread -rpath/opt/homebrew/Caskroom/miniforge/base/lib

// setenv LD_LIBRARY_PATH "/CAE/linux/anaconda3/lib"
// setenv PYTHONHOME "/CAE/linux/anaconda3/"
// g++ --std=c++11 -W -o animationTest.out test.cpp -Ilib -I/CAE/linux/anaconda3/include/python3.7m/ -I/CAE/linux/anaconda3/lib/python3.7/site-packages/numpy/core/include/ -L/CAE/linux/anaconda3/lib/ -lpython3.7m -lpthread -rpath/CAE/linux/anaconda3/lib