#include "LSDLineDetector.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

extern "C"
{
    #include "lsd.h"
}

LineSegmentList LSDLineDetector::detect(cv::Mat image)
{
    cv::Mat grey;
    cv::cvtColor(image, grey, CV_RGB2GRAY);

    int width = grey.size().width;
    int height = grey.size().height;

    // convert opencv image to LSD structure
    double *lsdImage = new double[width * height];
    uchar* data = (uchar*) grey.data;
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            lsdImage[j + i * width] = data[j + i * width];
        }
    }

    // run LSD
    int numDetections = 0;
    double* detections = lsd(&numDetections, lsdImage, width, height);
    delete lsdImage;


    LineSegmentList segments;
    LSDLineSegment* segment;
    for (int i = 0; i < numDetections; i++) {
        segment = reinterpret_cast<LSDLineSegment*>( &detections[7 * i] );
        segments.push_back(LineSegment(Point(segment->x1, segment->y1), Point(segment->x2, segment->y2)));
    }

    return segments;
}
