#ifndef DOORDETECTOR_H
#define DOORDETECTOR_H

#ifdef WIN32
    #include <windows.h>
    #include <time.h>
#else
    #include <stdio.h>
    #include <sys/time.h>
    #include <time.h>
#endif

#include <libgen.h>
#include <cmath>
#include <iostream>
#include <queue>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "LSWMS.h"
#include "LineSegment.h"

#define MIN_SEGMENT_LENGTH 0.01
#define MAX_SEGMENT_DISTANCE 0.1
#define LINE_GROWTH 0.03
#define MIN_DOOR_WIDTH 0.25
#define MAX_EDGE_DIFFERENCE 0.03

using namespace std;

class DoorDetector
{
    cv::Ptr<cv::CLAHE> clahe;
    LSWMS *lswms;
    int diagonalLength;

    void categorizeSegments(vector<LineSegment> &segments, vector<LineSegment> &horizontal, vector<LineSegment> &vertical);
    void drawSegments(const cv::Mat &src, cv::Mat &dest, vector<LineSegment> &segments, const cv::Scalar &color);
    void enhanceEdges(cv::Mat &image);
    void joinSegments(vector<LineSegment> &segments, const cv::Mat &img, bool horizontal);
    void applyLswms(cv::Mat img, vector<LineSegment> &segments);
    void findSegments(cv::Mat &grayImg, vector<LineSegment> &horizontal, vector<LineSegment> &vertical);
    void growSegments(vector<LineSegment> &segments, int length);
    void findDoor(vector<LineSegment> &vertical, vector<LineSegment> &horizontal, vector<cv::Point> &corners);
    bool isTopDoorSegment(LineSegment &left, LineSegment &right, LineSegment &top, int minDoorSize);
    bool isBottomDoorSegment(LineSegment &left, LineSegment &right, LineSegment &bottom, int minDoorSize);
public:
    DoorDetector(cv::Size inputSize);
    ~DoorDetector();
    bool findDoorCorners(const cv::Mat &grayImg, vector<cv::Point> &corners);
};

#endif // DOORDETECTOR_H
