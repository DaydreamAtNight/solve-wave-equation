#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

template <class T>
void print_vector_1D(vector<T> &v, string v_name);

template <class T>
void print_vector_2D(vector<vector<T> > &v, string v_name);

vector<double> linespace(double start, double ed, int num);

void meshgrid(vector<double> xin, vector<double> yin, vector<vector<double>> &xout, vector<vector<double>> &yout);

void test_linespace()
{
    vector<double> x = {1,2,3,4,5};
    vector<double> x_linespace = linespace(0.0, 4.0, 5);
    vector <vector<double> > xy = {{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5}};
    vector <vector<double> > xy_init(3, vector<double>(2));
    print_vector_1D(x, "x");
    print_vector_1D(x_linespace, "x_linespace");
    print_vector_2D<double>(xy, "xy");
}

void test_meshgrid()
{
    vector<double> x = linespace(0.0, 4.0, 5);
    vector<double> y = linespace(0.0, 4.0, 5);
    vector<vector<double> > xout, yout;
    meshgrid(x, y, xout, yout);
    print_vector_2D<double>(xout,"xout");
    print_vector_2D<double>(yout,"yout");
}

int main()
{
    test_linespace();
    test_meshgrid();

    return 0;
}

template <class T>
void print_vector_1D(vector<T> &v, string v_name)
{
    cout << v_name << " = " << endl;
    for_each(v.begin(), v.end(), [](T element){
        cout << element << " ";
    });
    cout << endl;
}

template <class T>
void print_vector_2D(vector<vector<T> > &v, string v_name)
{
    cout << v_name << " = " << endl;
    traverse_2D(v, [](T element){
        cout << element << " ";
    });
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

template <class T>
void traverse_2D(vector<vector<T>> &v, void func(double))
{
    for (typename vector<vector<T> >::iterator itx = v.begin(); itx != v.end(); itx++)
    {
        for (typename vector<T>::iterator ity = itx->begin(); ity != itx->end(); ity++)
        {
            func(*ity);
        }
        cout << endl;
    }
    cout << endl;
}
