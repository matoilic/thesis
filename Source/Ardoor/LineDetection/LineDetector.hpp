#ifndef LINEDETECTOR_HPP
#define LINEDETECTOR_HPP

#include <Ardoor/Ardoor.hpp>
#include <Ardoor/LineDetection/Types.hpp>

class ARD_EXPORT LineDetector
{
    virtual LineSegmentList detect(cv::Mat image) = 0;
};

#endif // LINEDETECTOR_HPP
