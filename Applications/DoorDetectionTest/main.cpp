#include <stdio.h>
#if (defined WIN32 || defined _WIN32)
    #include <direct.h>
#else
    #include <unistd.h>
#endif
#include <iostream>
#include <sstream>
#include <time.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <LSDLineDetector.hpp>
#include <PointQuadTree.hpp>
#include <algorithm>
#include <cmath>

using namespace std;

#define MODE 1


#define SEGMENT_MIN_LENGTH 50
#define SEGMENTS_MAX_DISTANCE 30
#define SEGMENTS_MAX_ANGLE 0.5
#define SCALE_HEIGHT 800
#define SEGMENT_MERGE_RADIUS 5

cv::RNG rng;
LSDLineDetector detector;

double cross(double a[2], double b[2])
{
    return a[0] * b[1] - a[1] * b[0];
}

void buildMatchingSegments(PointQuadTree<LineSegment*> &quadTree, LineSegment* segment, std::vector<std::vector<LineSegment*>*> &matchingSegments)
{
    int a = SEGMENT_MERGE_RADIUS * 2 + 1;
    Rectangle area(segment->start.x - SEGMENT_MERGE_RADIUS, segment->start.y - SEGMENT_MERGE_RADIUS, a, a);
    Rectangle area2(segment->end.x - SEGMENT_MERGE_RADIUS, segment->end.y - SEGMENT_MERGE_RADIUS, a, a);

    std::vector<LineSegment*> segments = quadTree.query(area);
    std::vector<LineSegment*> segments2 = quadTree.query(area2);
    segments.insert(segments.end(), segments2.begin(), segments2.end());

    bool hasMatch = false;
    for (auto it = segments.begin(); it != segments.end(); it++) {
        LineSegment* s = *it;

        if (!s->equals(*segment)) {

            double angle = abs(s->getAngle(*segment));
            if (angle >= 0.8) {
                hasMatch = true;

                if (segment->concatenation == nullptr) {
                    if (s->concatenation == nullptr) {
                        s->concatenation = new std::vector<LineSegment*>();
                        s->concatenation->push_back(s);
                        matchingSegments.push_back(s->concatenation);
                    }
                    segment->concatenation = s->concatenation;
                } else {
                    if (segment->concatenation == s->concatenation) {
                        continue;
                    }
                }

                segment->concatenation->push_back(segment);
            }
        }
    }

    if (!hasMatch && segment->concatenation == nullptr) {
        segment->concatenation = new std::vector<LineSegment*>();
        segment->concatenation->push_back(segment);
        matchingSegments.push_back(segment->concatenation);
    }
}


int main()
{
    clock_t start, end;

    char cwd[FILENAME_MAX];
    getcwd(cwd, sizeof(cwd));

    stringstream path;
    path << cwd << "/../../Data/Images/Doors/4.jpg";

    cv::Mat sourceImage, scaledImage, grey, canny, hist;
    sourceImage = cv::imread(path.str(), CV_LOAD_IMAGE_COLOR);

    double scale = SCALE_HEIGHT / (double) sourceImage.rows;
    cv::resize(sourceImage, scaledImage, cv::Size(), scale, scale);

    cv::cvtColor(scaledImage, grey, CV_RGB2GRAY);

    // destination image
    cv::Mat drawing = cv::Mat::zeros( grey.size(), CV_8UC3 );
    scaledImage.copyTo(drawing);
    cv::Mat lines = cv::Mat::zeros( grey.size(), CV_8UC3 );
    //scaledImage.copyTo(lines);

    start = clock();
    LineSegmentList segments = detector.detect(scaledImage);
    end = clock();
    std::cout << "Execution Time LSD: " << (end - start) << std::endl;
    std::cout << "Number of Segments: " << segments.size() << std::endl;


#if MODE == 0
    start = clock();

    // Merge lines
    PointQuadTree<LineSegment*> quadTree(scaledImage.cols, scaledImage.rows);
    for (auto it = segments.begin(); it != segments.end(); it++) {
        LineSegment* segment = &(*it);
        quadTree.insert(segment->start, segment);
        quadTree.insert(segment->end, segment);
    }

    std::vector<std::vector<LineSegment*>*> matchingSegments;
    for (auto it = segments.begin(); it != segments.end(); it++) {
        LineSegment* segment = &(*it);
        buildMatchingSegments(quadTree, segment, matchingSegments);
    }

    end = clock();
    std::cout << "Execution Time Merge: " << (end - start) << std::endl;


    LineSegmentList combinedSegments;
    for (auto it = matchingSegments.begin(); it != matchingSegments.end(); it++) {

        int minX = scaledImage.cols;
        int minY = scaledImage.rows;
        int maxX = 0;
        int maxY = 0;

        for (auto it2 = (*it)->begin(); it2 != (*it)->end(); it2++) {
            LineSegment* s = *it2;
            minX = std::min(minX, std::min(s->start.x, s->end.x));
            minY = std::min(minY, std::min(s->start.y, s->end.y));
            maxX = std::max(maxX, std::max(s->start.x, s->end.x));
            maxY = std::max(maxY, std::max(s->start.y, s->end.y));
        }

        LineSegment segment(Point(minX, minY), Point(maxX, maxY));
        combinedSegments.push_back(segment);

        cv::line(lines, cv::Point(segment.start.x, segment.start.y), cv::Point(segment.end.x, segment.end.y), cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255)), 1);
    }
