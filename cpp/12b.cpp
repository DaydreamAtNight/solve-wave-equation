#define _USE_MATH_DEFINES
#include <vector> 
#include <algorithm>
#include <cmath>
#include <iostream>

using namespace std;

template <class T>
void print_vector_1D(vector<T> &v, string v_name);

template <class T>
void print_vector_2D(vector<vector<T> > &v, string v_name);

vector<double> linespace(double start, double ed, int num);

void meshgrid(vector<double> xin, vector<double> yin, vector<vector<double>> &xout, vector<vector<double>> &yout);

template <class T>
void output_points_data(vector<T> &x, vector<T> &y, vector<vector<T> > &v, string message);

int main()
{
    // define parameters
	int nSteps = 100;
    double Dt = 0.001;
    double C = 15;
    double nx = 300.0, ny = 300.0;
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
	for (int j=0; j < ny; j++)
    {
    	for (int i=0; i < nx; i++)
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
	for (int step = 0; step < nSteps; step++)
	{
		for (int j=1; j < ny-1; j++)
		{
			for (int i=1; i < nx-1; i++)
			{
				u[j][i] = u[j][i] + Dt * v[j][i];
			}
		}

		for (int j=1; j < ny-1; j++)
		{
			for (int i=1; i < nx-1; i++)
			{
				v[j][i] = v[j][i] + 
				(C*Dt/(pow(Dx,2)))*(u[j][i-1]-2*u[j][i]+u[j][i+1])+ 
				(C*Dt/(pow(Dy,2)))*(u[j-1][i]-2*u[j][i]+u[j+1][i]);
			}
		}
	}
	output_points_data<double>(x, y, u, "x, y, v");
}

template <class T>
void print_vector_1D(vector<T> &v, string v_name)
{
    cout << v_name << " = " << endl;
    // for (auto it = v.begin(); it != v.end(); it++) // works in c++11
    for (typename vector<T>::iterator it = v.begin(); it != v.end(); it++)
    {
        cout << *it << " ";
    }
    cout << endl;
}

template <class T>
void print_vector_2D(vector<vector<T> > &v, string v_name)
{
    cout << v_name << " = " << endl;
    for (typename vector<vector<T> >::iterator itx = v.begin(); itx != v.end(); itx++)
    {
        for (typename vector<T>::iterator ity = itx->begin(); ity != itx->end(); ity++)
        {
            cout << *ity << ", ";
        }
        cout << endl;
    }
    cout << endl;
}

vector<double> linespace(double start, double ed, int num) 
{
    // catch rarely, throw often
    if (num < 2) {
        throw new exception();
    }
    int partitions = num - 1;
    vector<double> pts;
    // length of each segment    
    double length = (ed - start) / partitions; 
    // first, not to change
    pts.push_back(start);
    for (int i = 1; i < num - 1; i ++) {
        pts.push_back(start + i * length);
    }
    // last, not to change
    pts.push_back(ed);
    return pts;
}

template <class T>
void output_points_data(vector<T> &x, vector<T> &y, vector<vector<T> > &v, string message)
{
    cout << message << endl;
    for (int j=0; j < y.size(); j++)
    {
    	for (int i=0; i < x.size(); i++)
        {
        	cout << x[i] << ", " << y[j] << ", " << v[j][i] << endl;
        }
    }
}


void meshgrid(vector<double> xin, vector<double> yin, vector<vector<double>> &xout, vector<vector<double>> &yout)
{
	int nx=xin.size();
	int ny=yin.size();

	xout.assign(ny, vector<double>(nx, 0));
	yout.assign(ny, vector<double>(nx, 0));
	
	for (int j=0; j < ny; j++)
    {
    	for (int i=0; i < nx; i++)
        {
        	xout[j][i]=xin[i];
        	yout[j][i]=yin[j];
        }
    }
}


// g++-11 --std=c++11 -W -o 12b.out 12b.cpp