#include "FrameProcessor.hpp"

FrameProcessor::FrameProcessor(ArdoorContext *ardoorContext, PoseEstimator *poseEstimator, ArRenderingContext *renderingContext)
    : ardoorContext(ardoorContext)
    , poseEstimator(poseEstimator)
    , renderingContext(renderingContext)
{
}

void FrameProcessor::operator() (cv::Mat &frame)
{
    cv::Mat temp;
    cv::Mat *resultImage = &frame;

    if (ardoorContext->isFrontCamera()) {
        cv::flip(frame, temp, 1);
        resultImage = &temp;
    }

    PoseEstimationResult result = poseEstimator->estimatePose(*resultImage);
    renderingContext->update(*resultImage, result);
}
