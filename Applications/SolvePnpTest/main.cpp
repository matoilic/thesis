#include <iostream>
#include <functional>
#include <ctime>
#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <Ardoor/CameraCalib/CameraConfiguration.hpp>

using namespace std;

void solvePnpTest(int flags);


#define ITERATIONS 1000
#define IMAGE_WIDTH 800
#define IMAGE_HEIGHT 600

cv::Point2f points[] = { cv::Point2f(266, 59), cv::Point2f(428, 106), cv::Point2f(371, 518), cv::Point2f(211, 555) };

function<void()> functions[] = {
    bind(solvePnpTest, CV_ITERATIVE),
    bind(solvePnpTest, CV_P3P),
    bind(solvePnpTest, CV_EPNP)
};

CameraConfiguration cameraOrig(1726.7347f, 1732.9374f, 736.2678f, 593.4867f, 1600, 1200);
CameraConfiguration camera = cameraOrig.scale(IMAGE_WIDTH, IMAGE_HEIGHT);
cv::Matx33f cameraMatrix = camera.getIntrinsics();
cv::Mat_<float> distortionMatrix = camera.getDistorsion();

vector<cv::Point2f> imagePoints;
vector<cv::Point3f> objectPoints;


float getRatio(cv::Matx33f cameraMatrix, vector<cv::Point2f> points)
{
    cv::Mat A(cameraMatrix);
    cv::Point2f c1 = points.at(0);
    cv::Point2f c2 = points.at(1);
    cv::Point2f c3 = points.at(2);
    cv::Point2f c4 = points.at(3);

    float k2, k3;
    cv::Mat _ratio;
    cv::Mat n2, n3;

    cv::Mat m1 = (cv::Mat_<float>(3,1) << (float) c1.x, (float) c1.y, 1);
    cv::Mat m2 = (cv::Mat_<float>(3,1) << (float) c4.x, (float) c4.y, 1);
    cv::Mat m3 = (cv::Mat_<float>(3,1) << (float) c2.x, (float) c2.y, 1);
    cv::Mat m4 = (cv::Mat_<float>(3,1) << (float) c3.x, (float) c3.y, 1);

    k2 = (m1.cross(m4).dot(m3)) / ((m2.cross(m4)).dot(m3));
    k3 = (m1.cross(m4).dot(m2)) / ((m3.cross(m4)).dot(m2));
    n2 = (k2*m2) - m1;
    n3 = (k3*m3) - m1;

    _ratio = (n2.t()*(A.inv().t())*(A.inv())*n2) / (n3.t()*(A.inv().t())*(A.inv())*n3);

    float ratio = sqrt(_ratio.at<float>(0,0));
    if (ratio < 1) {
        ratio = 1 / ratio;
    }

    return ratio;
}



void solvePnpTest(int flags)
{
    cv::Mat rvec, tvec;
    cv::solvePnP(objectPoints, imagePoints, cameraMatrix, distortionMatrix, rvec, tvec, false, flags);
}

void init()
{
    for (auto point : points) {
        imagePoints.push_back(point);
    }


    float doorRatio = getRatio(cameraMatrix, imagePoints);

    objectPoints.push_back(cv::Point3f(0, doorRatio, 0));
    objectPoints.push_back(cv::Point3f(1, doorRatio, 0));
    objectPoints.push_back(cv::Point3f(1, 0, 0));
    objectPoints.push_back(cv::Point3f(0, 0, 0));
}



void doTests()
{
    clock_t start, end;

    for (auto f : functions) {
        start = clock();
        for (int i = 0; i < ITERATIONS; i++) {
            f();
        }
        end = clock();

        auto duration = end - start;
        cout << duration << "\t";
    }

    cout << endl;
}


int main()
{
    init();
    doTests();

    return EXIT_SUCCESS;
}

