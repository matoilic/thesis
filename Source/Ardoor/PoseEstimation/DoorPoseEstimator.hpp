#ifndef CHESSBOARDPOSEESTIMATOR_H
#define CHESSBOARDPOSEESTIMATOR_H

#include <opencv2/core/core.hpp>
#include <Ardoor/Ardoor.hpp>
#include <Ardoor/PoseEstimation/PoseEstimator.hpp>

class ARD_EXPORT DoorPoseEstimator : public PoseEstimator
{
public:
    DoorPoseEstimator(ArdoorContext *ardoorContext);
    virtual PoseEstimationResult estimatePose(cv::Mat& image);
};

#endif // CHESSBOARDPOSEESTIMATOR_H
