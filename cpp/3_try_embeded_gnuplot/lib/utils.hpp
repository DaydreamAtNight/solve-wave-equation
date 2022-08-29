#include <ctime>
#include <chrono>
#include "gnuplot-support.cpp"
using namespace std;

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
void print_vector_2D(vector<vector<T>> &v, string v_name)
{
    cout << v_name << " = " << endl;
    for (typename vector<vector<T>>::iterator itx = v.begin(); itx != v.end(); itx++)
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
    if (num < 2)
    {
        throw new exception();
    }
    int partitions = num - 1;
    vector<double> pts;
    // length of each segment
    double length = (ed - start) / partitions;
    // first, not to change
    pts.push_back(start);
    for (int i = 1; i < num - 1; i++)
    {
        pts.push_back(start + i * length);
    }
    // last, not to change
    pts.push_back(ed);
    return pts;
}

void meshgrid(vector<double> xin, vector<double> yin, vector<vector<double>> &xout, vector<vector<double>> &yout)
{
    int nx = xin.size();
    int ny = yin.size();

    xout.assign(ny, vector<double>(nx, 0));
    yout.assign(ny, vector<double>(nx, 0));

    for (int j = 0; j < ny; j++)
    {
        for (int i = 0; i < nx; i++)
        {
            xout[j][i] = xin[i];
            yout[j][i] = yin[j];
        }
    }
}

inline string time_diff(chrono::duration<double> elapsed_seconds)
{
    stringstream stream;
    stream << fixed << setprecision(2) << 1 / elapsed_seconds.count();
    return stream.str();
}

template <class T>
class GnuPlotWrapper
{
public:
    GnuPlotWrapper()
    {
        init_style();
        start = chrono::system_clock::now();
    }

    inline void init_style()
    {
        gp << "set xyplane at -0.2\n";
        gp << "set zrange [-0.2:0.5]\n";
        gp << "set cbrange [-0.1:0.13]\n";
        gp << "set palette defined ( 0 '#000090'" \
           << ", 1 '#000fff', 2 '#0090ff', 3 '#0fffee', "\
           << "4 '#90ff70', 5 '#ffee00', 6 '#ff7000', "\
           << "7 '#ee0000', 8 '#7f0000')\n";
        // gp << "set xrange [-1:1]\n";
        // gp << "set yrange [-1:1]\n";
        gp << "set hidden3d nooffset\n";
        gp << "set pm3d\n";
    }

    void update(vector<T> &x, vector<T> &y, vector<vector<T>> &u, int step = 5)
    {
        pts_reset();
        for (auto i = 0; i < x.size(); i += step)
        {
            this->segment_reset();
            for (auto j = 0; j < y.size(); j += step)
            {
                this->seg_push_back(x[i], y[j], u[j][i]);
            }
            this->pts_push_back();
        }
        this->plot();
    }

private:
    inline void pts_reset()
    {
        pts.erase(pts.begin(), pts.end());
    }

    inline void segment_reset()
    {
        segment.erase(segment.begin(), segment.end());
    }

    inline void seg_push_back(T &x, T &y, T &z)
    {
        segment.push_back(MyTriple<T>(x, y, z));
    }

    inline void pts_push_back()
    {
        pts.push_back(segment);
    }

    inline void plot()
    {
        gp << "splot '-' w pm3d notitle\n";
        gp.send2d(pts);
        auto end = chrono::system_clock::now();
        gp << "set label 1 '" << time_diff(end - start) \
           << " fps' at -0.8,0,0.3\n";
        start = chrono::system_clock::now();
        gp.flush();
    }

private:
    Gnuplot gp;
    vector<vector<MyTriple<T>>> pts;
    vector<MyTriple<T>> segment;
    chrono::system_clock::time_point start;
};