#ifndef DOORDETECTOR_H
#define DOORDETECTOR_H

#include <opencv2/core/core.hpp>
#include <Ardoor/Ardoor.hpp>
#include "LSWMS.hpp"
#include "LineSegment.hpp"

#define MIN_SEGMENT_LENGTH 0.01 //in percent of the image diagonal
#define MAX_SEGMENT_DISTANCE 0.1 //in percent of the image diagonal
#define LINE_GROWTH 0.03 //in percent of the image diagonal
#define MIN_DOOR_WIDTH 0.15 //in percent of the image diagonal
#define MIN_DOOR_HEIGHT 0.25 //in percent of the image diagonal
#define MAX_EDGE_DIFFERENCE 0.03 //in percent of the image diagonal

#define DOOR_SIZE_RATIO 4.91202 //squared height/width ratio of a DIN door

using namespace std;

struct _DoorCandidate
{
public:
    LineSegment top;
    LineSegment right;
    LineSegment bottom;
    LineSegment left;
};

class ARD_EXPORT DoorDetector
{
    cv::Ptr<cv::CLAHE> clahe;
    LSWMS *lswms;
    int diagonalLength;

    DoorDetector();
    void categorizeSegments(vector<LineSegment> &segments, vector<LineSegment> &horizontal, vector<LineSegment> &vertical);
    void enhanceEdges(cv::Mat &image);
    void joinSegments(vector<LineSegment> &segments, const cv::Mat &img, bool horizontal);
    void applyLswms(cv::Mat img, vector<LineSegment> &segments);
    void findSegments(cv::Mat &grayImg, vector<LineSegment> &horizontal, vector<LineSegment> &vertical);
    void growSegments(vector<LineSegment> &segments, int length);
    void findDoor(vector<LineSegment> &horizontal, vector<LineSegment> &vertical, vector<cv::Point> &corners);
    void findDoorCandidates(vector<LineSegment> &horizontal, vector<LineSegment> &vertical, vector<_DoorCandidate> &candidates);
    float evaluateCandidate(const _DoorCandidate &candidate, LinePoint &topLeft, LinePoint &topRight, LinePoint &bottomRight, LinePoint &bottomLeft);
    bool isTopDoorSegment(LineSegment &left, LineSegment &right, LineSegment &top, int minDoorSize);
    bool isBottomDoorSegment(LineSegment &left, LineSegment &right, LineSegment &bottom, int minDoorSize);
public:
    vector<LineSegment> horizontalSegments;
    vector<LineSegment> verticalSegments;
    vector<_DoorCandidate> candidates;
    DoorDetector(cv::Size inputSize);
    ~DoorDetector();
    bool findDoorCorners(const cv::Mat &grayImg, vector<cv::Point> &corners);
    void drawSegments(const cv::Mat &src, cv::Mat &dest, vector<LineSegment> &segments, const cv::Scalar &color);
};

#endif // DOORDETECTOR_H
