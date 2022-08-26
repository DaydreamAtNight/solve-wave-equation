#include <ctime>
#include <chrono>
#include <stack>
#include <iostream>
#include <unistd.h>
using namespace std;

std::stack<clock_t> tictoc_stack;

int main(int argc, char const *argv[])
{
    auto start = std::chrono::system_clock::now();
    sleep(1);
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    cout << "It took "<< elapsed_seconds.count() <<" fps"<< endl;
    return 0;
}
