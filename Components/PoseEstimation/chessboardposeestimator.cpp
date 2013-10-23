#include "chessboardposeestimator.h"
#include "opencv2/calib3d/calib3d.hpp"

ChessboardPoseEstimator::ChessboardPoseEstimator(CameraCalib& calibration, CameraConfiguration camera)
    : PoseEstimator(camera)
    , calibration(calibration)
{
}

PoseEstimationResult ChessboardPoseEstimator::estimatePose(cv::Mat& image)
{
    PoseEstimationResult result;

    cv::Matx33f M = getCamera().getIntrinsics();
    cv::Mat_<float> D = getCamera().getDistorsion();

    std::vector<cv::Point2f> imageCorners;
    std::vector<cv::Point3f> objectCorners;

    result.isObjectPresent = calibration.findAndDrawChessboardPoints(image, imageCorners, objectCorners);
    if (result.isObjectPresent)
    {
        cv::Point3f firstPoint = objectCorners.at(0);

        cv::Vec<double, 3> R;
        cv::Vec<double, 3> T;

        cv::Mat rvec, tvec;
        cv::solvePnP(cv::Mat(objectCorners), cv::Mat(imageCorners), M, D, rvec, tvec); // Calculate the Rotation and Translation vector

        rvec.convertTo(R, CV_64FC1);
        tvec.convertTo(T, CV_64FC1);

        std::cout.precision(2);
        std::cout << "rvec = [" << R(0) << ", " << R(1) << ", " << R(2) << "]   ";
        std::cout << "tvec = [" << T(0) << ", " << T(1) << ", " << T(2) << "]   ";
        std::cout << std::endl;

        cv::Matx33d rotMat;
        cv::Rodrigues(R, rotMat);

        // Build combined rotation and translation matrix
        cv::Matx44d Rt = cv::Matx44d::eye();

        Rt(0, 0) = -rotMat(0, 0);
        Rt(0, 1) = -rotMat(0, 1);
        Rt(0, 2) = rotMat(0, 2);
        Rt(0, 3) = T(0) + firstPoint.x;

        Rt(1, 0) = -rotMat(1, 0);
        Rt(1, 1) = -rotMat(1, 1);
        Rt(1, 2) = rotMat(1, 2);
        Rt(1, 3) = T(1) + firstPoint.y;

        Rt(2, 0) = -rotMat(2, 0);
        Rt(2, 1) = -rotMat(2, 1);
        Rt(2, 2) = rotMat(2, 2);
        Rt(2, 3) = T(2);

        // OpenGL has Y and Z axes reversed
        cv::Matx44d reverseYZ = cv::Matx44d::eye();
        reverseYZ(1, 1) = -1.0;
        reverseYZ(2, 2) = -1.0;

        result.mvMatrix = reverseYZ * Rt;
    }

    return result;
}
