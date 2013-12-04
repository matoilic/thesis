#include <iostream>
#include <cmath>
#include <opencv2/core/core.hpp>
#include <opencv2/legacy/legacy.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;

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

void findSegments(const string file)
{
    cv::Mat src = cv::imread(file), hough;
    if(src.rows > 800) {
        cv::resize(src, src, cv::Size(src.cols * 800 / src.rows, 800));
    }

    vector<cv::Vec4i> lines;
    int minLength = sqrt(src.cols * src.cols + src.rows * src.rows) * 0.05;
    cv::cvtColor(src, hough, CV_RGB2GRAY);
    enhanceEdges(hough, 5, 3);

    cv::Canny(hough, hough, 80, 200, 3);
    cv::HoughLinesP(hough, lines, 1, CV_PI / 180, 100, minLength, minLength);

    for(size_t i = 0; i < lines.size(); i++) {
        cv::line(src, cv::Point(lines[i][0], lines[i][1]), cv::Point(lines[i][2], lines[i][3]), CV_RGB(255, 0, 0));
    }

    cv::imshow(file, src);
}

int main()
{
    findSegments("../../Data/Images/Doors/Black_door.jpg");
    findSegments("../../Data/Images/Doors/back door 5.jpg");
    findSegments("../../Data/Images/Doors/Front-Door.jpg");
    findSegments("../../Data/Images/Doors/4.jpg");
    findSegments("../../Data/Images/Doors/3.jpg");
    findSegments("../../Data/Images/Doors/Standard-Office-Door-Size.jpg");
    findSegments("../../Data/Images/Doors/c1000xContemporary timber front door Kloeber Funkyfront.jpg");
    findSegments("../../Data/Images/Doors/door-designs-1.jpg");
    findSegments("../../Data/Images/Doors/entry-door-x.jpg");
    findSegments("../../Data/Images/Doors/front-door1.jpg");
    findSegments("../../Data/Images/Doors/front-entry-doors-960x1280-feng-shui-front-door-interior-ae-i.com.jpg");
    findSegments("../../Data/Images/Doors/Light_Minimal_a_jpg_1024x768_max_q85.jpg");
    findSegments("../../Data/Images/Doors/open_door1.jpg");

    cv::waitKey();

    return 0;
}

