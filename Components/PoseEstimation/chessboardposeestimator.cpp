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

    result.isObjectPresent = calibration.findChessboardPoints(image, imageCorners, objectCorners);
    if (result.isObjectPresent)
    {
        cv::Vec<float, 3> R;
        cv::Vec<float, 3> T;

        cv::Mat rvec, tvec;
        cv::solvePnP(cv::Mat(objectCorners), cv::Mat(imageCorners), M, D, rvec, tvec); // Calculate the Rotation and Translation vector

        rvec.convertTo(R, CV_32F);
        tvec.convertTo(T, CV_32F);

        cv::Matx33f rotMat;
        cv::Rodrigues(R, rotMat);

        cv::Matx44f mvMatrix = cv::Matx44f::eye();

        mvMatrix(0, 0) = rotMat(0, 0);
        mvMatrix(0, 1) = rotMat(0, 1);
        mvMatrix(0, 2) = rotMat(0, 2);
        mvMatrix(0, 3) = T(0);

        mvMatrix(1, 0) = -rotMat(1, 0);
        mvMatrix(1, 1) = -rotMat(1, 1);
        mvMatrix(1, 2) = -rotMat(1, 2);
        mvMatrix(1, 3) = -T(1);

        mvMatrix(2, 0) = -rotMat(2, 0);
        mvMatrix(2, 1) = -rotMat(2, 1);
        mvMatrix(2, 2) = -rotMat(2, 2);
        mvMatrix(2, 3) = -T(2);

        result.mvMatrix = mvMatrix;
    }

    return result;
}
