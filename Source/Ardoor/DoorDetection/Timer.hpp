#ifndef TIMER_H
#define TIMER_H

#if defined(DEBUG) && !defined(WIN32)

#include <iostream>
#include <string>
#include <chrono>
#include <ctime>

class Timer
{
    std::chrono::time_point<std::chrono::system_clock> _start;
    std::chrono::time_point<std::chrono::system_clock> _end;
public:
    Timer();
    void start();
    void stop();
    void show(std::string label);
    void reset();
};

#endif

#endif // TIMER_H
