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