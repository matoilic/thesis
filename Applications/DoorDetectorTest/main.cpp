#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <Ardoor/DoorDetection/DoorDetector.hpp>

using namespace std;

void findCorners(const string &input)
{
    cv::Mat img = cv::imread(input), gray;
    if(img.rows > 800) {
        cv::resize(img, img, cv::Size(img.cols * 800 / img.rows, 800));
    }
    cv::cvtColor(img, gray, CV_RGB2GRAY);

    vector<cv::Point> corners;
    DoorDetector detector(cv::Size(img.cols, img.rows));
    if(detector.findDoorCorners(gray, corners)) {
        //detector.drawSegments(img, img, detector.horizontalSegments, CV_RGB(255, 0, 0));
        //detector.drawSegments(img, img, detector.verticalSegments, CV_RGB(0, 255, 0));

        for(_DoorCandidate &c: detector.candidates) {
            cv::line(img, c.top.start, c.top.end, CV_RGB(255, 0, 255), 2);
            cv::line(img, c.right.start, c.right.end, CV_RGB(255, 0, 255), 2);
            cv::line(img, c.bottom.start, c.bottom.end, CV_RGB(255, 0, 255), 2);
            cv::line(img, c.left.start, c.left.end, CV_RGB(255, 0, 255), 2);
        }

        /*for(cv::Point &p: corners) {
            cv::circle(img, p, 5, CV_RGB(0, 225, 233), -5);
        }*/

        /*cv::line(img, corners[0], corners[1], CV_RGB(0, 225, 233), 2);
        cv::line(img, corners[1], corners[2], CV_RGB(0, 225, 233), 2);
        cv::line(img, corners[2], corners[3], CV_RGB(0, 225, 233), 2);
        cv::line(img, corners[3], corners[0], CV_RGB(0, 225, 233), 2);
        cv::line(img, corners[0], corners[2], CV_RGB(0, 225, 233), 2);
        cv::line(img, corners[1], corners[3], CV_RGB(0, 225, 233), 2);*/
    }

    cv::imshow(input, img);
}

int main(int argc, char** argv)
{
//    findCorners("../../Data/Images/Doors/Misc/Black_door.jpg");
//    findCorners("../../Data/Images/Doors/Misc/back door 5.jpg");
//    findCorners("../../Data/Images/Doors/Misc/Front-Door.jpg");
//    findCorners("../../Data/Images/Doors/Misc/4.jpg");
//    findCorners("../../Data/Images/Doors/Misc/3.jpg");
//    findCorners("../../Data/Images/Doors/Misc/Standard-Office-Door-Size.jpg");
//    findCorners("../../Data/Images/Doors/Misc/c1000xContemporary timber front door Kloeber Funkyfront.jpg");
//    findCorners("../../Data/Images/Doors/Misc/door-designs-1.jpg");
//    findCorners("../../Data/Images/Doors/Misc/entry-door-x.jpg");
//    findCorners("../../Data/Images/Doors/Misc/front-door1.jpg");
//    findCorners("../../Data/Images/Doors/Misc/front-entry-doors-960x1280-feng-shui-front-door-interior-ae-i.com.jpg");
//    findCorners("../../Data/Images/Doors/Misc/Light_Minimal_a_jpg_1024x768_max_q85.jpg");
//    findCorners("../../Data/Images/Doors/Misc/open_door1.jpg");
//    findCorners("../../Data/Images/Doors/Misc/Christmas front-door-mollica.jpg");
//    findCorners("../../Data/Images/Doors/Misc/front-door-come-on-over-front-door-progress-ae-i.com.jpg");
    findCorners("/Users/matoilic/Workspace/School/Thesis/Documentation/images/door-side.jpg");
//    findCorners("../../Data/Images/Doors/Misc/7.jpg");

    while(cv::waitKey() != 'q');
}
