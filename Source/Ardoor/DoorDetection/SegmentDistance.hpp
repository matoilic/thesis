#ifndef SEGMENTDISTANCE_H
#define SEGMENTDISTANCE_H

#include <cmath>
#include "LinePoint.hpp"

class SegmentDistance
{
public:
    LinePoint start;
    LinePoint end;

    int dx;
    int dy;

    double length;

    bool areCollinear;
    bool areParallel;
    bool areIntersecting;

    SegmentDistance(LinePoint s, LinePoint e) : start(s), end(e) {
        dx = std::abs(end.x - start.x);
        dy = std::abs(end.y - start.y);
        length = std::sqrt(dx * dx + dy * dy);
    }
};

#endif // SEGMENTDISTANCE_H
