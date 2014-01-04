#include <Ardoor/Capture/Capture.hpp>

Capture::Capture()
{
    stopped = true;
}

Capture::~Capture()
{
    if (!isStopped()) {
        stop();
        captureThread.wait();
    }
}

bool Capture::start(const std::string &inputFile)
{
    if (!isStopped())
        return false;

    bool success = (!inputFile.empty()) ? capture.open(inputFile) : capture.open(0);

    if (success) {
        stopped = false;

        moveToThread(&captureThread);
        connect(&captureThread, SIGNAL(started()), this, SLOT(captureLoop()));
        //connect(&captureThread, SIGNAL(finished()), &captureThread, SLOT(deleteLater()));
        captureThread.start();
    }

    return success;
}

bool Capture::isStopped()
{
    mutex.lock();
    bool value = stopped;
    mutex.unlock();
    return value;
}

void Capture::stop()
{
    mutex.lock();
    stopped = true;
    mutex.unlock();
}

void Capture::captureLoop()
{
    bool success;
    cv::Mat frame;

    bool doStop = false;
    while (!doStop) {
        success = capture.read(frame);
        if (!success) {
            stop();
            break;
        }
        if (handler && frame.rows > 0 && frame.cols > 0) {
            (*handler)(frame);
        }

        mutex.lock();
        doStop = stopped;
        mutex.unlock();

        QThread::yieldCurrentThread();
    }

    captureThread.quit();
}

void Capture::setHandler(ImageHandler *handler)
{
    this->handler = handler;
}
