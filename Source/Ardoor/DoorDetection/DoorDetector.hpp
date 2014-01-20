#ifndef DOORDETECTOR_H
#define DOORDETECTOR_H

#include <opencv2/core/core.hpp>
#include <Ardoor/Ardoor.hpp>
#include <Ardoor/Context/ArdoorContext.hpp>
#include "LSWMS.hpp"
#include "LineSegment.hpp"
#include "PHough.hpp"
#include "Timer.hpp"
#include "MSAC.hpp"

#define MIN_SEGMENT_LENGTH 0.1 //in percent of the image diagonal
#define MAX_SEGMENT_DISTANCE 0.05 //in percent of the image diagonal
#define MAX_SEGMENT_SHIFT 8 //in px
#define MAX_SEGMENT_GRADIENT_DIFFERENCE 0.08748 //tan(5°)
#define SEGMENT_GROWTH 0.05 //in percent of the image diagonal
#define MIN_DOOR_WIDTH 0.10 //in percent of the image diagonal
#define MIN_DOOR_HEIGHT 0.49 //in percent of the image diagonal
#define DOOR_SIZE_RATIO 4.91202 //squared height/width ratio of a DIN door
#define MAX_HORIZONTAL_DIVERGENCE 0.839
#define MAX_VERTICAL_DIVERGENCE 0.3491
#define MIN_LENGTH_CATEGORIZATION 0.1 //in percent of the image diagonal

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
    LSWMS *segmentDetector;
    MSAC *vanishingPointDetector;
//    PHough *segmentDetector;
    Timer timer;
    int diagonalLength;
    cv::Matx33f intrinsics;

    DoorDetector();
    void categorizeSegments(vector<LineSegment> &segments, vector<LineSegment> &horizontal, vector<LineSegment> &vertical);
    void enhanceEdges(cv::Mat &image);
    void joinSegments(vector<LineSegment> &segments, const cv::Mat &img, bool horizontal);
    void runSegmentDetection(cv::Mat img, vector<LineSegment> &segments);
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
