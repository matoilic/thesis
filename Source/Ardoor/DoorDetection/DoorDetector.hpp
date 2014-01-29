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

#define MIN_SEGMENT_LENGTH 0.05 //in percent of the image diagonal
#define MAX_SEGMENT_DISTANCE 0.05 //in percent of the image diagonal
#define MAX_SEGMENT_SHIFT 8 //in px
#define MAX_SEGMENT_GRADIENT_DIFFERENCE 0.08748 //tan(5°)
#define SEGMENT_GROWTH 0.2 //in percent of the image diagonal
#define MIN_DOOR_WIDTH 0.10 //in percent of the image diagonal
#define MIN_DOOR_HEIGHT 0.20 //in percent of the image diagonal
#define MAX_DOOR_HEIGHT 0.40 //in percent of the image diagonal
//#define DOOR_SIZE_RATIO 4.91202 //squared height/width ratio of a common door
#define DOOR_SIZE_RATIO 2.05 //squared height/width ratio of a common door
#define MAX_DOOR_SIZE_RATIO 6.5 //squared height/width ratio of a common door
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
    float ratio;
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
    cv::Size inputSize;

    DoorDetector();
    /**
     * Categorizes the given segments into horizontal and vertical by detecting the respective vanishing points.
     * Segments that don't fall in any of the two categories are ignored.
     */
    void categorizeSegments(vector<LineSegment> &segments, vector<LineSegment> &horizontal, vector<LineSegment> &vertical);
    /**
     * Enhances the given image to improve edgde detection results
     */
    void enhanceEdges(cv::Mat &image);
    /**
     * Optimizes the given segments by merging similar and close segments
     */
    void joinSegments(vector<LineSegment> &segments, const cv::Mat &img, bool horizontal);
    /**
     * Runs the segment detection and optimization process
     */
    void runSegmentDetection(cv::Mat img, vector<LineSegment> &segments);
    /**
     * Searches for horizontal and vertical line segments wihint the given gray scale image
     */
    void findSegments(cv::Mat &grayImg, vector<LineSegment> &horizontal, vector<LineSegment> &vertical);
    /**
     * Strechtes the given segment by length
     */
    void growSegments(vector<LineSegment> &segments, int length);
    /**
     * Runs the door detection and evaluation process, writing the best match into corners (Top Left, Top Right, Bottom Right, Bottom Left)
     */
    void findDoor(vector<LineSegment> &horizontal, vector<LineSegment> &vertical, vector<cv::Point> &corners);
    /**
     * Searches for possible doors that can be constructed using the given segments
     */
    void findDoorCandidates(vector<LineSegment> &horizontal, vector<LineSegment> &vertical, vector<_DoorCandidate> &candidates);
    /**
     * Evaluates the given door candidate and returns it's corners
     */
    float evaluateCandidate(const _DoorCandidate &candidate, LinePoint &topLeft, LinePoint &topRight, LinePoint &bottomRight, LinePoint &bottomLeft);
    /**
     * Evaluates if the given top segment is a possible door segment given the left and right edges and the expected door size
     */
    bool isTopDoorSegment(LineSegment &left, LineSegment &right, LineSegment &top, int minDoorSize);
    /**
     * Evaluates if the given bottom segment is a possible door segment given the left and right edges and the expected door size
     */
    bool isBottomDoorSegment(LineSegment &left, LineSegment &right, LineSegment &bottom, int minDoorSize);
    /**
     * Calculates the four intersection points of the four given edges. Returns true if successful, false otherwise
     */
    bool getCornersFromEdges(const LineSegment &left, const LineSegment &top, const LineSegment &right, const LineSegment &bottom, LinePoint &tl, LinePoint &tr, LinePoint &br, LinePoint &bl);
public:
    /**
     * These variables hold the results of the latest detection run for debugging purposes
     */
    vector<LineSegment> horizontalSegments;
    vector<LineSegment> verticalSegments;
    vector<_DoorCandidate> candidates;

    /**
     * @param inputSize the size of the input image
     * @param context the current application context
     */
    DoorDetector(cv::Size inputSize, ArdoorContext *context);
    virtual ~DoorDetector();
    /**
     * Runs the door detection on the given grayscale image and writes the corners of the match into corners.
     * Return true if a door was found, false otherwise.
     */
    bool findDoorCorners(const cv::Mat &grayImg, vector<cv::Point> &corners);
    /**
     * Helper function for debugging puposes to drae line segment on a image.
     */
    void drawSegments(const cv::Mat &src, cv::Mat &dest, vector<LineSegment> &segments, const cv::Scalar &color);
};

#endif // DOORDETECTOR_H
