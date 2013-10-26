#include "cameraconfiguration.h"

void CameraConfiguration::initialize(float fx, float fy, float cx, float cy, cv::Size calibrationSize)
{
    intrinsics = cv::Matx33f::zeros();
    intrinsics(0, 0) = fx;
    intrinsics(1, 1) = fy;
    intrinsics(0, 2) = cx;
    intrinsics(1, 2) = cy;

    this->calibrationSize = calibrationSize;
}

CameraConfiguration::CameraConfiguration(const CameraConfiguration& other)
{
    intrinsics = other.getIntrinsics();
    calibrationSize = other.calibrationSize;
}

CameraConfiguration& CameraConfiguration::operator=(const CameraConfiguration& rhs)
{
    intrinsics = rhs.getIntrinsics();
    calibrationSize = rhs.calibrationSize;
    return *this;
}

const cv::Matx33f& CameraConfiguration::getIntrinsics() const
{
    return intrinsics;
}

const cv::Mat_<float>&  CameraConfiguration::getDistorsion() const
{
    return distortion;
}

float CameraConfiguration::getFocalLengthX()
{
    return intrinsics(0, 0);
}

float CameraConfiguration::getFocalLengthY()
{
    return intrinsics(1, 1);
}

float CameraConfiguration::getPrimaryPointX()
{
    return intrinsics(0, 2);
}

float CameraConfiguration::getPrimaryPointY()
{
    return intrinsics(1, 2);
}

CameraConfiguration& CameraConfiguration::scale(cv::Size imageSize)
{
    // If the image size of the images used for calibration differs from the one
    // of the captured video frame: scale camera matrix values!
    float scaleX = imageSize.width / (float) calibrationSize.width;
    float scaleY = imageSize.height / (float) calibrationSize.height;

    float fx = getFocalLengthX() * scaleX;
    float fy = getFocalLengthY() * scaleY;
    float cx = getPrimaryPointX() * scaleX;
    float cy = getPrimaryPointY() * scaleY;

    return CameraConfiguration(fx, fy, cx, cy, imageSize);
}

