#ifndef LINEPOINT_H
#define LINEPOINT_H

#include <opencv2/core/core.hpp>

class LinePoint : public cv::Point_<int>
{
public:
    LinePoint() { }
    LinePoint(int x, int y) : cv::Point_<int>(x, y) { }
    LinePoint(cv::Point &p) : cv::Point_<int>(p.x, p.y) { }
    double distanceFromOrigin() const;
    double distanceTo(const LinePoint &p) const;

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
