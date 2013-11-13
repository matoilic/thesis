#ifndef CAMERAWINDOW_H
#define CAMERAWINDOW_H

#include <vector>
#include <thread>
#include <mutex>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <GLFW/glfw3.h>
#include "ardoor.h"
#define GET_GL_ERROR CameraWindow::getGlError(__FILE__, __LINE__, false)

using namespace std;

typedef function<void(cv::Mat&)> FrameProcessor;

class ARD_EXPORT CameraWindow
{
    cv::VideoCapture* capture;
    thread captureThread;
    cv::Mat currentFrame;
    int desiredFrameRate;
    double frameDrawTime;
    double frameEndTime;
    double frameStartTime;
    bool isStopped;
    bool needRedisplay;
    vector<FrameProcessor> processors;
    mutex frameLock;
    GLFWwindow* window;
    GLuint shaderVertexId;
    GLuint shaderFragmentId;
    GLuint shaderProgramId;

    GLuint backgroundVboVertices;
    GLuint backgroundVboIndices;
    GLuint backgroundVboTexture;
    GLuint backgroundNumIndices;
    GLuint backroundTextureId;

    //variable locations and identifiers within the current program
    GLint a_position;
    GLint a_normal;
    GLint a_texCoord;
    GLint u_mvMatrix;
    GLint u_mvpMatrix;
    GLint u_nMatrix;
    GLint u_texture0;

    void buildPlane();
    void captureLoop();
    void draw(const cv::Mat &frame);
    void ensureFramerate();
    void initGL();
    void initWindow(int width, int height);
    void matToTexture(const cv::Mat &mat);
public:
    CameraWindow(int frameRate);
    static void getGlError(char* file, int line, bool quit);
    void startCapturing(const string &inputFile = "");
    void stopCapturing();
};

#endif // CAMERAWINDOW_H
