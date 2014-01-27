#ifndef LINE_SEGMENT_H
#define LINE_SEGMENT_H

#include <opencv2/core/core.hpp>
#include "LinePoint.hpp"
#include "SegmentDistance.hpp"
#include "VanishingPoint.hpp"

#define MAX_GRADIENT 0.14054 //tan(15°)
#define MAX_GRADIENT_DIFFERENCE 0.2618 //tan(5°)
#define PI_HALF 1.57079632679
#define F_ZERO 0.00000001

class LineSegment
{
public:
    LinePoint start;
    LinePoint end;
    bool deleted;
    float error;
    bool horizontal;
    bool vertical;
    VanishingPoint vanishingPoint;

    LineSegment() : deleted(false), horizontal(false), vertical(false) { }
    LineSegment(LinePoint s, LinePoint e) : start(s), end(e), deleted(false), horizontal(false), vertical(false) { }

    SegmentDistance distanceTo(const LineSegment &s2) const;
    LinePoint intersectionPointWith(const LineSegment &s2) const;
    int dX() const;
    int dY() const;
    double gradient() const;
    bool isHorizontal() const;
    bool isVertical() const;
    void joinWith(const LineSegment second);
    double length() const;
    void alignToVanishingPoint();
    static double round(double number);
};

class GradientComparator
{
public:
    bool operator()(const LineSegment &s1, const LineSegment &s2) const
    {
        return abs(s1.gradient()) < abs(s2.gradient());
    }
};

#endif // LINE_SEGMENT_H
