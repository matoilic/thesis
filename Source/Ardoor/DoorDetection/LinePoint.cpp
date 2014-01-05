#include <cmath>
#include "LinePoint.hpp"

double LinePoint::distanceFromOrigin() const
{
    return std::sqrt(x * x + y * y);
}
