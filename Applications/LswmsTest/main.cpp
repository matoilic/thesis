#ifdef WIN32
    #include <windows.h>
    #include <time.h>
#else
    #include <stdio.h>
    #include <sys/time.h>
    #include <time.h>
#endif

#include <libgen.h>
#include <cmath>
#include <iostream>
#include <queue>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "LSWMS.h"
#include "SegmentDistance.h"

using namespace std;

// Timing
#ifdef WIN32
    double t1, t2;
#else
    int t1, t2;
    struct timeval ts;
#endif
double t;

void enhanceEdges(cv::Mat &image, int kernelsize, float gain)
{
    cv::Mat blurredImage(image.cols, image.rows, image.type());

    cv::Point anchor(-1,-1);
    cv::Size ksize(kernelsize, kernelsize);

    cv::medianBlur(image, image, kernelsize);
    cv::boxFilter(image, blurredImage, -1, ksize, anchor, true, cv::BORDER_REFLECT);

    cv::Mat edgeImage = image - blurredImage;

    image = image + gain * edgeImage;
}

void categorizeSegments(vector<LSEG> &segments, vector<LSEG> &horizontal, vector<LSEG> &vertical)
{
    for(LSEG seg: segments) {
        if(seg.isHorizontal()) {
            horizontal.push_back(seg);
        } else if(seg.isVertical()) {
            vertical.push_back(seg);
        }
    }
}

void drawSegments(const cv::Mat &src, cv::Mat &dest, vector<LSEG> &segments, const cv::Scalar &color)
{
    src.copyTo(dest);
    for(LSEG seg: segments) {
        cv::line(dest, seg.start, seg.end, color, 1);
        cv::circle(dest, seg.start, 2, CV_RGB(0, 255, 0));
        cv::circle(dest, seg.end, 2, CV_RGB(0, 255, 255));
    }
}

void joinSegments(vector<LSEG> &segments, const cv::Mat &img, bool horizontal)
{
    bool foundMatching;
    double maxDistance = sqrt(img.rows * img.rows + img.cols * img.cols) * 0.10;
    int step = 0;
    cv::Mat result;
    drawSegments(img, result, segments, CV_RGB(0, 0, 255));
    cv::imshow(to_string(step), result);

    do {
        foundMatching = false;
        sort(segments.begin(), segments.end());

        for(vector<LSEG>::iterator left = segments.begin(); left < segments.end(); left++) {
            double leftGradient = left->gradient();

            for(vector<LSEG>::iterator right = left + 1; right < segments.end(); right++) {
                if(abs(right->gradient() - leftGradient) > MAX_GRADIENT_DIFFERENCE) break;

                SegmentDistance dist = left->distanceTo(*right);

                if(dist.length < maxDistance) {
                    if(horizontal) {
                        if(dist.dy <= 8) {
                            left->joinWith(*right);
                            leftGradient = left->gradient();
                            segments.erase(right);
                            foundMatching = true;
                        }
                    } else {
                        if(dist.dx <= 8) {
                            left->joinWith(*right);
                            leftGradient = left->gradient();
                            segments.erase(right);
                            foundMatching = true;
                        }
                    }
                }
            }
        }

        drawSegments(img, result, segments, CV_RGB(0, 0, 255));
        cv::imshow(to_string(++step), result);
    } while(foundMatching);
}

void applyLswms(LSWMS &lswms, cv::Mat img, vector<LSEG> &segments)
{
    vector<LSEG> tmpSegments;
    lswms.run(img, tmpSegments);

    segments.reserve(segments.size() + tmpSegments.size());
    segments.insert(segments.end(), tmpSegments.begin(), tmpSegments.end());
}

void findSegments(string file)
{
    cv::Mat src = cv::imread(file), gray, result;
    if(src.rows > 800) {
        cv::resize(src, src, cv::Size(src.cols * 800 / src.rows, 800));
    }

    vector<cv::Mat> channels;    
    src.copyTo(result);
    cv::Size size(src.cols, src.rows);
    vector<LSEG> segments;

    cv::split(src, channels);

    LSWMS lswms(size, 3, 10000);
    gettimeofday(&ts,0);
    t1 = (ts.tv_sec * 1000 + (ts.tv_usec / 1000));

    cv::cvtColor(src, gray, CV_RGB2GRAY);
    cv::equalizeHist(gray, gray);
    enhanceEdges(gray, 5, 10);
    applyLswms(lswms, gray, segments);

    for(int i = 0; i < channels.size(); i++) {
        applyLswms(lswms, channels[i], segments);
    }

    gettimeofday(&ts,0);
    t2 = (ts.tv_sec * 1000 + (ts.tv_usec / 1000));
    t = (double)t2-(double)t1;
    std::cout << file << " LSWMS runtime: " << t << "ms" << std::endl;

    cout << "found " << segments.size() << endl;
    vector<LSEG> horizontal, vertical;
    categorizeSegments(segments, horizontal, vertical);
    //joinSegments(horizontal, src, true);
    joinSegments(vertical, src, false);

    gettimeofday(&ts,0);
    t2 = (ts.tv_sec * 1000 + (ts.tv_usec / 1000));
    t = (double)t2-(double)t1;
    std::cout << file << " Total runtime: " << t << "ms" << std::endl;

    //drawSegments(result, horizontal, CV_RGB(255, 0, 0));
    drawSegments(result, result, vertical, CV_RGB(0, 0, 255));

    const char* path = file.c_str();
    string filename(basename(const_cast<char*>(path)));
    cv::imwrite("Data/" + filename, result);
    cv::imshow(filename, result);
}

/** Main function*/
int main(int argc, char** argv)
{
    LineSegment l1(LinePoint(0, 0), LinePoint(0, 7));
    LineSegment l2(LinePoint(5, 5), LinePoint(5, 10));
    cout << l1.distanceTo(l2).length << endl;
    //findSegments("../../Data/Images/Doors/Black_door.jpg");
    //findSegments("../../Data/Images/Doors/back door 5.jpg");
    //findSegments("../../Data/Images/Doors/Front-Door.jpg");
    //findSegments("../../Data/Images/Doors/4.jpg");
    findSegments("../../Data/Images/Doors/3.jpg");
    //findSegments("../../Data/Images/Doors/Standard-Office-Door-Size.jpg");
    //findSegments("../../Data/Images/Doors/c1000xContemporary timber front door Kloeber Funkyfront.jpg");
    //findSegments("../../Data/Images/Doors/door-designs-1.jpg");
    //findSegments("../../Data/Images/Doors/entry-door-x.jpg");
    //findSegments("../../Data/Images/Doors/front-door1.jpg");
    //findSegments("../../Data/Images/Doors/front-entry-doors-960x1280-feng-shui-front-door-interior-ae-i.com.jpg");
    //findSegments("../../Data/Images/Doors/Light_Minimal_a_jpg_1024x768_max_q85.jpg");
    //findSegments("../../Data/Images/Doors/open_door1.jpg");

    cv::waitKey();
}