#endif

#if MODE == 1

    // loop over all line segments and filter lines without a minimal length
    start = clock();
    for (auto it = segments.begin(); it != segments.end(); it++) {
        LineSegment segment = *it;

        if (segment.getLength() < SEGMENT_MIN_LENGTH) {
            it = segments.erase(it);
        }
    }
    end = clock();
    std::cout << "Execution Time Filter Segments (Length): " << (end - start) << std::endl;

    int numIntersections = 0;

    // intersect line segments
    start = clock();
    int offset = 0;
    for (auto it = segments.begin(); it != segments.end(); it++) {
        offset++;

        for (auto it2 = segments.begin() + offset; it2 != segments.end(); it2++) {
            LineSegment s1 = *it;
            LineSegment s2 = *it2;

            double d1 = s1.start.calculateDistanceTo(s2.start);
            double d2 = s1.start.calculateDistanceTo(s2.end);
            double d3 = s1.end.calculateDistanceTo(s2.start);
            double d4 = s1.end.calculateDistanceTo(s2.end);
            double dist = std::min(std::min(d1, d2), std::min(d3, d4));

            if (dist < SEGMENTS_MAX_DISTANCE) {

                double q0[] = { s1.start.x, s1.start.y };
                double q1[] = { s1.end.x, s1.end.y };
                double p0[] = { s2.start.x, s2.start.y };
                double p1[] = { s2.end.x, s2.end.y };

                double s[] = { q1[0] - q0[0], q1[1] - q0[1] };
                double r[] = { p1[0] - p0[0], p1[1] - p0[1] };


                double cosA = s1.getAngle(s2);
                if (abs(cosA) < SEGMENTS_MAX_ANGLE) {
                    double divisor = cross(r, s);
                    if (divisor != 0) {
                        double qp[] = { q0[0] - p0[0], q0[1] - p0[1] };
                        double u = cross(qp, r) / cross(r, s);

                        double x = q0[0] + u * s[0];
                        double y = q0[1] + u * s[1];

                        cv::line(drawing, cv::Point(s1.start.x, s1.start.y), cv::Point(s1.end.x, s1.end.y), cv::Scalar(255, 255, 255));
                        cv::line(drawing, cv::Point(s2.start.x, s2.start.y), cv::Point(s2.end.x, s2.end.y), cv::Scalar(255, 255, 255));
                        cv::circle(drawing, cv::Point(x, y), 3, cv::Scalar(0, 255, 0), 2);
                    }
                }
            }
        }
    }
    end = clock();
    std::cout << "Execution Time Intersect Lines: " << (end - start) << std::endl;

    std::cout << "Number of Intersections: " << numIntersections << std::endl;

    std::string fullPath = path.str();
    auto index = fullPath.find_last_of("/");
    std::string imageName = fullPath.substr(index, fullPath.size() - index);

    cv::imwrite("D:\\BFH\\Semester8\\thesis\\Data\\Images\\LSD" + imageName, lines);

#endif


    cv::namedWindow("Display window", CV_WINDOW_AUTOSIZE);
    cv::imshow("Lines", lines);
    cv::imshow("Display window", drawing);
    cv::waitKey(0);

    return 0;
}

void printMat(const cv::Mat_<float> &mat)
{
    for (int r = 0; r < mat.rows; r++)
    {
        for (int c = 0; c < mat.cols; c++)
        {
            cout << mat.row(r).col(c) << "\t";
        }

        cout << endl;
    }
}
