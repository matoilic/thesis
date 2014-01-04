#include <iostream>
#include <opencv2/calib3d/calib3d.hpp>
#include <Ardoor/PoseEstimation/ChessboardPoseEstimator.hpp>


ChessboardPoseEstimator::ChessboardPoseEstimator(ArdoorContext *ardoorContext)
    : PoseEstimator(ardoorContext)
{
    cv::Size size = ardoorContext->getChessboardSize();
    calibration = new CameraCalib(size);
}

ChessboardPoseEstimator::~ChessboardPoseEstimator()
{
    delete calibration;
}

PoseEstimationResult ChessboardPoseEstimator::estimatePose(cv::Mat& image)
{
    PoseEstimationResult result;
    result.width = calibration->getBoardSize().width;
    result.height = calibration->getBoardSize().height;

    CameraConfiguration camera = getArdoorContext()->getCameraConfiguration(image.cols, image.rows);
    cv::Matx33f M = camera.getIntrinsics();
    std::vector<float> D = camera.getDistorsion();

    std::vector<cv::Point2f> imageCorners;
    std::vector<cv::Point3f> objectCorners;

    result.isObjectPresent = calibration->findChessboardPoints(image, imageCorners, objectCorners);
    //result.isObjectPresent = calibration->findAndDrawChessboardPoints(image, imageCorners, objectCorners);
    if (result.isObjectPresent)
    {
        cv::Vec<float, 3> R;
        cv::Vec<float, 3> T;

        cv::Mat rvec, tvec;
        cv::solvePnP(cv::Mat(objectCorners), cv::Mat(imageCorners), M, D, rvec, tvec); // Calculate the Rotation and Translation vector

        rvec.convertTo(R, CV_32FC1);
        tvec.convertTo(T, CV_32FC1);

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

        cv::Matx44f mvMatrix = reverseYZ * Rt;

        // Convert cv::Mat to QMatrix4x4
        for (int i = 0; i < mvMatrix.rows; i++) {
            for (int j = 0; j < mvMatrix.cols; j++) {
                result.mvMatrix(i, j) = mvMatrix(i, j);
            }
        }
    }

    return result;
}
