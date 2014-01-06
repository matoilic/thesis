#ifndef CHESSBOARDPOSEESTIMATOR_H
#define CHESSBOARDPOSEESTIMATOR_H

#include <opencv2/core/core.hpp>
#include <Ardoor/Ardoor.hpp>
#include <Ardoor/PoseEstimation/PoseEstimator.hpp>

class ARD_EXPORT DoorPoseEstimator : public PoseEstimator
{
public:
    DoorPoseEstimator(ArdoorContext *ardoorContext);
    ~DoorPoseEstimator();

    virtual std::vector<PoseEstimationResult> estimatePose(cv::Mat& image);
    float getRatio(cv::Matx33f cameraMatrix, cv::Point2f c1, cv::Point2f c2, cv::Point2f c3, cv::Point2f c4);
};

#endif // CHESSBOARDPOSEESTIMATOR_H
