#include <iostream>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <Ardoor/Projection/ProjectionUtil.hpp>


using namespace std;

struct Camera {
    std::string name;
    cv::Matx33f intrinsics;
    std::vector<float> distCoeffs;
};

struct Camera camera;

struct Camera loadCamera(std::string name, std::string calibrationFile)
{
    struct Camera camera;
    camera.name = name;

    ifstream in;
    in.open(calibrationFile);

    float fx, fy, cx, cy;
    in >> fx;
    in >> fy;
    in >> cx;
    in >> cy;

    camera.intrinsics = cv::Matx33f::eye();
    camera.intrinsics(0, 0) = fx;
    camera.intrinsics(1, 1) = fy;
    camera.intrinsics(0, 2) = cx;
    camera.intrinsics(1, 2) = cy;

    float distCoeff;
    while (!in.eof()) {
        in >> distCoeff;
        camera.distCoeffs.push_back(distCoeff);
    }

    in.close();

    return camera;
}

void printMat(const cv::Mat &mat)
{
    for (int r = 0; r < mat.rows; r++)
    {
        for (int c = 0; c < mat.cols; c++)
        {
            cout << mat.row(r).col(c) << "\t";
        }

        cout << endl;
    }
}


void calculateRatio(std::vector<cv::Point2f> points, bool undistort)
{
    std::vector<cv::Point2f> imagePoints;

    if (undistort) {
        cv::Mat src(points);
        cv::Mat undistortedPoints;
        cv::undistortPoints(src, undistortedPoints, camera.intrinsics, cv::Mat(camera.distCoeffs).t());

        for (int i = 0; i < 4; i++) {
            auto x = undistortedPoints.at<float>(i, 0);
            auto y = undistortedPoints.at<float>(i, 1);

            cv::Point3f undistortedPoint = camera.intrinsics * cv::Point3f(x, y, 1);
            imagePoints.push_back(cv::Point2f(undistortedPoint.x, undistortedPoint.y));
        }
    } else {
        imagePoints = points;
    }

    float ratio = ProjectionUtil::getRatio(camera.intrinsics, imagePoints.at(0), imagePoints.at(1), imagePoints.at(2), imagePoints.at(3));
    if (ratio < 1) ratio = 1/ratio;

    std::cout << ratio << std::endl;
}

int main(int argc, char** argv)
{
    //camera = loadCamera("Canon EOS 500D", "../../Data/cannon-eos-500d.txt");
    camera = loadCamera("Canon EOS 500D v2", "../../Data/canon-eos-500d-v2.txt");
    bool doUndistort = false;

    std::vector<cv::Point2f> points;

    points.clear();
    points.push_back(cv::Point2f(415, 749));
    points.push_back(cv::Point2f(2558, 790));
    points.push_back(cv::Point2f(2524, 1873));
    points.push_back(cv::Point2f(398, 1799));
    calculateRatio(points, doUndistort);

    points.clear();
    points.push_back(cv::Point2f(630, 892));
    points.push_back(cv::Point2f(2509, 877));
    points.push_back(cv::Point2f(2564, 1821));
    points.push_back(cv::Point2f(478, 1800));
    calculateRatio(points, doUndistort);

    points.clear();
    points.push_back(cv::Point2f(707, 934));
    points.push_back(cv::Point2f(2483, 948));
    points.push_back(cv::Point2f(2574, 1713));
    points.push_back(cv::Point2f(432, 1671));
    calculateRatio(points, doUndistort);

    points.clear();
    points.push_back(cv::Point2f(773, 1112));
    points.push_back(cv::Point2f(2482, 1137));
    points.push_back(cv::Point2f(2608, 1640));
    points.push_back(cv::Point2f(405, 1590));
    calculateRatio(points, doUndistort);

    points.clear();
    points.push_back(cv::Point2f(850, 1423));
    points.push_back(cv::Point2f(2558, 1409));
    points.push_back(cv::Point2f(2724, 1633));
    points.push_back(cv::Point2f(429, 1640));
    calculateRatio(points, doUndistort);

    while (cv::waitKey() != 'q');
}
