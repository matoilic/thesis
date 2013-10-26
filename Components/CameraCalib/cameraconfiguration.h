#ifndef CAMERAMATRIX_H
#define CAMERAMATRIX_H

#include "opencv2/core/core.hpp"
#include "ardoor.h"
class ARD_EXPORT CameraConfiguration
{
private:
    void initialize(float fx, float fy, float cx, float cy, cv::Size calibrationSize);

public:
    CameraConfiguration() { initialize(0, 0, 0, 0, cv::Size(0, 0)); }
    CameraConfiguration(float fx, float fy, float cx, float cy, cv::Size calibrationSize) { initialize(fx, fy, cx, cy, calibrationSize); }
    CameraConfiguration(const CameraConfiguration& other);

    CameraConfiguration& operator=(const CameraConfiguration& rhs);

    const cv::Matx33f& getIntrinsics() const;
    const cv::Mat_<float>& getDistorsion() const;

    float getFocalLengthX();
    float getFocalLengthY();
    float getPrimaryPointX();
    float getPrimaryPointY();

    CameraConfiguration& scale(cv::Size imageSize);

private:
    cv::Size calibrationSize;
    cv::Matx33f intrinsics;
    cv::Mat_<float> distortion;
};

#endif // CAMERAMATRIX_H
