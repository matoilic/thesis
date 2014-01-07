#include "FrameProcessor.hpp"
#include <opencv2/imgproc/imgproc.hpp>

FrameProcessor::FrameProcessor(ArdoorContext *ardoorContext, PoseEstimator *poseEstimator, ArRenderingContext *renderingContext, QWindow *window)
    : ardoorContext(ardoorContext)
    , poseEstimator(poseEstimator)
    , renderingContext(renderingContext)
    , window(window)
{
}

void FrameProcessor::operator() (cv::Mat &frame)
{
    if (ardoorContext->isFrontCamera()) {
        cv::flip(frame, frame, 1);
    }   

    cv::cvtColor(frame, frame, CV_BGR2RGB);

    PoseEstimationResult result = poseEstimator->estimatePose(frame);
    renderingContext->update(frame, result);

    window->resize(frame.cols, frame.rows);
}
