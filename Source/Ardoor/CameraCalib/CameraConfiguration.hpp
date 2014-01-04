#ifndef CAMERAMATRIX_H
#define CAMERAMATRIX_H

#include <opencv2/core/core.hpp>
#include <Ardoor/Ardoor.hpp>

class ARD_EXPORT CameraConfiguration
{
public:
    CameraConfiguration(float fx = 0, float fy = 0, float cx = 0, float cy = 0, int calibrationWidth = 0, int calibrationHeight = 0, std::vector<float> distCoeffs = std::vector<float>());
    CameraConfiguration(const CameraConfiguration& other);

    CameraConfiguration& operator=(const CameraConfiguration& rhs);
    void update(float fx, float fy, float cx, float cy, int calibrationWidth, int calibrationHeight, std::vector<float> distCoeffs);

    const cv::Matx33f& getIntrinsics() const;
    const std::vector<float>& getDistorsion() const;

    float getFocalLengthX();
    float getFocalLengthY();
    float getPrimaryPointX();
    float getPrimaryPointY();

    int getCalibrationWidth();
    int getCalibrationHeight();

    CameraConfiguration scale(int width, int height);

private:
    int calibrationWidth;
    int calibrationHeight;
    cv::Matx33f intrinsics;
    std::vector<float> distortion;
};

#endif // CAMERAMATRIX_H
