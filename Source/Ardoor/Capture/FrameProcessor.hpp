#ifndef FRAMEPROCESSOR_HPP
#define FRAMEPROCESSOR_HPP

#include <QWindow>
#include <Ardoor/Capture/ImageHandler.hpp>
#include <Ardoor/PoseEstimation/PoseEstimator.hpp>
#include <Ardoor/Rendering/ArRenderingContext.hpp>

class ARD_EXPORT FrameProcessor : public ImageHandler
{
private:
    ArdoorContext *ardoorContext;
    PoseEstimator *poseEstimator;
    ArRenderingContext *renderingContext;
    QWindow *window;

public:
    FrameProcessor(ArdoorContext *ardoorContext, PoseEstimator *poseEstimator, ArRenderingContext *renderingContext, QWindow *window);
    virtual void operator() (cv::Mat &frame);
};
#endif // FRAMEPROCESSOR_HPP
