#include "ProjectionUtil.hpp"
#include <opencv2/calib3d/calib3d.hpp>


void ProjectionUtil::solvePnP(cv::InputArray &objectCorners, cv::InputArray &imageCorners, CameraConfiguration &camera, QMatrix4x4 &out, int flags)
{
    cv::Vec<float, 3> R;
    cv::Vec<float, 3> T;
    cv::Mat rvec, tvec;

    cv::Matx33f M = camera.getIntrinsics();
    std::vector<float>  D = camera.getDistorsion();
    cv::solvePnP(objectCorners, imageCorners, M, D, rvec, tvec, false, flags); // Calculate the Rotation and Translation vector

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


float ProjectionUtil::getRatio(cv::Matx33f cameraMatrix, cv::Point2f c1, cv::Point2f c2, cv::Point2f c3, cv::Point2f c4)
{
    cv::Mat A(cameraMatrix);

    float k2, k3;
    cv::Mat _ratio;
    cv::Mat n2, n3;

    cv::Mat m1 = (cv::Mat_<float>(3,1) << (float) c1.x, (float) c1.y, 1);
    cv::Mat m2 = (cv::Mat_<float>(3,1) << (float) c4.x, (float) c4.y, 1);
    cv::Mat m3 = (cv::Mat_<float>(3,1) << (float) c2.x, (float) c2.y, 1);
    cv::Mat m4 = (cv::Mat_<float>(3,1) << (float) c3.x, (float) c3.y, 1);

    k2 = (m1.cross(m4).dot(m3)) / ((m2.cross(m4)).dot(m3));
    k3 = (m1.cross(m4).dot(m2)) / ((m3.cross(m4)).dot(m2));
    n2 = (k2 * m2) - m1;
    n3 = (k3 * m3) - m1;

    _ratio = (n2.t() * (A.inv().t()) * (A.inv()) * n2) / (n3.t() * (A.inv().t()) * (A.inv()) * n3);

    return sqrt(_ratio.at<float>(0, 0));
}

