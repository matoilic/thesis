#ifndef CALIBRATIONPROCESSOR_HPP
#define CALIBRATIONPROCESSOR_HPP

#include <QObject>
#include <QtGui/QWindow>

#include <ctime>

#include <Ardoor/Context/ArdoorContext.hpp>
#include <Ardoor/Capture/Capture.hpp>
#include <Ardoor/CameraCalib/CameraCalib.hpp>
#include <Ardoor/Rendering/ArRenderingContext.hpp>

#define CALIB_NUM_IMAGES 100

class CalibrationProcessor : public QObject, public ImageHandler
{
private:
    Q_OBJECT

public:
    CalibrationProcessor(ArdoorContext *ardoorContext, ArRenderingContext *renderingContext, QWindow *window);
    ~CalibrationProcessor();

    virtual void operator() (cv::Mat &frame);
    void start();

signals:
    void finished();

private:
    void doCalibration();
    void drawStatus(cv::Mat &frame);

    Capture capture;
    ArdoorContext *ardoorContext;
    ArRenderingContext *renderingContext;
    QWindow *window;
    CameraCalib *calib;
    int numOfDetections;
    cv::Size imageSize;
    clock_t prevTimestamp;

};

#endif // CALIBRATIONPROCESSOR_HPP
