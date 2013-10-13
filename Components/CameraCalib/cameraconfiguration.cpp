#include "cameraconfiguration.h"

void CameraConfiguration::initialize(float fx, float fy, float cx, float cy)
{
    intrinsics = cv::Matx33f::zeros();
    intrinsics(0, 0) = fx;
    intrinsics(1, 1) = fy;
    intrinsics(0, 2) = cx;
    intrinsics(1, 2) = cy;
}

CameraConfiguration::CameraConfiguration(const CameraConfiguration& other)
{
    intrinsics = other.getIntrinsics();
}

CameraConfiguration& CameraConfiguration::operator=(const CameraConfiguration& rhs)
{
    intrinsics = rhs.getIntrinsics();
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
