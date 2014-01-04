#include <Ardoor/CameraCalib/CameraConfiguration.hpp>

CameraConfiguration::CameraConfiguration(float fx, float fy, float cx, float cy, int calibrationWidth, int calibrationHeight, std::vector<float> distCoeffs)
{
    update(fx, fy, cx, cy, calibrationWidth, calibrationHeight, distCoeffs);
}

CameraConfiguration::CameraConfiguration(const CameraConfiguration& other)
{
    intrinsics = other.getIntrinsics();
    calibrationWidth = other.calibrationWidth;
    calibrationHeight = other.calibrationHeight;
    distortion = other.distortion;
}

CameraConfiguration& CameraConfiguration::operator=(const CameraConfiguration& rhs)
{
    intrinsics = rhs.getIntrinsics();
    calibrationWidth = rhs.calibrationWidth;
    calibrationHeight = rhs.calibrationHeight;
    distortion = rhs.distortion;
    return *this;
}

const cv::Matx33f& CameraConfiguration::getIntrinsics() const
{
    return intrinsics;
}

void CameraConfiguration::update(float fx, float fy, float cx, float cy, int calibrationWidth, int calibrationHeight, std::vector<float> distCoeffs)
{
    intrinsics = cv::Matx33f::zeros();
    intrinsics(0, 0) = fx;
    intrinsics(1, 1) = fy;
    intrinsics(0, 2) = cx;
    intrinsics(1, 2) = cy;
    intrinsics(2, 2) = 1;

    this->calibrationWidth = calibrationWidth;
    this->calibrationHeight = calibrationHeight;

    distortion = distCoeffs;
}

const std::vector<float>& CameraConfiguration::getDistorsion() const
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

int CameraConfiguration::getCalibrationWidth()
{
    return calibrationWidth;
}

int CameraConfiguration::getCalibrationHeight()
{
    return calibrationHeight;
}

CameraConfiguration CameraConfiguration::scale(int width, int height)
{
    // If the image size of the images used for calibration differs from the one
    // of the captured video frame: scale camera matrix values!
    float scaleX = width / (float) calibrationWidth;
    float scaleY = height / (float) calibrationHeight;

    float fx = getFocalLengthX() * scaleX;
    float fy = getFocalLengthY() * scaleY;
    float cx = getPrimaryPointX() * scaleX;
    float cy = getPrimaryPointY() * scaleY;

    return CameraConfiguration(fx, fy, cx, cy, width, height, distortion);
}

