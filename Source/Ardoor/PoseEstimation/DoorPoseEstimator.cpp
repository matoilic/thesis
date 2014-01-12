#include <iostream>
#include <opencv2/calib3d/calib3d.hpp>
#include <Ardoor/PoseEstimation/DoorPoseEstimator.hpp>
#include <Ardoor/DoorDetection/DoorDetector.hpp>
#include <Ardoor/Projection/ProjectionUtil.hpp>

DoorPoseEstimator::DoorPoseEstimator(ArdoorContext *ardoorContext)
    : PoseEstimator(ardoorContext)
{
}

PoseEstimationResult DoorPoseEstimator::estimatePose(cv::Mat& image)
{
    PoseEstimationResult result;

    CameraConfiguration camera = getArdoorContext()->getCameraConfiguration(image.cols, image.rows);

    cv::Mat gray;
    cv::cvtColor(image, gray, CV_RGB2GRAY);

    std::vector<cv::Point> corners;
    DoorDetector detector(cv::Size(image.cols, image.rows));

    result.isObjectPresent = detector.findDoorCorners(gray, corners);
    result.isObjectPresent = true;
    if (result.isObjectPresent) {
        std::vector<cv::Point2f> imagePoints;
        for (std::vector<cv::Point>::iterator it = corners.begin(); it != corners.end(); it++) {
            imagePoints.push_back(cv::Point2f((*it).x, (*it).y));
        }

        imagePoints.clear();
        imagePoints.push_back(cv::Point2f(443, 34));
        imagePoints.push_back(cv::Point2f(576, 71));
        imagePoints.push_back(cv::Point2f(562, 533));
        imagePoints.push_back(cv::Point2f(430, 593));


        float doorRatio = ProjectionUtil::getRatio(camera.getIntrinsics(), imagePoints.at(0), imagePoints.at(1), imagePoints.at(2), imagePoints.at(3));
        if (doorRatio < 1) {
            doorRatio = 1/doorRatio;
        }

        std::vector<cv::Point3f> objectPoints;
        objectPoints.push_back(cv::Point3f(0, doorRatio, 0));
        objectPoints.push_back(cv::Point3f(1, doorRatio, 0));
        objectPoints.push_back(cv::Point3f(1, 0, 0));
        objectPoints.push_back(cv::Point3f(0, 0, 0));

        result.width = 1;
        result.height = doorRatio;

        ProjectionUtil::solvePnP(objectPoints, imagePoints, camera, result.mvMatrix);
        ProjectionUtil::reverseYZ(result.mvMatrix);
    }

    return result;
}
