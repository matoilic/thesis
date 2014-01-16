#include "PHough.hpp"
#include <opencv2/highgui/highgui.hpp>

void PHough::run(const cv::Mat &img, vector<LineSegment> &segments)
{
    cv::Mat edges(img.cols, img.rows, img.type());
    cv::medianBlur(edges, edges, 5);

    cv::Canny(img, edges, 20, 80, 3);

//    cv::Point anchor(-1,-1);
//    cv::Size ksize(5,5);
//    cv::boxFilter(img, edges, -1, ksize, anchor, true, cv::BORDER_REFLECT);
//    edges = (img - edges) * 3;

    vector<cv::Vec4i> lines;
    cv::HoughLinesP(edges, lines, 1, CV_PI / 180, 80, 30, 10);

    for(cv::Vec4i &line: lines) {
        LinePoint start(line[0], line[1]);
        LinePoint end(line[2], line[3]);

        LineSegment segment(start, end);

        int deltaX = segment.dX();
        int deltaY = segment.dY();

        if((deltaX < 0 && segment.isHorizontal()) || (deltaY < 0 && segment.isVertical())) {
            LinePoint start = segment.start;
            segment.start = segment.end;
            segment.end = start;
        }

        int segLength = deltaX * deltaX + deltaY * deltaY;

        if(segLength > _minSegmentLength) {
            segments.push_back(segment);
        }
    }
}
