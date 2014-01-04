#include <Ardoor/PoseEstimation/PoseEstimator.hpp>


PoseEstimator::PoseEstimator(ArdoorContext *ardoorContext)
{
    this->ardoorContext = ardoorContext;
}

ArdoorContext* PoseEstimator::getArdoorContext()
{
    return ardoorContext;
}
