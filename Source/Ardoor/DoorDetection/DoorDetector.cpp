#include <iostream>
#include <cmath>
#include <opencv2/highgui/highgui.hpp>
#include <Ardoor/Projection/ProjectionUtil.hpp>
#include "DoorDetector.hpp"
#include "SegmentDistance.hpp"
#include "VanishingPoint.hpp"

using namespace std;

DoorDetector::DoorDetector(cv::Size inputSize, ArdoorContext *context)
{
    this->inputSize = inputSize;
    clahe = cv::createCLAHE();
    clahe->setClipLimit(5);
    clahe->setTilesGridSize(cv::Size(8, 8));

    diagonalLength = sqrt(inputSize.width * inputSize.width + inputSize.height * inputSize.height);
    segmentDetector = new LSWMS(inputSize, 3, diagonalLength * MIN_SEGMENT_LENGTH, 10000);
//    segmentDetector = new PHough(diagonalLength * MIN_SEGMENT_LENGTH);
    vanishingPointDetector = new MSAC();
    vanishingPointDetector->init(MODE_NIETO, inputSize, true);

    intrinsics = context->getCameraConfiguration(inputSize.width, inputSize.height).getIntrinsics();
}

DoorDetector::~DoorDetector()
{
    delete segmentDetector;
    delete vanishingPointDetector;
}

void DoorDetector::enhanceEdges(cv::Mat &image)
{
    timer.start();
    clahe->apply(image, image);
    timer.stop();
    timer.show("enhanceEdges");
}

