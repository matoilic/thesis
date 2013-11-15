#include "chessboardposeestimator.h"
#include "opencv2/calib3d/calib3d.hpp"

ChessboardPoseEstimator::ChessboardPoseEstimator(CameraCalib& calibration, CameraConfiguration camera)
    : PoseEstimator(camera)
    , calibration(calibration)
{
}

PoseEstimationResult ChessboardPoseEstimator::estimatePose(cv::Mat& image)
{
    std::cout << "estimatePose()" << std::endl << std::flush;

    PoseEstimationResult result;

    CameraConfiguration *camera = getCamera().scale(image.cols, image.rows);
    cv::Matx33f M = camera->getIntrinsics();
    cv::Mat_<float> D = camera->getDistorsion();

    std::vector<cv::Point2f> imageCorners;
    std::vector<cv::Point3f> objectCorners;

    result.isObjectPresent = calibration.findAndDrawChessboardPoints(image, imageCorners, objectCorners);
    if (result.isObjectPresent)
    {
        cv::Vec<float, 3> R;
        cv::Vec<float, 3> T;

        cv::Mat rvec, tvec;
        cv::solvePnP(cv::Mat(objectCorners), cv::Mat(imageCorners), M, D, rvec, tvec); // Calculate the Rotation and Translation vector

        rvec.convertTo(R, CV_32FC1);
        tvec.convertTo(T, CV_32FC1);

        std::cout.precision(2);
        std::cout << "rvec = [" << R(0) << ", " << R(1) << ", " << R(2) << "]   ";
        std::cout << "tvec = [" << T(0) << ", " << T(1) << ", " << T(2) << "]   ";
        std::cout << std::endl;

        cv::Matx33f rotMat;
        cv::Rodrigues(R, rotMat);

        // Build combined rotation and translation matrix
        cv::Matx44f Rt = cv::Matx44f::eye();

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

        // OpenGL has Y and Z axes reversed
        cv::Matx44f reverseYZ = cv::Matx44f::eye();
        reverseYZ(1, 1) = -1.0;
        reverseYZ(2, 2) = -1.0;

        result.mvMatrix = reverseYZ * Rt;
    }

    delete camera;
    return result;
}
