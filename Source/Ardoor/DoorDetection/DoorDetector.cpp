#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include "DoorDetector.hpp"
#include "SegmentDistance.hpp"

using namespace std;

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
            if(left->deleted) continue;
            double leftGradient = left->gradient();

            for(vector<LineSegment>::iterator right = left + 1; right < segments.end(); right++) {
                if(right->deleted) continue;
                if(abs(right->gradient() - leftGradient) > MAX_GRADIENT_DIFFERENCE) break;

                SegmentDistance dist = left->distanceTo(*right);

                if(dist.length < maxDistance) {
                    if(horizontal) {
                        if(dist.dy <= 8) {
                            left->joinWith(*right);
                            leftGradient = left->gradient();
                            right->deleted = true;
                            foundMatching = true;
                        }
                    } else {
                        if(dist.dx <= 8) {
                            left->joinWith(*right);
                            leftGradient = left->gradient();
                            right->deleted = true;
                            foundMatching = true;
                        }
                    }
                }
            }
        }
    } while(foundMatching);


    segments.erase(
        remove_if(segments.begin(), segments.end(), [](LineSegment &s) {
            return s.deleted;
        }),
        segments.end());
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

void DoorDetector::findDoorCandidates(vector<LineSegment> &horizontal, vector<LineSegment> &vertical, vector<_DoorCandidate> &candidates)
{
    _DoorCandidate candidate;

    int minDoorWidth = diagonalLength * MIN_DOOR_WIDTH;
    int minDoorHeight = diagonalLength * MIN_DOOR_HEIGHT;

    for(vector<LineSegment>::iterator l = vertical.begin(); l < vertical.end(); l++) {
        if(l->length() >= minDoorWidth) {
            candidate.left = *l;

            for(vector<LineSegment>::iterator r = l + 1; r < vertical.end(); r++) {
                int width = l->distanceTo(*r).length;
                if(r->length() >= minDoorWidth && width >= minDoorWidth) {
                    candidate.right = *r;

                    for(vector<LineSegment>::iterator t = horizontal.begin(); t < horizontal.end(); t++) {
                        if(isTopDoorSegment(*l, *r, *t, minDoorWidth)) {
                            candidate.top = *t;

                            for(vector<LineSegment>::iterator b = t + 1; b < horizontal.end(); b++) {
                                if(isBottomDoorSegment(*l, *r, *b, minDoorWidth)) {
                                    int height = t->distanceTo(*b).length;

                                    if(height >= minDoorHeight) {
                                        candidate.bottom = *b;
                                        candidates.push_back(candidate);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

float DoorDetector::evaluateCandidate(const _DoorCandidate &candidate, LinePoint &topLeft, LinePoint &topRight, LinePoint &bottomRight, LinePoint &bottomLeft)
{
    topLeft = candidate.top.intersectionPointWith(candidate.left);
    topRight = candidate.top.intersectionPointWith(candidate.right);
    bottomRight = candidate.bottom.intersectionPointWith(candidate.right);
    bottomLeft = candidate.bottom.intersectionPointWith(candidate.left);


    int width = ((topRight.x - topLeft.x) * (topRight.x - topLeft.x)) + ((topRight.y - topLeft.y) * (topRight.y - topLeft.y));
    int height = ((bottomLeft.x - topLeft.x) * (bottomLeft.x - topLeft.x)) + ((bottomLeft.y - topLeft.y) * (bottomLeft.y - topLeft.y));

    return abs((float)height / (float)width - DOOR_SIZE_RATIO);
}

void DoorDetector::findDoor(vector<LineSegment> &horizontal, vector<LineSegment> &vertical, vector<cv::Point> &corners)
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

    findDoorCandidates(horizontal, vertical, candidates);

    float bestScore = 10000.0f; //lower score is better
    for(_DoorCandidate candidate: candidates) {
        LinePoint tl, tr, br, bl;
        float score = evaluateCandidate(candidate, tl, tr, br, bl);
        if(score < bestScore) {
            bestScore = score;
            corners.clear();
            corners.reserve(4);
            corners.push_back(tl);
            corners.push_back(tr);
            corners.push_back(br);
            corners.push_back(bl);
        }
    }
}

bool DoorDetector::findDoorCorners(const cv::Mat &grayImg, vector<cv::Point> &corners)
{
    assert(grayImg.type() == CV_8UC1);

    candidates.clear();
    verticalSegments.clear();
    horizontalSegments.clear();

    cv::Mat copy(grayImg);
    findSegments(copy, horizontalSegments, verticalSegments);
    findDoor(horizontalSegments, verticalSegments, corners);

    return corners.size() == 4;
}
