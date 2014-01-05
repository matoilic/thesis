#include "DoorDetector.h"
#include "opencv2/highgui/highgui.hpp"
#include "SegmentDistance.h"

DoorDetector::DoorDetector(cv::Size inputSize)
{
    clahe = cv::createCLAHE();
    clahe->setClipLimit(5);
    clahe->setTilesGridSize(cv::Size(8, 8));

    lswms = new LSWMS(inputSize, 3, MIN_SEGMENT_LENGTH, 10000);
    diagonalLength = sqrt(inputSize.width * inputSize.width + inputSize.height * inputSize.height);
}

DoorDetector::~DoorDetector()
{
    delete lswms;
}

void DoorDetector::enhanceEdges(cv::Mat &image)
{
    clahe->apply(image, image);
}

void DoorDetector::categorizeSegments(vector<LineSegment> &segments, vector<LineSegment> &horizontal, vector<LineSegment> &vertical)
{
    for(LineSegment seg: segments) {
        if(seg.isHorizontal()) {
            horizontal.push_back(seg);
        } else if(seg.isVertical()) {
            vertical.push_back(seg);
        }
    }
}

void DoorDetector::drawSegments(const cv::Mat &src, cv::Mat &dest, vector<LineSegment> &segments, const cv::Scalar &color)
{
    src.copyTo(dest);
    for(LineSegment seg: segments) {
        cv::line(dest, seg.start, seg.end, color, 1);
        cv::circle(dest, seg.start, 2, CV_RGB(0, 255, 0));
        cv::circle(dest, seg.end, 2, CV_RGB(0, 255, 255));
    }
}

void DoorDetector::joinSegments(vector<LineSegment> &segments, const cv::Mat &img, bool horizontal)
{
    bool foundMatching;
    double maxDistance = diagonalLength * MAX_SEGMENT_DISTANCE;
    auto comparator = [](const LineSegment& a, const LineSegment& b)->bool {
        return a.gradient() < b.gradient();
    };

    do {
        foundMatching = false;
        sort(segments.begin(), segments.end(), comparator);

        for(vector<LineSegment>::iterator left = segments.begin(); left < segments.end(); left++) {
            double leftGradient = left->gradient();

            for(vector<LineSegment>::iterator right = left + 1; right < segments.end(); right++) {
                if(abs(right->gradient() - leftGradient) > MAX_GRADIENT_DIFFERENCE) break;

                SegmentDistance dist = left->distanceTo(*right);

                if(dist.length < maxDistance) {
                    if(horizontal) {
                        if(dist.dy <= 8) {
                            left->joinWith(*right);
                            leftGradient = left->gradient();
                            segments.erase(right);
                            foundMatching = true;
                        }
                    } else {
                        if(dist.dx <= 8) {
                            left->joinWith(*right);
                            leftGradient = left->gradient();
                            segments.erase(right);
                            foundMatching = true;
                        }
                    }
                }
            }
        }
    } while(foundMatching);
}

void DoorDetector::applyLswms(cv::Mat img, vector<LineSegment> &segments)
{
    lswms->run(img, segments);
}

void DoorDetector::findSegments(cv::Mat &grayImg, vector<LineSegment> &horizontal, vector<LineSegment> &vertical)
{
    vector<LineSegment> segments;

    enhanceEdges(grayImg);
    applyLswms(grayImg, segments);

    categorizeSegments(segments, horizontal, vertical);
    joinSegments(horizontal, grayImg, true);
    joinSegments(vertical, grayImg, false);
}

void DoorDetector::growSegments(vector<LineSegment> &segments, int length)
{
    for(LineSegment &seg: segments) {
        double gradient = seg.gradient();
        double cos = std::cos(gradient), sin = std::sin(gradient);

        seg.start.x -= length * cos;
        seg.start.y -= length * sin;
        seg.end.x += length * cos;
        seg.end.y += length * sin;
    }
}

