#include <cmath>
#include <stdio.h>
#include <iostream>

#include "LineSegment.h"

double LineSegment::distancePointToLine(double x0, double y0, double x1, double y1, double x2, double y2) const
{
    double dx = x2 - x1, dy = y2 - y1;
    double t = ((x0 - x1) * dx + (y0 - y1) * dy) / (dx * dx + dy * dy);

    //point on line segment with the shortest distance to this point
    double lx = x1 + t * dx;
    double ly = y1 + t * dy;

    dx = lx - x0;
    dy = ly - y0;

    return sqrt(dx * dx + dy * dy);
}

double LineSegment::distancePointToPoint(double x0, double y0, double x1, double y1) const
{
    double dx = x1 - x0;
    double dy = y1 - y0;

    return std::sqrt(dx * dx + dy * dy);
}

SegmentDistance LineSegment::distanceTo(const LineSegment &s2) const
{
    //scale all coordinates to a value between 0 and 1
    int maxX = std::max(std::max(std::max(start.x, end.x), s2.start.x), s2.end.x);
    int maxY = std::max(std::max(std::max(start.y, end.y), s2.start.y), s2.end.y);
    double scale = std::max(maxX, maxY);

    double x0 = start.x / scale;
    double y0 = start.y / scale;
    double x1 = end.x / scale;
    double y1 = end.y / scale;

    double x2 = s2.start.x / scale;
    double y2 = s2.start.y / scale;
    double x3 = s2.end.x / scale;
    double y3 = s2.end.y / scale;

    double t = (y3 - y2) * (x1 - x0) - (x3 - x2) * (y1 - y0);
    double a = (x3 - x2) * (y0 - y2) - (y3 - y2) * (x0 - x2);
    double b;
    double xa, ya, xb, yb; //coords of the points that build the shortest distance

    //neither parallel nor collinear
    if(t != 0) {
        a /= t;
        b = ((x1 - x0) * (y0 - y2) - (y1 - y0) * (x0 - x2)) / t;

        t = std::min(std::max(0.0, a), 1.0);
        xa = x0 + t * (x1 - x0);
        ya = y0 + t * (y1 - y0);

        t = std::min(std::max(0.0, distancePointToLine(xa, ya, x2, y2, x3, y3)), 1.0);
        xa = x2 + t * (x3 - x2);
        ya = y2 + t * (y3 - y2);

        t = std::min(std::max(0.0, b), 1.0);
        xb = x2 + t * (x3 - x2);
        yb = y2 + t * (y3 - y2);

        t = std::min(std::max(0.0, distancePointToLine(xb, yb, x0, y0, x1, y1)), 1.0);
        xb = x0 + t * (x1-x0);
        yb = y0 + t * (y1-y0);

        return SegmentDistance(LinePoint(xa * scale, ya * scale), LinePoint(xb * scale, yb * scale));
    }

    //parallel
    if(a != 0) {
        a = distancePointToLine(x0, y0, x2, y2, x3, y3);

        if(a <= 1 && a >= 0) {
            t = a;
            xa = x2 + t * (x3 - x2);
            ya = y2 + t * (y3 - y2);
            xb = x0;
            yb = y0;
        } else {
            t = std::min(std::max(0.0, a), 1.0);
            xa = x2 + t * (x3 - x2);
            ya = y2 + t * (y3 - y2);
            t = std::min(std::max(0.0, distancePointToLine(xa, ya, x0, y0, x1, y1)), 1.0);
            xb = x0 + t * (x1 - x0);
            yb = y0 + t * (y1 - y0);
        }

        SegmentDistance dist = SegmentDistance(LinePoint(xa * scale, ya * scale), LinePoint(xb * scale, yb * scale));
        dist.areParallel = true;

        return dist;
    }

    double endA = end.distanceFromOrigin(), endB = s2.end.distanceFromOrigin();
    if(endA > endB) {
        SegmentDistance dist = SegmentDistance(LinePoint(s2.end.x, s2.end.y), LinePoint(start.x, start.y));
        dist.areCollinear = true;

        return dist;
    } else {
        SegmentDistance dist = SegmentDistance(LinePoint(end.x, end.y), LinePoint(s2.start.x, s2.start.y));
        dist.areCollinear = true;

        return dist;
    }
}

double LineSegment::gradient() const
{
    if(dX() == 0) return PI_HALF;

    return std::atan(std::abs((double)dY() / dX()));
}

int LineSegment::dX() const
{
    return end.x - start.x;
}

int LineSegment::dY() const
{
    return end.y - start.y;
}

bool LineSegment::isHorizontal() const
{
    return (gradient() <= MAX_GRADIENT);
}

bool LineSegment::isVertical() const
{
    return (gradient() >= PI_HALF - MAX_GRADIENT);
}

/*bool LineSegment::isJoinableWith(const LineSegment &second)
{
    return (std::abs(second.gradient() - gradient()) <= MAX_GRADIENT_DIFFERENCE);
}*/

double LineSegment::length() const
{
    int dx = dX();
    int dy = dY();

    return std::sqrt(dx * dx + dy * dy);
}

void LineSegment::joinWith(const LineSegment second)
{
    cv::Point newStart, newEnd;

    newStart = (second.start.distanceFromOrigin() < start.distanceFromOrigin()) ? second.start : start;
    newEnd = (second.end.distanceFromOrigin() > end.distanceFromOrigin()) ? second.end : end;

    double thisWeight, secondWeight, thisLength = length(), secondLength = second.length();

    if(thisLength > secondLength) {
        secondWeight = secondLength / thisLength;
        thisWeight = 1 - secondWeight;
    } else {
        thisWeight = thisLength / secondLength;
        secondWeight = 1 - thisWeight;
    }

    if(isHorizontal()) {
        newStart.y = std::round(thisWeight * start.y + secondWeight * second.start.y);
        newEnd.y = std::round(thisWeight * end.y + secondWeight * second.end.y);
    } else {
        newStart.x = std::round(thisWeight * start.x + secondWeight * second.start.x);
        newEnd.x = std::round(thisWeight * end.x + secondWeight * second.end.x);
    }

    start = newStart;
    end = newEnd;
}
