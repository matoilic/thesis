#include <iostream>
#include <opencv2/calib3d/calib3d.hpp>
#include <Ardoor/PoseEstimation/ChessboardPoseEstimator.hpp>
#include <Ardoor/Projection/ProjectionUtil.hpp>


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

    CameraConfiguration camera = getArdoorContext()->getCameraConfiguration(image.cols, image.rows);

    std::vector<cv::Point2f> imageCorners;
    std::vector<cv::Point3f> objectCorners;
    result.isObjectPresent = calibration->findChessboardPoints(image, imageCorners, objectCorners);

    if (result.isObjectPresent) {
        result.width = calibration->getBoardSize().width;
        result.height = calibration->getBoardSize().height;

        ProjectionUtil::solvePnP(objectCorners, imageCorners, camera, result.mvMatrix);
        ProjectionUtil::reverseYZ(result.mvMatrix);
    }

    return result;
}
