#include "FrameProcessor.hpp"
#include <opencv2/imgproc/imgproc.hpp>

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

    cv::cvtColor(*resultImage, *resultImage, CV_BGR2RGB);

    std::vector<PoseEstimationResult> results = poseEstimator->estimatePose(*resultImage);
    renderingContext->update(*resultImage, results);
}
