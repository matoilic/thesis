#include "capture.h"

Capture::Capture()
{
    isStopped = true;
}

bool Capture::start()
{
    if (!isStopped)
        return false;

    bool success = capture.open(0);

    if (success) {
        isStopped = false;
        auto threadFunction = std::bind(&Capture::captureLoop, this);
        captureThread = std::thread(threadFunction);
    }

    return success;
}

void Capture::stop()
{
    isStopped = true;
}

void Capture::captureLoop()
{
    bool success;
    cv::Mat frame;

    while (!isStopped) {
        success = capture.read(frame);
        if (!success) {
            stop();
        }
        if (handler) {
            handler(frame);
        }
    }
}

void Capture::setHandler(CaptureHandler handler)
{
    this->handler = handler;
}
