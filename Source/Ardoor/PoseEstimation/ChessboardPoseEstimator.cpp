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

std::vector<PoseEstimationResult> ChessboardPoseEstimator::estimatePose(cv::Mat& image)
{
    std::vector<PoseEstimationResult> results;

    CameraConfiguration camera = getArdoorContext()->getCameraConfiguration(image.cols, image.rows);

    std::vector<cv::Point2f> imageCorners;
    std::vector<cv::Point3f> objectCorners;
    bool success = calibration->findChessboardPoints(image, imageCorners, objectCorners);

    if (success) {
        PoseEstimationResult result;
        result.width = calibration->getBoardSize().width;
        result.height = calibration->getBoardSize().height;

        ProjectionUtil::solvePnP(objectCorners, imageCorners, camera, result.mvMatrix);
        ProjectionUtil::reverseYZ(result.mvMatrix);

        results.push_back(result);
    }

    return results;
}
