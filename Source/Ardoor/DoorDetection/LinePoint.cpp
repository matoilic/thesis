#include <cmath>
#include "LinePoint.hpp"

double LinePoint::distanceFromOrigin() const
{
    return std::sqrt(x * x + y * y);
}

double LinePoint::distanceTo(const LinePoint &p) const
{
    int dx = x - p.x;
    int dy = y - p.y;

    return std::sqrt(dx * dx + dy * dy);
}
