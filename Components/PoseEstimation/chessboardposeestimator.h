#ifndef CHESSBOARDPOSEESTIMATOR_H
#define CHESSBOARDPOSEESTIMATOR_H

#include "opencv2/core/core.hpp"
#include "ardoor.h"
#include "poseestimator.h"
#include "CameraCalib.h"

class ARD_EXPORT ChessboardPoseEstimator : public PoseEstimator
{
private:
    CameraCalib& calibration;

public:
    ChessboardPoseEstimator(CameraCalib& calibration, CameraConfiguration camera);
    virtual PoseEstimationResult estimatePose(cv::Mat& image);
};

#endif // CHESSBOARDPOSEESTIMATOR_H
