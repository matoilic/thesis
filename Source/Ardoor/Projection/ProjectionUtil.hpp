#ifndef PROJECTIONUTIL_HPP
#define PROJECTIONUTIL_HPP

#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <QMatrix4x4>
#include <Ardoor/CameraCalib/CameraConfiguration.hpp>

class ARD_EXPORT ProjectionUtil
{
public:
    static void solvePnP(cv::InputArray &objectCorners, cv::InputArray &imageCorners, CameraConfiguration &camera, QMatrix4x4 &out, int flags = CV_ITERATIVE);
    static void extrinsicVectorsToMatrix(cv::Vec<float, 3> &R, cv::Vec<float, 3> &T, cv::Matx44f &Rt);
    static void convertMatrix(cv::Matx44f &src, QMatrix4x4 &dest);
    static void reverseYZ(QMatrix4x4 &matrix);
    static float getRatio(cv::Matx33f cameraMatrix, cv::Point2f c1, cv::Point2f c2, cv::Point2f c3, cv::Point2f c4);

};

#endif // PROJECTIONUTIL_HPP
