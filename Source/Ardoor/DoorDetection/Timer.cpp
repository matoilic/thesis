#include "Timer.hpp"
#include <string>

#if defined(DEBUG) && !defined(WIN32)

using namespace std;

Timer::Timer()
{
}

void Timer::start()
{
    _start = std::chrono::system_clock::now();
}

void Timer::stop()
{
    _end = std::chrono::system_clock::now();
}

void Timer::show(std::string label)
{
    //double elapsed = std::chrono::duration_cast<std::chrono::microseconds>(_end - _start).count();
    //cout << label << ": " << elapsed << "microseconds" << endl << flush;
}

#endif
