#ifndef TYPES_HPP
#define TYPES_HPP

#include <QMatrix4x4>
#include <opencv2/core/core.hpp>
#include <Ardoor/Ardoor.hpp>

#define ARD_POSEESTIMATION_SCALE_FACTOR 100.0

typedef ARD_EXPORT struct
{
    bool isObjectPresent;
    QMatrix4x4 mvMatrix;
    float width;
    float height;

} PoseEstimationResult;


#endif // TYPES_HPP
