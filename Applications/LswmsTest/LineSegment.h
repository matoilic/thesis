#ifndef LINE_SEGMENT_H
#define LINE_SEGMENT_H

#include <opencv2/core/core.hpp>
#include "LinePoint.h"
#include "SegmentDistance.h"

#define MAX_GRADIENT 0.267949192 //tan(15°)
#define MAX_GRADIENT_DIFFERENCE 0.2618 //tan(5°)
#define PI_HALF 1.57079632679
#define F_ZERO 0.00000001

class LinePoint;
class LineSegment
{
    double distancePointToLine(double x0, double y0, double x1, double y1, double x2, double y2) const;
    double distancePointToPoint(double x0, double y0, double x1, double y1) const;
public:
    LinePoint start;
    LinePoint end;
    float error;

    LineSegment() { }
    LineSegment(LinePoint s, LinePoint e) : start(s), end(e) { }

    SegmentDistance distanceTo(const LineSegment &s2) const;
    LinePoint intersectionPointWith(const LineSegment &s2) const;
    int dX() const;
    int dY() const;
    double gradient() const;
    bool isHorizontal() const;
    bool isVertical() const;
    void joinWith(const LineSegment second);
    double length() const;
};

class GradientComparator
{
public:
    bool operator()(const LineSegment &s1, const LineSegment &s2) const
    {
        return s1.gradient() < s2.gradient();
    }
};

#endif // LINE_SEGMENT_H
