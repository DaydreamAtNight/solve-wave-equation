#include <vector>
#include <cmath>
#include "gnuplot-support.cpp"

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

int main()
{
    // test_static_surface();
    test_ani_surface();
    return 0;
}

// in mac, compile with
// g++ --std=c++17 -O2  test_lib.cpp  -o test_lib -Ilib -I/opt/homebrew/Cellar/boost/1.76.0/include/ -L/opt/homebrew/Cellar/boost/1.76.0/lib -lutil -lboost_iostreams -lboost_system -lboost_filesystem

// in linux, compile with makefile