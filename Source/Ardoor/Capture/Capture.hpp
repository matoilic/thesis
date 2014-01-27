#ifndef CAPTURE_H
#define CAPTURE_H

#include <string>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/video.hpp>
#include <QThread>
#include <QMutex>
#include <Ardoor/Ardoor.hpp>
#include <Ardoor/Capture/ImageHandler.hpp>

#define CAPTURE_MAX_FRAME_WIDTH 600

class ARD_EXPORT Capture : private QObject
{
private:
    Q_OBJECT

    cv::VideoCapture capture;
    ImageHandler *handler;
    QThread captureThread;
    bool stopped;
    QMutex mutex;

private slots:
    void captureLoop();

public:
    Capture();
    ~Capture();

    bool start(const std::string &inputFile = "");
    bool startImageFile(std::string imageFile);
    void stop();
    bool isStopped();

    void setHandler(ImageHandler *handler);
};

#endif // CAPTURE_H
