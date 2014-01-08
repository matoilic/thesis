#include <cmath>
#include <stdio.h>
#include <iostream>
#include <assert.h>

#include "LineSegment.hpp"

SegmentDistance LineSegment::distanceTo(const LineSegment &s2) const
{
    float ux = dX();
    float uy = dY();
    float vx = s2.dX();
    float vy = s2.dY();
    float wx = start.x - s2.start.x;
    float wy = start.y - s2.start.y;

    float a = ux * ux + uy * uy;
    float b = ux * vx + uy * vy;
    float c = vx * vx + vy * vy;
    float d = ux * wx + uy * wy;
    float e = vx * wx + vy * wy;

    float D = a * c - b * b; // always >= 0
    float sc, sN, sD = D; // sc = sN / sD, default sD = D >= 0
    float tc, tN, tD = D; // tc = tN / tD, default tD = D >= 0

    // compute the line parameters of the two closest points
    if (D < F_ZERO) { // the lines are almost parallel
        sN = 0.0f; // force using point P0 on segment S1
        sD = 1.0f; // to prevent possible division by 0.0 later
        tN = e;
        tD = c;
    } else { // get the closest points on the infinite lines
        sN = (b * e - c * d);
        tN = (a * e - b * d);
        if (sN < 0.0) {        // sc < 0 => the s=0 edge is visible
            sN = 0.0;
            tN = e;
            tD = c;
        } else if (sN > sD) {  // sc > 1  => the s=1 edge is visible
            sN = sD;
            tN = e + b;
            tD = c;
        }
    }

    if (tN < 0.0) { // tc < 0 => the t=0 edge is visible
        tN = 0.0;
        // recompute sc for this edge
        if (-d < 0.0) {
            sN = 0.0;
        } else if (-d > a) {
            sN = sD;
        } else {
            sN = -d;
            sD = a;
        }
    } else if (tN > tD) { // tc > 1  => the t=1 edge is visible
        tN = tD;
        // recompute sc for this edge
        if ((-d + b) < 0.0) {
            sN = 0;
        } else if ((-d + b) > a) {
            sN = sD;
        } else {
            sN = (-d +  b);
            sD = a;
        }
    }

    // finally do the division to get sc and tc
    sc = (abs(sN) < F_ZERO ? 0.0 : sN / sD);
    tc = (abs(tN) < F_ZERO ? 0.0 : tN / tD);

    LinePoint p0(round(tc * vx), round(tc * vy));
    LinePoint p1(round(wx + sc * ux), round(wy + sc * uy));

    return SegmentDistance(p0, p1);
}

LinePoint LineSegment::intersectionPointWith(const LineSegment &s2) const
{
    float ux = dX();
    float uy = dY();
    float vx = s2.dX();
    float vy = s2.dY();
    float wx = start.x - s2.start.x;
    float wy = start.y - s2.start.y;
    float D = ux * vy - vx * uy;

    float sI = (vx * wy - wx * vy) / D;

    LinePoint intersection(start.x + sI * ux, start.y + sI * uy);

    return intersection;
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

double LineSegment::length() const
{
    int dx = dX();
    int dy = dY();

    return std::sqrt(dx * dx + dy * dy);
}

void LineSegment::joinWith(const LineSegment second)
{    
    if(isHorizontal()) {
        //asert(second.isHorizontal());

        if(start.x < second.start.x && end.x > second.end.x) return;

        if(second.start.x < start.x && second.end.x > end.x) {
            start = second.start;
            end = second.end;

            return;
        }
    } else {
        //asert(second.isVertical());

        if(start.y < second.start.y && end.y > second.end.y) return;

        if(second.start.y < start.y && second.end.y > end.y) {
            start = second.start;
            end = second.end;

            return;
        }
    }

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
        newStart.y = round(thisWeight * start.y + secondWeight * second.start.y);
        newEnd.y = round(thisWeight * end.y + secondWeight * second.end.y);
    } else {
        newStart.x = round(thisWeight * start.x + secondWeight * second.start.x);
        newEnd.x = round(thisWeight * end.x + secondWeight * second.end.x);
    }

    start = newStart;
    end = newEnd;
}

double LineSegment::round(double number)
{
    return std::floor(number + 0.5);
}
