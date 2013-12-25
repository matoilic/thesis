#include "linepoint.h"
#include <cmath>

double LinePoint::distanceFromOrigin() const
{
    return std::sqrt(x * x + y * y);
}
