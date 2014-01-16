#ifndef PHOUGH_HPP
#define PHOUGH_HPP

#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "LineSegment.hpp"
#include "LinePoint.hpp"

using namespace std;

class PHough
{
    int _minSegmentLength;
public:
    PHough(int minSegmentLength) { _minSegmentLength = minSegmentLength * minSegmentLength; }
    void run(const cv::Mat &img, vector<LineSegment> &segments);
};

#endif // PHOUGH_HPP
