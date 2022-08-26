#include <iostream>
#include <boost/timer.hpp>
using namespace std;

int main()
{
    boost::time t;
    cout << "now time is " << t.elapsed() << endl;

    return 0;
}