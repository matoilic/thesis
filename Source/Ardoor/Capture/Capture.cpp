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
        capture.set(CV_CAP_PROP_CONVERT_RGB, 1);

        //scale large inputs down, since we don't need highres images
        cv::Mat frame;
        int scaledWidth, scaledHeight;
        success = capture.read(frame);
        if(success && frame.cols > CAPTURE_MAX_FRAME_WIDTH) {
            float factor = CAPTURE_MAX_FRAME_WIDTH / (float)frame.cols;
            scaledWidth = frame.cols * factor;
            scaledHeight = frame.rows * factor;
            capture.set(CV_CAP_PROP_FRAME_WIDTH, scaledWidth);
            capture.set(CV_CAP_PROP_FRAME_HEIGHT, scaledHeight);
        }

        stopped = false;

        moveToThread(&captureThread);
        connect(&captureThread, SIGNAL(started()), this, SLOT(captureLoop()));
        //connect(&captureThread, SIGNAL(finished()), &captureThread, SLOT(deleteLater()));
        captureThread.start();
    }

    return success;
}

bool Capture::startImageFile(string imageFile)
{
    cv::Mat frame = cv::imread(imageFile);

    if (handler && frame.rows > 0 && frame.cols > 0) {
        (*handler)(frame);
    }

    return true;
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
    bool success, doStop = false;
    cv::Mat frame;

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
