#include "ProjectionUtil.hpp"
#include <opencv2/calib3d/calib3d.hpp>


void ProjectionUtil::solvePnP(cv::InputArray &objectCorners, cv::InputArray &imageCorners, CameraConfiguration &camera, QMatrix4x4 &out)
{
    cv::Vec<float, 3> R;
    cv::Vec<float, 3> T;
    cv::Mat rvec, tvec;

    cv::Matx33f M = camera.getIntrinsics();
    std::vector<float>  D = camera.getDistorsion();
    cv::solvePnP(objectCorners, imageCorners, M, D, rvec, tvec); // Calculate the Rotation and Translation vector

    rvec.convertTo(R, CV_32FC1);
    tvec.convertTo(T, CV_32FC1);

    // Build combined rotation and translation matrix
    cv::Matx44f Rt;
    extrinsicVectorsToMatrix(R, T, Rt);

    convertMatrix(Rt, out);
}

void ProjectionUtil::extrinsicVectorsToMatrix(cv::Vec<float, 3> &R, cv::Vec<float, 3> &T, cv::Matx44f &Rt)
{
    cv::Matx33f rotMat;
    cv::Rodrigues(R, rotMat);

    // Build combined rotation and translation matrix
    Rt = cv::Matx44f::eye();

    Rt(0, 0) = rotMat(0, 0);
    Rt(0, 1) = rotMat(0, 1);
    Rt(0, 2) = -rotMat(0, 2);
    Rt(0, 3) = T(0);

    Rt(1, 0) = rotMat(1, 0);
    Rt(1, 1) = rotMat(1, 1);
    Rt(1, 2) = -rotMat(1, 2);
    Rt(1, 3) = T(1);

    Rt(2, 0) = rotMat(2, 0);
    Rt(2, 1) = rotMat(2, 1);
    Rt(2, 2) = -rotMat(2, 2);
    Rt(2, 3) = T(2);
}

void ProjectionUtil::convertMatrix(cv::Matx44f &src, QMatrix4x4 &dest)
{
    // Convert cv::Mat to QMatrix4x4
    for (int i = 0; i < src.rows; i++) {
        for (int j = 0; j < src.cols; j++) {
            dest(i, j) = src(i, j);
        }
    }
}


void ProjectionUtil::reverseYZ(QMatrix4x4 &matrix)
{
    QMatrix4x4 reverse;
    reverse.setToIdentity();
    reverse(1, 1) = -1.0;
    reverse(2, 2) = -1.0;

    matrix = reverse * matrix;
}
