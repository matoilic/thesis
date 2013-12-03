#ifdef WIN32
    #include <windows.h>
    #include <time.h>
#else
    #include <stdio.h>
    #include <sys/time.h>
    #include <time.h>
#endif

#include <cmath>
#include <iostream>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "LSWMS.h"

using namespace std;

// Timing
#ifdef WIN32
    double t1, t2;
#else
    int t1, t2;
    struct timeval ts;
#endif
double t;

void categorizeSegments(const vector<LSEG> &segments, vector<LSEG> &horizontal, vector<LSEG> &vertical)
{
    double maxAngleX = (CV_PI / 180) * 15;
    double minAngleY = (CV_PI / 2) - maxAngleX;
    double maxAngleY = (CV_PI / 2) + maxAngleX;

    for(int i = 0; i < segments.size(); i++) {
        int x = segments[i][1].x - segments[i][0].x;
        int y = segments[i][1].y - segments[i][0].y;
        int len = sqrt(x * x + y * y);

        double angle = acos(x / len);

        if(angle <= maxAngleX) {
            horizontal.push_back(segments[i]);
        } else if(angle >= minAngleY && angle <= maxAngleY) {
            vertical.push_back(segments[i]);
        }
    }
}

void drawSegments(cv::Mat &img, const vector<LSEG> &segments, const cv::Scalar &color)
{
    for(int i = 0; i < segments.size(); i++) {
        cv::line(img, segments[i][0], segments[i][1], color, 2);
    }
}

void applyLswms(LSWMS &lswms, cv::Mat img, vector<LSEG> &segments, vector<double> errors)
{
    vector<LSEG> tmpSegments;
    vector<double> tmpErrors;
    lswms.run(img, tmpSegments, tmpErrors);

    segments.reserve(segments.size() + tmpSegments.size());
    segments.insert(segments.end(), tmpSegments.begin(), tmpSegments.end());

    errors.reserve(errors.size() + tmpErrors.size());
    errors.insert(errors.end(), tmpErrors.begin(), tmpErrors.end());
}

void findSegments(string file)
{
    cv::Mat src = cv::imread(file), gray, result;
    vector<cv::Mat> channels;
    cv::resize(src, src, cv::Size(src.cols / 3, src.rows / 3));
    //cv::cvtColor(src, gray, CV_RGB2GRAY);
    src.copyTo(result);
    cv::Size size(src.cols, src.rows);
    vector<LSEG> segments;
    vector<double> errors;

    cv::split(src, channels);

    LSWMS lswms(size, 3, 10000);
    gettimeofday(&ts,0);
    t1 = (ts.tv_sec * 1000 + (ts.tv_usec / 1000));

    cv::cvtColor(src, gray, CV_RGB2GRAY);
    applyLswms(lswms, gray, segments, errors);

    for(int i = 0; i < channels.size(); i++) {
        applyLswms(lswms, channels[i], segments, errors);
    }

    gettimeofday(&ts,0);
    t2 = (ts.tv_sec * 1000 + (ts.tv_usec / 1000));
    t = (double)t2-(double)t1;
    std::cout << file << " LSWMS runtime: " << t << "ms" << std::endl;

    gettimeofday(&ts,0);
    t2 = (ts.tv_sec * 1000 + (ts.tv_usec / 1000));
    t = (double)t2-(double)t1;
    std::cout << file << " Total runtime: " << t << "ms" << std::endl;

    vector<LSEG> horizontal, vertical;
    categorizeSegments(segments, horizontal, vertical);

    drawSegments(result, horizontal, CV_RGB(255, 0, 0));
    drawSegments(result, vertical, CV_RGB(0, 0, 255));

    cv::imshow(file, result);
}

/** Main function*/
int main(int argc, char** argv)
{
    findSegments("../../Data/Images/Doors/Black_door.jpg");
    findSegments("../../Data/Images/Doors/back door 5.jpg");
    findSegments("../../Data/Images/Doors/Front-Door.jpg");
    findSegments("../../Data/Images/Doors/4.jpg");
    findSegments("../../Data/Images/Doors/3.jpg");
    findSegments("../../Data/Images/Doors/Standard-Office-Door-Size.jpg");

    cv::waitKey();
}
