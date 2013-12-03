#include <iostream>
#include <cmath>
#include <opencv2/core/core.hpp>
#include <opencv2/legacy/legacy.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;

int main()
{
    string file = "/Users/matoilic/Desktop/doors/Front-Door.jpg";
    cv::Mat src = cv::imread(file), gray, canny, hough;
    vector<cv::Vec2f> lines;

    cv::cvtColor(src, gray, CV_RGB2GRAY);
    src.copyTo(hough);

    cv::Canny(gray, canny, 80, 200, 3);
    cv::HoughLines(canny, lines, 1, CV_PI / 180, 100);

    for(size_t i = 0; i < lines.size(); i++) {
        float rho = lines[i][0], theta = lines[i][1];
        cv::Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));
        cv::line(hough, pt1, pt2, cv::Scalar(255, 0, 0));
    }

    cv::imshow("Original", src);
    cv::imshow("Canny", canny);
    cv::imshow("Hough", hough);

    cv::waitKey();

    return 0;
}

