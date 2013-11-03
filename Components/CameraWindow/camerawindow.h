#ifndef CAMERAWINDOW_H
#define CAMERAWINDOW_H

#include <vector>
#include <thread>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <GLFW/glfw3.h>

using namespace std;

typedef function<void(cv::Mat&)> FrameProcessor;

class CameraWindow
{
    cv::VideoCapture capture;
    std::thread captureThread;
    cv::Mat currentFrame;
    int desiredFrameRate;
    double frameDrawTime;
    double frameEndTime;
    double frameStartTime;
    bool isStopped;
    bool needRedisplay;
    std::vector<FrameProcessor> processors;
    std::mutex frameLock;
    GLFWwindow* window;

    void captureLoop();
    void draw(const cv::Mat &frame);
    void ensureFramerate();
    void initGL();
    void initShader();
    void initWindow(int width, int height);
    GLuint matToTexture(const cv::Mat &mat);
public:
    CameraWindow(int frameRate);
    void startCapturing(const string &inputFile = "");
    void stopCapturing();
};

#endif // CAMERAWINDOW_H
