#ifndef POSEESTIMATOR_H
#define POSEESTIMATOR_H

#include <opencv2/core/core.hpp>
#include <Ardoor/Ardoor.hpp>
#include <Ardoor/Context/ArdoorContext.hpp>
#include <Ardoor/PoseEstimation/Types.hpp>


class ARD_EXPORT PoseEstimator
{
private:
    ArdoorContext *ardoorContext;

public:
    PoseEstimator(ArdoorContext *ardoorContext);
    virtual ~PoseEstimator() {}

    ArdoorContext* getArdoorContext();
    virtual PoseEstimationResult estimatePose(cv::Mat& image) = 0;
};

#endif // POSEESTIMATOR_H
