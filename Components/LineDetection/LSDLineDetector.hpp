#ifndef LSDLINEDETECTOR_HPP
#define LSDLINEDETECTOR_HPP

#include "LineDetector.hpp"


typedef ARD_EXPORT struct LSDLineSegment {
    double x1;
    double y1;
    double x2;
    double y2;
    double width;
    double p;
    double nfa;
} LSDLineSegment;


class ARD_EXPORT LSDLineDetector : public LineDetector
{
public:
     LineSegmentList detect(cv::Mat image);
};

#endif // LSDLINEDETECTOR_HPP
