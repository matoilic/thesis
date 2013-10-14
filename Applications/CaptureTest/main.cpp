#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "capture.h"

using namespace std;

#define WINDOW_NAME "Capture"

void processFrame(cv::Mat& frame)
{
    cout << "processFrame()" << flush << endl;
    cv::imshow(WINDOW_NAME, frame);
}

int main()
{
    cv::namedWindow(WINDOW_NAME, CV_WINDOW_AUTOSIZE);

    Capture capture;
    capture.setHandler(processFrame);
    capture.start();

    while (true) {
        char key = cv::waitKey(30);

        if (key == 27) {
            capture.stop();
            break;
        }
    }

    return 0;
}
