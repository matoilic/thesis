#include "poseestimator.h"

PoseEstimator::PoseEstimator(CameraConfiguration camera)
{
    setCamera(camera);
}

CameraConfiguration PoseEstimator::getCamera()
{
    return camera;
}

void PoseEstimator::setCamera(CameraConfiguration camera)
{
    this->camera = camera;
}
