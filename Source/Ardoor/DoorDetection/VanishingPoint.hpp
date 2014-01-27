#ifndef VANISHINGPOINT_H
#define VANISHINGPOINT_H

#include <opencv2/core/core.hpp>
#include "LinePoint.hpp"

class LineSegment;

class VanishingPoint : public cv::Point_<int>
{
public:
    bool finite;
    VanishingPoint() { }
    VanishingPoint(int x, int y, bool f) : cv::Point_<int>(x, y), finite(f) { }
    double distanceTo(const LineSegment &seg) const;
    LinePoint projectOn(const LineSegment &seg) const;

    inline VanishingPoint& operator=(const VanishingPoint &p)
    {
        x = p.x;
        y = p.y;
        finite = p.finite;

        return *this;
    }
};

#endif // VANISHINGPOINT_H
