#ifndef CHESSBOARDPOSEESTIMATOR_H
#define CHESSBOARDPOSEESTIMATOR_H

#include <opencv2/core/core.hpp>
#include <Ardoor/Ardoor.hpp>
#include <Ardoor/PoseEstimation/PoseEstimator.hpp>
#include <Ardoor/CameraCalib/CameraCalib.hpp>

class ARD_EXPORT ChessboardPoseEstimator : public PoseEstimator
{
public:
    ChessboardPoseEstimator(ArdoorContext *ardoorContext);
    ~ChessboardPoseEstimator();

    virtual PoseEstimationResult estimatePose(cv::Mat& image);

private:
    CameraCalib *calibration;
};

#endif // CHESSBOARDPOSEESTIMATOR_H
