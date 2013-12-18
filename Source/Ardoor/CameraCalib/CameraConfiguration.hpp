#ifndef CAMERAMATRIX_H
#define CAMERAMATRIX_H

#include <opencv2/core/core.hpp>
#include <Ardoor/Ardoor.hpp>

class ARD_EXPORT CameraConfiguration
{
private:
    void initialize(float fx, float fy, float cx, float cy, int calibrationWidth, int calibrationHeight);

public:
    CameraConfiguration() { initialize(0, 0, 0, 0, 0, 0); }
    CameraConfiguration(float fx, float fy, float cx, float cy, int calibrationWidth, int calibrationHeight) { initialize(fx, fy, cx, cy, calibrationWidth, calibrationHeight); }
    CameraConfiguration(const CameraConfiguration& other);

    CameraConfiguration& operator=(const CameraConfiguration& rhs);

    const cv::Matx33f& getIntrinsics() const;
    const cv::Mat_<float>& getDistorsion() const;

    float getFocalLengthX();
    float getFocalLengthY();
    float getPrimaryPointX();
    float getPrimaryPointY();

    CameraConfiguration scale(int width, int height);

private:
    int calibrationWidth;
    int calibrationHeight;
    cv::Matx33f intrinsics;
    cv::Mat_<float> distortion;
};

#endif // CAMERAMATRIX_H