void DoorDetector::categorizeSegments(vector<LineSegment> &segments, vector<LineSegment> &horizontal, vector<LineSegment> &vertical)
{
    /*int hCount = 0, vCount = 0;
    float horizontalAngle = 0, verticalAngle = 0;
    for(LineSegment seg: segments) {
        double grad = abs(seg.gradient());

        if(seg.length() < diagonalLength * MIN_LENGTH_CATEGORIZATION) continue;

        if(grad <= MAX_HORIZONTAL_DIVERGENCE) {
            hCount++;
            horizontalAngle += grad;
        } else if(grad >= PI_HALF - MAX_VERTICAL_DIVERGENCE) {
            vCount++;
            verticalAngle += grad;
        }
    }

    horizontalAngle /= hCount;
    verticalAngle /= vCount;

    for(LineSegment seg: segments) {
        seg.horizontalAngle = horizontalAngle;
        seg.verticalAngle = verticalAngle;
        if(seg.isHorizontal()) {
            horizontal.push_back(seg);
        } else if(seg.isVertical()) {
            vertical.push_back(seg);
        }
    }*/    

    vector<VanishingPoint> vps;
    vector<int> numInliers;
    vector<vector<LineSegment>> lineSegmentsClusters;
    VanishingPoint horizontalVp, verticalVp;

    timer.start();
    vanishingPointDetector->multipleVPEstimation(segments, lineSegmentsClusters, numInliers, vps, 2);
    timer.stop();
    timer.show("VP detection");

    cout << "VP" << endl;
    cout << vps.size() << endl;
    cout << vps[1].x << endl;

    if(abs(vps[0].y) < abs(vps[1].y)) {
        horizontal = lineSegmentsClusters[0];
        vertical = lineSegmentsClusters[1];
        horizontalVp = vps[0];
        verticalVp = vps[1];
    } else {
        horizontal = lineSegmentsClusters[1];
        vertical = lineSegmentsClusters[0];
        horizontalVp = vps[1];
        verticalVp = vps[0];
    }

    for(vector<LineSegment>::iterator s = horizontal.begin(); s < horizontal.end(); s++) {
        s->horizontal = true;
        s->vanishingPoint = horizontalVp;

        if(s->dX() < 0) {
            LinePoint start = s->start;
            s->start = s->end;
            s->end = start;
        }

        s->alignToVanishingPoint();
    }

    for(vector<LineSegment>::iterator s = vertical.begin(); s < vertical.end(); s++) {
        s->vertical = true;
        s->vanishingPoint = verticalVp;

        if(s->dY() < 0) {
            LinePoint start = s->start;
            s->start = s->end;
            s->end = start;
        }

        s->alignToVanishingPoint();
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
        return a.start.distanceFromOrigin() < b.start.distanceFromOrigin();
    };

    do {
        foundMatching = false;
        sort(segments.begin(), segments.end(), comparator);

        for(vector<LineSegment>::iterator left = segments.begin(); left < segments.end(); left++) {
            if(left->deleted) continue;
            double leftGradient = left->gradient();

            for(vector<LineSegment>::iterator right = left + 1; right < segments.end(); right++) {
                if(right->deleted) continue;
                if(abs(right->gradient() - leftGradient) > MAX_SEGMENT_GRADIENT_DIFFERENCE) continue;

                SegmentDistance dist = left->distanceTo(*right);

                if(dist.length < maxDistance) {
                    if(horizontal) {
                        if(dist.dy <= MAX_SEGMENT_SHIFT) {
                            left->joinWith(*right);
                            left->alignToVanishingPoint();
                            leftGradient = left->gradient();
                            right->deleted = true;
                            foundMatching = true;
                        }
                    } else {
                        if(dist.dx <= MAX_SEGMENT_SHIFT) {
                            left->joinWith(*right);
                            left->alignToVanishingPoint();
                            leftGradient = left->gradient();
                            right->deleted = true;
                            foundMatching = true;
                        }
                    }
                } else {
                    break;
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

void DoorDetector::runSegmentDetection(cv::Mat img, vector<LineSegment> &segments)
{
    timer.start();
    segmentDetector->run(img, segments);
    timer.stop();
    timer.show("segmentDetector");

    /*cv::Mat copy;
    cv::cvtColor(img, copy, CV_GRAY2RGB);
    drawSegments(copy, copy, segments, CV_RGB(255, 255, 0));
    cv::imshow("lswm", copy);*/
}

void DoorDetector::findSegments(cv::Mat &grayImg, vector<LineSegment> &horizontal, vector<LineSegment> &vertical)
{
    vector<LineSegment> segments;

    enhanceEdges(grayImg);
    runSegmentDetection(grayImg, segments);

    cv::Mat copy;
    cv::cvtColor(grayImg, copy, CV_GRAY2RGB);
    drawSegments(copy, copy, segments, CV_RGB(255, 255, 0));
    cout << segments.size() << endl;
    cv::imshow("raw segments", copy);

    timer.start();
    categorizeSegments(segments, horizontal, vertical);
    timer.stop();
    timer.show("categorizeSegments");
    cv::cvtColor(grayImg, copy, CV_GRAY2RGB);
    drawSegments(copy, copy, horizontal, CV_RGB(255, 0, 0));
    drawSegments(copy, copy, vertical, CV_RGB(0, 255, 0));
    cv::imshow("categorized segments", copy);

    timer.start();
    joinSegments(horizontal, grayImg, true);
    joinSegments(vertical, grayImg, false);
    timer.stop();
    timer.show("joinSegments");

    cv::cvtColor(grayImg, copy, CV_GRAY2RGB);
    drawSegments(copy, copy, horizontal, CV_RGB(255, 0, 0));
    drawSegments(copy, copy, vertical, CV_RGB(0, 255, 0));
    cv::imshow("joined segments", copy);
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
        top.end.x >= right.start.x &&
        top.end.y >= right.start.y
    );
}

bool DoorDetector::isBottomDoorSegment(LineSegment &left, LineSegment &right, LineSegment &bottom, int minDoorSize)
{
    return (
        bottom.start.x <= left.end.x &&
        bottom.start.y <= left.end.y &&
        bottom.end.x >= right.end.x &&
        bottom.end.y <= right.end.y
    );
}

void DoorDetector::findDoorCandidates(vector<LineSegment> &horizontal, vector<LineSegment> &vertical, vector<_DoorCandidate> &candidates)
{
    _DoorCandidate candidate;

    int minDoorWidth = diagonalLength * MIN_DOOR_WIDTH;
    int minDoorHeight = diagonalLength * MIN_DOOR_HEIGHT;
    int maxDoorHeight = diagonalLength * MAX_DOOR_HEIGHT;
    float doorRatio;
    int doorHeight;
    LinePoint tl, tr, br, bl;

    for(vector<LineSegment>::iterator l = vertical.begin(); l < vertical.end(); l++) {
        if(l->length() >= minDoorHeight) {
            candidate.left = *l;

            for(vector<LineSegment>::iterator r = l + 1; r < vertical.end(); r++) {
                if(r->length() >= minDoorHeight) {
                    candidate.right = *r;

                    for(vector<LineSegment>::iterator t = horizontal.begin(); t < horizontal.end(); t++) {
                        if(isTopDoorSegment(*l, *r, *t, minDoorWidth)) {
                            candidate.top = *t;

                            for(vector<LineSegment>::iterator b = t + 1; b < horizontal.end(); b++) {
                                if(isBottomDoorSegment(*l, *r, *b, minDoorWidth)) {
                                    if(getCornersFromEdges(*l, *t, *r, *b, tl, tr, br, bl)) {
                                        doorHeight = max(tl.distanceTo(bl), tr.distanceTo(br));

                                        if(doorHeight >= minDoorHeight && doorHeight < maxDoorHeight) {
                                            doorRatio = 1.0f / ProjectionUtil::getRatio(intrinsics, tl, tr, br, bl);

                                            if(doorRatio < MAX_DOOR_SIZE_RATIO) {
                                                candidate.bottom = *b;
                                                candidate.ratio = doorRatio;
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
    }
}

float DoorDetector::evaluateCandidate(const _DoorCandidate &candidate, LinePoint &topLeft, LinePoint &topRight, LinePoint &bottomRight, LinePoint &bottomLeft)
{
    getCornersFromEdges(candidate.left, candidate.top, candidate.right, candidate.bottom, topLeft, topRight, bottomRight, bottomLeft);
//    int width = ((topRight.x - topLeft.x) * (topRight.x - topLeft.x)) + ((topRight.y - topLeft.y) * (topRight.y - topLeft.y));
//    int height = ((bottomLeft.x - topLeft.x) * (bottomLeft.x - topLeft.x)) + ((bottomLeft.y - topLeft.y) * (bottomLeft.y - topLeft.y));

    return abs(candidate.ratio - DOOR_SIZE_RATIO);
}

void DoorDetector::findDoor(vector<LineSegment> &horizontal, vector<LineSegment> &vertical, vector<cv::Point> &corners)
{    
    int lineGrowth = diagonalLength * SEGMENT_GROWTH;
    timer.start();
    growSegments(horizontal, lineGrowth);
    growSegments(vertical, lineGrowth);
    timer.stop();
    timer.show("growSegments");

    timer.start();
    sort(horizontal.begin(), horizontal.end(), [](const LineSegment &a, const LineSegment &b) {
        return a.start.y < b.start.y;
    });

    sort(vertical.begin(), vertical.end(), [](const LineSegment &a, const LineSegment &b) {
        return a.start.x < b.start.x;
    });

    findDoorCandidates(horizontal, vertical, candidates);
    timer.stop();
    timer.show("findDoorCandidates");

    timer.start();
    float bestScore = 10000.0f; //lower score is better
    for(_DoorCandidate &candidate: candidates) {
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
    timer.stop();
    timer.show("evaluation");
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

    cv::Mat copy2(grayImg);
    cv::cvtColor(grayImg, copy2, CV_GRAY2RGB);
    drawSegments(copy2, copy2, horizontalSegments, CV_RGB(255, 0, 0));
    drawSegments(copy2, copy2, verticalSegments, CV_RGB(0, 255, 0));
    cv::imshow("grown segments", copy2);

    return corners.size() == 4;
}

bool DoorDetector::getCornersFromEdges(const LineSegment &left, const LineSegment &top, const LineSegment &right, const LineSegment &bottom, LinePoint &tl, LinePoint &tr, LinePoint &br, LinePoint &bl)
{
    tl = top.intersectionPointWith(left);
    tr = top.intersectionPointWith(right);
    br = bottom.intersectionPointWith(right);
    bl = bottom.intersectionPointWith(left);

    int mx = inputSize.width, my = inputSize.height;

    return (
        tl.x >= 0 && tl.x <= mx && tl.y >= 0 && tl.y <= my &&
        tr.x >= 0 && tr.x <= mx && tr.y >= 0 && tr.y <= my &&
        br.x >= 0 && br.x <= mx && br.y >= 0 && br.y <= my &&
        bl.x >= 0 && bl.x <= mx && bl.y >= 0 && bl.y <= my
    );
}
