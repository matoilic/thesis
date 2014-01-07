#include <iostream>
#include <opencv2/calib3d/calib3d.hpp>
#include <Ardoor/PoseEstimation/DoorPoseEstimator.hpp>
#include <Ardoor/DoorDetection/DoorDetector.hpp>
#include <Ardoor/Projection/ProjectionUtil.hpp>


DoorPoseEstimator::DoorPoseEstimator(ArdoorContext *ardoorContext)
    : PoseEstimator(ardoorContext)
{
}

DoorPoseEstimator::~DoorPoseEstimator()
{
}

float DoorPoseEstimator::getRatio(cv::Matx33f cameraMatrix, cv::Point2f c1, cv::Point2f c2, cv::Point2f c3, cv::Point2f c4)
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

PoseEstimationResult DoorPoseEstimator::estimatePose(cv::Mat& image)
{
    PoseEstimationResult result;

    CameraConfiguration camera = getArdoorContext()->getCameraConfiguration(image.cols, image.rows);

    cv::Mat gray;
    cv::cvtColor(image, gray, CV_RGB2GRAY);

    std::vector<cv::Point> corners;
    DoorDetector detector(cv::Size(image.cols, image.rows));

    result.isObjectPresent = detector.findDoorCorners(gray, corners);
    if (result.isObjectPresent) {
        qDebug() << "Door found!";

        std::vector<cv::Point2f> imagePoints;
        for (std::vector<cv::Point>::iterator it = corners.begin(); it != corners.end(); it++) {
            imagePoints.push_back(cv::Point2f((*it).x, (*it).y));
        }

        float doorRatio = getRatio(camera.getIntrinsics(), imagePoints.at(0), imagePoints.at(1), imagePoints.at(2), imagePoints.at(3));
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
