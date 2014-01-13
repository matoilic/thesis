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

        std::vector<cv::Point2f> undistortedPoints;
        ProjectionUtil::undistortPoints(imagePoints, undistortedPoints, camera);

        float doorRatio = ProjectionUtil::getRatio(camera.getIntrinsics(), undistortedPoints.at(0), undistortedPoints.at(1), undistortedPoints.at(2), undistortedPoints.at(3));
        if (doorRatio < 1) {
            doorRatio = 1/doorRatio;
        }

        float w = 1.0;
        float h = doorRatio;

        // The solvePnP results with the floating point ratio were inaccurate.
        // Scaling everything here and in the OpenGL context improved the results.
        float wScaled = w * ARD_POSEESTIMATION_SCALE_FACTOR;
        float hScaled = h * ARD_POSEESTIMATION_SCALE_FACTOR;

        std::vector<cv::Point3f> objectPoints;
        objectPoints.push_back(cv::Point3f(0, hScaled, 0));
        objectPoints.push_back(cv::Point3f(wScaled, hScaled, 0));
        objectPoints.push_back(cv::Point3f(wScaled, 0, 0));
        objectPoints.push_back(cv::Point3f(0, 0, 0));

        result.width = w;
        result.height = h;

        ProjectionUtil::solvePnP(objectPoints, imagePoints, camera, result.mvMatrix, CV_P3P);
        ProjectionUtil::reverseYZ(result.mvMatrix);
    }

    return result;
}
