#include "CalibrationProcessor.hpp"
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/core/core.hpp>
#include <QApplication>

CalibrationProcessor::CalibrationProcessor(ArdoorContext *ardoorContext, ArRenderingContext *renderingContext, QWindow *window)
    : ardoorContext(ardoorContext)
    , renderingContext(renderingContext)
    , window(window)
    , prevTimestamp(0)
    , numOfDetections(0)
{
    cv::Size size = ardoorContext->getChessboardSize();
    calib = new CameraCalib(size);
}

CalibrationProcessor::~CalibrationProcessor()
{
    delete calib;
}

void CalibrationProcessor::operator() (cv::Mat &frame)
{
    if (prevTimestamp == 0) {
        window->resize(frame.cols, frame.rows);
    }

    std::vector<cv::Point2f> imageCorners;
    std::vector<cv::Point3f> objectCorners;

    clock_t delay = 1000;

    if (clock() - prevTimestamp > delay) {
        if (calib->findChessboardPoints(frame, imageCorners, objectCorners)) {
            imageSize = frame.size();

            calib->addPoints(imageCorners, objectCorners);
            //cv::drawChessboardCorners(frame, imageSize, imageCorners, true);

            numOfDetections++;
        }
        prevTimestamp = clock();
    }

    drawStatus(frame);

    std::vector<PoseEstimationResult> emptyResult;
    renderingContext->update(frame, emptyResult);

    if (numOfDetections >= CALIB_NUM_IMAGES) {
        doCalibration();
    }
}

void CalibrationProcessor::start()
{
    capture.setHandler(this);
    capture.start();
}

void CalibrationProcessor::doCalibration()
{
    capture.stop();

    cv::Mat_<float> m;
    std::vector<float> d;

    calib->calibrate(imageSize);
    calib->getIntrinsicsMatrix().convertTo(m, CV_32F);
    calib->getDistortionCoeffs().copyTo(d);

    float fx = m.at<float>(0, 0);
    float fy = m.at<float>(1, 1);
    float cx = m.at<float>(0, 2);
    float cy = m.at<float>(1, 2);

    CameraConfiguration camera(fx, fy, cx, cy, imageSize.width, imageSize.height, d);
    ardoorContext->setCameraConfiguration(camera);
    ardoorContext->saveSettings();

    emit finished();
}

void CalibrationProcessor::drawStatus(cv::Mat &frame)
{
    std::stringstream str;
    str << numOfDetections << " / " << CALIB_NUM_IMAGES;

    cv::putText(frame, str.str(), cv::Point(60, 60), cv::FONT_HERSHEY_DUPLEX, 0.8, cv::Scalar(255, 255, 255));
}
