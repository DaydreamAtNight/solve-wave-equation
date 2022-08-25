#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

template <class T>
void overwrite_points_data(vector<T> &x, vector<T> &y, vector<vector<T> > &v, string header)
{
  ofstream test_csv("test.csv", ofstream::trunc); // trunc means overwrite
  test_csv << header << endl;
  for (int j=0; j < y.size(); j++)
  {
    for (int i=0; i < x.size(); i++)
      {
        test_csv << x[i] << ", " << y[j] << ", " << v[j][i] << endl;
      }
  }
  test_csv.close();
}

int main(int argc, char const *argv[])
{
  vector<double> x = {1,2,3};
  vector<double> y = {1,2};
  vector<vector<double> > v = {{1,2,3},{4,5,7}};

  output_points_data(x,y,v,"x, y, z");

  return 0;
}

