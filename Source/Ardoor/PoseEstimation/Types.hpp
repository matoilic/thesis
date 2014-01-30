#ifndef TYPES_HPP
#define TYPES_HPP

#include <QMatrix4x4>
#include <opencv2/core/core.hpp>
#include <Ardoor/Ardoor.hpp>

#define ARD_POSEESTIMATION_SCALE_FACTOR 100.0

typedef ARD_EXPORT struct
{
    /*
     * true if if the desired object was found in the image
     */
    bool isObjectPresent;
    /*
     * holds the model-view-matrix for the projection of the
     * AR object
     */
    QMatrix4x4 mvMatrix;
    /*
     * width of the searched object in object space
     */
    float width;
    /*
     * height of the searched object in object space
     */
    float height;

} PoseEstimationResult;


#endif // TYPES_HPP
