#ifndef CAPTURE_H
#define CAPTURE_H

#include <functional>
#include <thread>
#include "opencv2/highgui/highgui.hpp"

#include "ardoor.h"

typedef std::function<void(cv::Mat&)> CaptureHandler;

class ARD_EXPORT Capture
{
private:
    cv::VideoCapture capture;
    CaptureHandler handler;
    std::thread captureThread;
    bool isStopped;

    void captureLoop();

public:
    Capture();

    bool start();
    void stop();

    void setHandler(CaptureHandler handler);
};

#endif // CAPTURE_H
