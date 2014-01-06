#ifndef PROJECTIONUTIL_HPP
#define PROJECTIONUTIL_HPP

#include <opencv2/core/core.hpp>
#include <QMatrix4x4>
#include <Ardoor/CameraCalib/CameraConfiguration.hpp>

class ProjectionUtil
{
public:
    static void solvePnP(cv::InputArray &objectCorners, cv::InputArray &imageCorners, CameraConfiguration &camera, QMatrix4x4 &out);
    static void extrinsicVectorsToMatrix(cv::Vec<float, 3> &R, cv::Vec<float, 3> &T, cv::Matx44f &Rt);
    static void convertMatrix(cv::Matx44f &src, QMatrix4x4 &dest);
    static void reverseYZ(QMatrix4x4 &matrix);
};

#endif // PROJECTIONUTIL_HPP
