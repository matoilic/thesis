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
    detector.findDoorCorners(gray, corners);
    detector.drawSegments(img, img, detector.horizontalSegments, CV_RGB(255, 0, 0));
    detector.drawSegments(img, img, detector.verticalSegments, CV_RGB(0, 255, 0));

    for(cv::Point &p: corners) {
        cv::circle(img, p, 5, CV_RGB(0, 255, 0), -5);
    }

    cv::imshow(input, img);
}

int main(int argc, char** argv)
{
    findCorners("../../Data/Images/Doors/Black_door.jpg");
    findCorners("../../Data/Images/Doors/back door 5.jpg");
    findCorners("../../Data/Images/Doors/Front-Door.jpg");
    findCorners("../../Data/Images/Doors/4.jpg");
    findCorners("../../Data/Images/Doors/3.jpg");
    findCorners("../../Data/Images/Doors/Standard-Office-Door-Size.jpg");
    findCorners("../../Data/Images/Doors/c1000xContemporary timber front door Kloeber Funkyfront.jpg");
    findCorners("../../Data/Images/Doors/door-designs-1.jpg");
    findCorners("../../Data/Images/Doors/entry-door-x.jpg");
    findCorners("../../Data/Images/Doors/front-door1.jpg");
    findCorners("../../Data/Images/Doors/front-entry-doors-960x1280-feng-shui-front-door-interior-ae-i.com.jpg");
    findCorners("../../Data/Images/Doors/Light_Minimal_a_jpg_1024x768_max_q85.jpg");
    findCorners("../../Data/Images/Doors/open_door1.jpg");

    cv::waitKey();
}
