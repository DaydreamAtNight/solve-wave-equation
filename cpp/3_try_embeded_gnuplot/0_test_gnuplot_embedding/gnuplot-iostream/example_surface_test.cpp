#include <vector>
#include <cmath>
#include "gnuplot-iostream.h"

template <typename T>
struct MyTriple
{
    MyTriple() : x(0), y(0), z(0) {}
    MyTriple(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}

    T x, y, z;
};

// Tells gnuplot-iostream how to print objects of class MyTriple.
namespace gnuplotio
{
    template <typename T>
    struct BinfmtSender<MyTriple<T>>
    {
        static void send(std::ostream &stream)
        {
            BinfmtSender<T>::send(stream);
            BinfmtSender<T>::send(stream);
            BinfmtSender<T>::send(stream);
        }
    };

    template <typename T>
    struct BinarySender<MyTriple<T>>
    {
        static void send(std::ostream &stream, const MyTriple<T> &v)
        {
            BinarySender<T>::send(stream, v.x);
            BinarySender<T>::send(stream, v.y);
            BinarySender<T>::send(stream, v.z);
        }
    };

    template <typename T>
    struct TextSender<MyTriple<T>>
    {
        static void send(std::ostream &stream, const MyTriple<T> &v)
        {
            TextSender<T>::send(stream, v.x);
            stream << " ";
            TextSender<T>::send(stream, v.y);
            stream << " ";
            TextSender<T>::send(stream, v.z);
        }
    };
} // namespace gnuplotio

MyTriple<double> get_point(double &x, double &y, double &z)
{
    return MyTriple<double>(x, y, z);
}

void test_static_surface()
{
    Gnuplot gp;
    std::vector<double> x = {1, 2, 3};
    std::vector<double> y = {1, 2};
    std::vector<std::vector<double>> v = {{1, 2, 3}, {1, 3, 3}};
    int num_v = x.size() * y.size();

    gp << "set zrange [0:4]\n";
    gp << "set hidden3d nooffset\n";

    auto plots = gp.splotGroup();
    {
        std::vector<std::tuple<
            std::vector<double>,
            std::vector<double>,
            std::vector<double>>>
            pts;

        std::vector<double> x_pts(num_v);
        std::vector<double> y_pts(num_v);
        std::vector<double> z_pts(num_v);
        for (long unsigned int j = 0; j < y.size(); j++)
        {
            for (long unsigned int i = 0; i < x.size(); i++)
            {
                x_pts[i + j * y.size()] = get_point(x[i], y[j], v[j][i]).x;
                y_pts[i + j * y.size()] = get_point(x[i], y[j], v[j][i]).y;
                z_pts[i + j * y.size()] = get_point(x[i], y[j], v[j][i]).z;
            }
        }
        pts.emplace_back(x_pts, y_pts, z_pts);
        plots.add_plot2d(pts, "with lines title 'vec of std::tuple of vec'");
    }

    gp << plots;
    return;
}

void test_ani_surface()
{
    std::vector<double> x = {1, 2, 3};
    std::vector<double> y = {1, 2};
    std::vector<std::vector<double>> v = {{1, 2, 3}, {4, 5, 6}};

    Gnuplot gp;
    // gp << "set zrange [0:4]\n";
    gp << "set hidden3d nooffset\n";

    std::vector<MyTriple<double>> pts;
    for (int step = 0; step < 10; step++)
    {
        pts.erase(pts.begin(), pts.end());
        for (long unsigned int i = 0; i < x.size(); i++)
        {
            for (long unsigned int j = 0; j < y.size(); j++)
            {
                pts.push_back(MyTriple<double>(x[i], y[j], v[j][i]));
                // std::cout<<x[i]<<" "<<y[j]<<" "<<v[j][i]<< std::endl;
            }
        }
        gp << "splot '-' binary" << gp.binfmt(pts, "record") << "with lines notitle\n";
        gp.sendBinary(pts);
        gp.flush();

        v[0][0] += 2;
        sleep(1);
    }

    return;
}

void test_ani_surface_pm3d()
{
    std::vector<double> x = {1, 2, 3};
    std::vector<double> y = {1, 2};
    std::vector<std::vector<double>> v = {{1, 2, 3}, {4, 5, 6}};

    Gnuplot gp;
    // gp << "set zrange [0:4]\n";
    gp << "set hidden3d nooffset\n";
    gp << "set pm3d\n";

    std::vector<std::vector<MyTriple<std::string>>> pts;
    for (int step = 0; step < 10; step++)
    {
        pts.erase(pts.begin(), pts.end());
        for (long unsigned int i = 0; i < x.size(); i++)
        {
            std::vector<MyTriple<std::string>> segment;
            for (long unsigned int j = 0; j < y.size(); j++)
            {
                segment.push_back(MyTriple<std::string>(std::to_string(x[i]), std::to_string(y[j]), std::to_string(v[j][i])));
            }
            pts.push_back(segment);
        }
        gp << "splot '-' notitle\n";
        gp.send2d(pts);
        gp.flush();

        v[0][0] += 2;
        sleep(1);
    }

    return;
}

int main()
{
    // test_static_surface();
    // test_ani_surface();
    test_ani_surface_pm3d();
    return 0;
}

// in mac, compile with
// g++ --std=c++17 -O2  example_surface_test.cpp  -o example_surface_test -L/opt/homebrew/Cellar/boost/1.76.0/lib -I/opt/homebrew/Cellar/boost/1.76.0/include/ -lutil -lboost_iostreams -lboost_system -lboost_filesystem

// in linux, compile with makefile