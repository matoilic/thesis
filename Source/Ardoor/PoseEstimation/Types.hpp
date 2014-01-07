#ifndef TYPES_HPP
#define TYPES_HPP

#include <QMatrix4x4>
#include <opencv2/core/core.hpp>
#include <Ardoor/Ardoor.hpp>

typedef ARD_EXPORT struct
{
    bool isObjectPresent;
    QMatrix4x4 mvMatrix;
    float width;
    float height;

} PoseEstimationResult;


#endif // TYPES_HPP