bool DoorDetector::isTopDoorSegment(LineSegment &left, LineSegment &right, LineSegment &top, int minDoorSize)
{
    return (
        top.start.x <= left.start.x &&
        top.start.y >= left.start.y &&
        top.start.y <= left.end.y - minDoorSize &&
        top.end.x >= right.start.x &&
        top.end.y >= right.start.y &&
        top.end.y <= right.end.y - minDoorSize
    );
}

bool DoorDetector::isBottomDoorSegment(LineSegment &left, LineSegment &right, LineSegment &bottom, int minDoorSize)
{
    return (
        bottom.start.x <= left.end.x &&
        bottom.start.y <= left.end.y &&
        bottom.start.y >= left.start.y + minDoorSize &&
        bottom.end.x >= right.end.x &&
        bottom.end.y <= right.end.y &&
        bottom.end.y >= right.start.y + minDoorSize
    );
}

void DoorDetector::findDoor(vector<LineSegment> &vertical, vector<LineSegment> &horizontal, vector<cv::Point> &corners)
{
    int lineGrowth = diagonalLength * LINE_GROWTH;
    growSegments(horizontal, lineGrowth);
    growSegments(vertical, lineGrowth);

    sort(horizontal.begin(), horizontal.end(), [](const LineSegment &a, const LineSegment &b) {
        return a.start.y < b.start.y;
    });

    sort(vertical.begin(), vertical.end(), [](const LineSegment &a, const LineSegment &b) {
        return a.start.x < b.start.x;
    });

    LineSegment *top = nullptr, *right = nullptr, *bottom = nullptr, *left = nullptr;
    int minDoorSize = diagonalLength * MIN_DOOR_WIDTH;
    int maxEdgeDifference = diagonalLength * MAX_EDGE_DIFFERENCE;

    for(vector<LineSegment>::iterator l = vertical.begin(); l < vertical.end(); l++) {
        if(l->length() >= minDoorSize) {
            for(vector<LineSegment>::iterator r = l + 1; r < vertical.end(); r++) {
                int width = l->distanceTo(*r).length;
                if(r->length() >= minDoorSize && width >= minDoorSize) {
                    for(vector<LineSegment>::iterator t = horizontal.begin(); t < horizontal.end(); t++) {
                        if(isTopDoorSegment(*l, *r, *t, minDoorSize)) {
                            for(vector<LineSegment>::iterator b = t + 1; b < horizontal.end(); b++) {
                                if(isBottomDoorSegment(*l, *r, *b, minDoorSize)) {
                                    bottom = &(*b);
                                    break;
                                }
                            }

                            if(bottom != nullptr) {
                                top = &(*t);
                                break;
                            }
                        }
                    }

                    if(bottom != nullptr) {
                        right = &(*r);
                        break;
                    }
                }
            }

            if(bottom != nullptr) {
                left = &(*l);
                break;
            }
        }
    }

    if(bottom != nullptr) {
        corners.push_back(top->intersectionPointWith(*left)); //top left
        corners.push_back(top->intersectionPointWith(*right)); //top right
        corners.push_back(bottom->intersectionPointWith(*left)); //bottom right
        corners.push_back(bottom->intersectionPointWith(*right)); //bottom left
    }
}

bool DoorDetector::findDoorCorners(const cv::Mat &grayImg, vector<cv::Point> &corners)
{
    assert(grayImg.type() == CV_8UC1);

    cv::Mat copy(grayImg);
    vector<LineSegment> horizontal, vertical;
    findSegments(copy, horizontal, vertical);
    findDoor(vertical, horizontal, corners);

    //for debugging
    cv::cvtColor(copy, copy, CV_GRAY2RGB);
    drawSegments(copy, copy, horizontal, CV_RGB(255, 0, 0));
    drawSegments(copy, copy, vertical, CV_RGB(0, 255, 0));
    cv::imshow("", copy);

    return corners.size() == 4;
}
