#include "VanishingPoint.hpp"
#include "LineSegment.hpp"
#include <cmath>

double VanishingPoint::distanceTo(const LineSegment &seg) const
{
    LinePoint projection = projectOn(seg);

    return std::sqrt((x - projection.x) * (x - projection.x) + (y - projection.y) * (y - projection.y));
}

LinePoint VanishingPoint::projectOn(const LineSegment &seg) const
{
    double vx = seg.end.x - seg.start.x;
    double vy = seg.end.y - seg.start.y;
    double wx = x - seg.start.x;
    double wy = y - seg.start.y;

    double c1 = wx * vx + wy * vy;
    double c2 = vx * vx + vy * vy;
    double b = c1 / c2;

    int px = LineSegment::round(seg.start.x + b * vx);
    int py = LineSegment::round(seg.start.y + b * vy);

    return LinePoint(px, py);
}
