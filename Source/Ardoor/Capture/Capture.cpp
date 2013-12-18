#include <Ardoor/Capture/Capture.hpp>

Capture::Capture()
{
    isStopped = true;
}

bool Capture::start(const std::string &inputFile)
{
    if (!isStopped)
        return false;

    bool success = (!inputFile.empty()) ? capture.open(inputFile) : capture.open(0);

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
        if (handler && frame.rows > 0 && frame.cols > 0) {
            handler(frame);
        }
    }
}

void Capture::setHandler(CaptureHandler handler)
{
    this->handler = handler;
}
