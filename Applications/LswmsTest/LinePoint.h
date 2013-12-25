#include <opencv2/core/core.hpp>
#include "LineSegment.h"

#ifndef LINEPOINT_H
#define LINEPOINT_H
class LineSegment;
class LinePoint : public cv::Point_<int>
{
public:
    LinePoint() { }
    LinePoint(int x, int y) : cv::Point_<int>(x, y) { }
    double distanceFromOrigin() const;

    inline LinePoint& operator=(const LinePoint &p)
    {
        x = p.x;
        y = p.y;

        return *this;
    }

    inline LinePoint& operator=(const cv::Point &p)
    {
        x = p.x;
        y = p.y;

        return *this;
    }
};

#endif // LINEPOINT_H
