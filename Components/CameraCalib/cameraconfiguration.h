#ifndef CAMERAMATRIX_H
#define CAMERAMATRIX_H

#include "opencv2/core/core.hpp"
#include "ardoor.h"

class ARD_EXPORT CameraConfiguration
{
private:
    void initialize(float fx, float fy, float cx, float cy);

public:
    CameraConfiguration() { initialize(0, 0, 0, 0); };
    CameraConfiguration(float fx, float fy, float cx, float cy) { initialize(fx, fy, cx, cy); };
    CameraConfiguration(const CameraConfiguration& other);

    CameraConfiguration& CameraConfiguration::operator=(const CameraConfiguration& rhs);

    const cv::Matx33f& getIntrinsics() const;
    const cv::Mat_<float>& getDistorsion() const;

    float getFocalLengthX();
    float getFocalLengthY();
    float getPrimaryPointX();
    float getPrimaryPointY();

private:
    cv::Matx33f intrinsics;
    cv::Mat_<float> distortion;
};

#endif // CAMERAMATRIX_H
