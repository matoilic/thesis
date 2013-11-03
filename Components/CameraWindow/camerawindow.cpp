#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include "camerawindow.h"

using namespace std;

CameraWindow::CameraWindow(int frameRate)
{
    desiredFrameRate = frameRate;
    needRedisplay = false;
    isStopped = true;
}

void CameraWindow::captureLoop()
{
    bool success;

    while(!glfwWindowShouldClose(window) && !isStopped) {
        frameLock.lock();
        success = capture.read(currentFrame);
        if (success) {
            needRedisplay = true;
            frameLock.unlock();
        } else {
            frameLock.unlock();
            break;
        }
    }
}

void CameraWindow::draw(const cv::Mat &frame)
{
    int width = frame.cols, height = frame.rows;
    GLuint texture = matToTexture(frame);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);

    glPixelStorei(GL_PACK_ALIGNMENT, 3);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 3);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);
        glTexCoord2f(1, 1);
        glVertex2f(-width / 2, height / 2);
        glTexCoord2f(0, 1);
        glVertex2f(width / 2, height / 2);
        glTexCoord2f(0, 0);
        glVertex2f(width / 2, -height / 2);
        glTexCoord2f(1, 0);
        glVertex2f(-width / 2, -height / 2);
    glEnd();

    glDeleteTextures(1, &texture);

    glDisable(GL_TEXTURE_2D);

    glFlush();
}

void CameraWindow::ensureFramerate()
{
    static double allowedFrameTime = 1.0 / desiredFrameRate;

    frameEndTime = glfwGetTime();
    frameDrawTime = frameEndTime - frameStartTime;

    long sleepTime;

    if (frameDrawTime < allowedFrameTime) {
        sleepTime = (long)((allowedFrameTime - frameDrawTime) * 1000);
        this_thread::sleep_for(chrono::milliseconds(sleepTime));
    }
}

void CameraWindow::initGL()
{
    if (!glfwInit()) {
        cerr << "Failed to initialize GLFW" << endl;
        exit(EXIT_FAILURE);
    }

    window = glfwCreateWindow(100, 100, "", NULL, NULL);
    if (!window) {
        cerr << "Failed to open GLFW window" << endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Change to the projection matrix and set our viewing volume
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glEnable(GL_CULL_FACE);
    glfwSwapInterval(1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void CameraWindow::initShader()
{
    GLuint program = 0;
    GLuint shader = 0;

    // define minimal default shaders
    const char* fragmentShader = "void main(void) { gl_FragColor = gl_Color; }";
    const char* vertexShader =  "void main(void) { gl_FrontColor = gl_Color; gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex; }";

    glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*)&program);
    glDeleteProgram(program);
    program = glCreateProgram();

    shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shader, 1, &vertexShader, NULL);
    glCompileShader(shader);
    glAttachShader(program, shader);
    glDeleteShader(shader);

    shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shader, 1, &fragmentShader, NULL);
    glCompileShader(shader);
    glAttachShader(program, shader);
    glDeleteShader(shader);

    glLinkProgram(program);

    // check for successful program linking
    GLint successful = false;
    glGetProgramiv(program, GL_LINK_STATUS, &successful);
    if (successful) {
        glUseProgram(program);
    } else {
        cout << "error linking shader program" << endl;
    }
}

void CameraWindow::initWindow(int width, int height)
{
    glfwWindowHint(GLFW_RED_BITS, 8);
    glfwWindowHint(GLFW_GREEN_BITS, 8);
    glfwWindowHint(GLFW_BLUE_BITS, 8);
    glfwWindowHint(GLFW_ALPHA_BITS, 8);
    glfwWindowHint(GLFW_DEPTH_BITS, 32);

    GLfloat fieldOfView = 45.0f;
    GLfloat zNear = 0.1f;
    GLfloat zFar = 200.0f;

    // equivilant to calling:
    // gluPerspective(fieldOfView / 2.0f, width / height , near, far);
    // avoids requiring glu.h
    GLfloat aspectRatio = (width > height) ? float(width) / float(height) : float(width) / float(height);
    GLfloat fH = tan(float(fieldOfView / 360.0f * 3.14159f)) * zNear;
    GLfloat fW = fH * aspectRatio;
    glFrustum(-fW, fW, -fH, fH, zNear, zFar);

    glfwSetWindowSize(window, width, height);
}

GLuint CameraWindow::matToTexture(const cv::Mat &mat)
{
    GLuint texture;
    glGenTextures(1, &texture);

    glTexImage2D(GL_TEXTURE_2D,     // Type of texture
                 0,                 // Pyramid level (for mip-mapping) - 0 is the top level
                 GL_RGB,            // Internal colour format to convert to
                 mat.cols,          // Image width  i.e. 640 for Kinect in standard mode
                 mat.rows,          // Image height i.e. 480 for Kinect in standard mode
                 0,                 // Border width in pixels (can either be 1 or 0)
                 GL_BGR, // Input image format (i.e. GL_RGB, GL_RGBA, GL_BGR etc.)
                 GL_UNSIGNED_BYTE,  // Image data type
                 mat.ptr());        // The actual image data itself

    return texture;
}

void CameraWindow::startCapturing(const string &inputFile)
{
    if (!isStopped)
        return;

    isStopped = false;
    initGL();
    initShader();

    bool success = (inputFile.empty()) ? capture.open(0) : capture.open(inputFile);

    if(!success) {
        if(inputFile.empty()) {
            cerr << "Failed to start capturing from camera" << endl;
        } else {
            cerr << "Failed to capture from video " << inputFile << endl;
        }

        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    //grab first frame for window initializaiton
    capture.read(currentFrame);
    initWindow(currentFrame.cols, currentFrame.rows);

    auto threadFunction = std::bind(&CameraWindow::captureLoop, this);
    captureThread = std::thread(threadFunction);

    while (!glfwWindowShouldClose(window) && !isStopped) {
        frameStartTime = glfwGetTime();

        if(needRedisplay) {
            frameLock.lock();

            draw(currentFrame);
            glfwSwapBuffers(window);
            glfwPollEvents();
            needRedisplay = false;

            frameLock.unlock();
        } else {
            glfwWaitEvents();
        }

        //ensureFramerate();
    }

    stopCapturing();
}

void CameraWindow::stopCapturing()
{
    isStopped = true;
    captureThread.join();
    capture.release();
    glfwDestroyWindow(window);
    glfwTerminate();
}
