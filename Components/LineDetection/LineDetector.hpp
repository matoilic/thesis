#ifndef LINEDETECTOR_HPP
#define LINEDETECTOR_HPP

#include "ardoor.h"
#include "Types.hpp"

class ARD_EXPORT LineDetector
{
    virtual LineSegmentList detect(cv::Mat image) = 0;
};

#endif // LINEDETECTOR_HPP
