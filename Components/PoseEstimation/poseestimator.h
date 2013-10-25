#ifndef POSEESTIMATOR_H
#define POSEESTIMATOR_H

#include "opencv2/core/core.hpp"
#include "ardoor.h"
#include "cameraconfiguration.h"

typedef struct
{
    bool isObjectPresent;
    cv::Matx44d mvMatrix;

} PoseEstimationResult;


class ARD_EXPORT PoseEstimator
{
private:
    CameraConfiguration camera;

public:
    PoseEstimator(CameraConfiguration camera);

    CameraConfiguration getCamera();
    void setCamera(CameraConfiguration camera);

    virtual PoseEstimationResult estimatePose(cv::Mat& image) = 0;
};

#endif // POSEESTIMATOR_H
