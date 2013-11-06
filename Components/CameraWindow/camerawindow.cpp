#include <iostream>
#include <stdio.h>
#include <GL/glew.h>
#if defined(__APPLE__)
    #include <OpenGL/gl.h>
#else
    #include <windows.h>
    #include <GL/GL.h>
#endif
#include <opencv2/highgui/highgui.hpp>
#include "camerawindow.h"

using namespace std;

void onGlfwError(int error, const char* description)
{
    fputs(description, stderr);
}

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
        if(needRedisplay)  {
            this_thread::yield();
            continue;
        }

        frameLock.lock();

        success = capture.read(currentFrame);
        if (success) {
            needRedisplay = true;
            frameLock.unlock();
        } else {
            frameLock.unlock();
            break;
        }
        cout << "captured" << endl;
        this_thread::yield();
    }
}

void CameraWindow::draw(const cv::Mat &frame)
{
    int width = frame.cols, height = frame.rows;
    GLuint texture = matToTexture(frame);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, texture);
    const GLfloat bgTextureVertices[] = { 0, 0, width, 0, 0, height, width, height };
    const GLfloat bgTextureCoords[]   = { 0, 1, 1, 1, 0, 0, 1, 0 };

    //begin othogonal projection
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0, width, 0.0, height, 0.01, 1000.0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -1000.0);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Update attribute values.
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(2, GL_FLOAT, 0, bgTextureVertices);
    glTexCoordPointer(2, GL_FLOAT, 0, bgTextureCoords);

    glColor4f(1,1,1,1);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_TEXTURE_2D);

    //end orthogonal projection
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

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
    } else {
        this_thread::yield();
    }
}

void CameraWindow::initGL()
{
    if (!glfwInit()) {
        cerr << "Failed to initialize GLFW" << endl;
        exit(EXIT_FAILURE);
    }

    glfwSetErrorCallback(onGlfwError);

    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RED_BITS, 8);
    glfwWindowHint(GLFW_GREEN_BITS, 8);
    glfwWindowHint(GLFW_BLUE_BITS, 8);
    glfwWindowHint(GLFW_ALPHA_BITS, 8);
    glfwWindowHint(GLFW_DEPTH_BITS, 32);
    glfwWindowHint(GLFW_SAMPLES, 4);
    #ifdef DEBUG
       glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    #endif

    window = glfwCreateWindow(100, 100, "", NULL, NULL);
    if (!window) {
        cerr << "Failed to open GLFW window" << endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glewExperimental = GL_TRUE;
    GLint glew = glewInit();
    if(glew != GLEW_OK) {
        cerr << "Failed to initialize GLEW" << endl;
        cerr << glewGetErrorString(glew) << endl;
        exit(EXIT_FAILURE);
    }

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

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);

    GLfloat ambient[]  = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat diffuse[]  = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
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
    int width = mat.cols, height = mat.rows;

    glGenTextures(1, &texture);

    glPixelStorei(GL_PACK_ALIGNMENT, 3);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 3);
    glBindTexture(GL_TEXTURE_2D, texture);

    if (mat.channels() == 3)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, mat.data);
    else if(mat.channels() == 4)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, mat.data);
    else if (mat.channels() == 1)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, mat.data);

    return texture;
}

void CameraWindow::startCapturing(const string &inputFile)
{
    if (!isStopped)
        return;

    isStopped = false;

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
    needRedisplay = true;
    initGL();
    initWindow(currentFrame.cols, currentFrame.rows);
    initShader();

    auto threadFunction = std::bind(&CameraWindow::captureLoop, this);
    //captureThread = std::thread(threadFunction);

    while (!glfwWindowShouldClose(window) && !isStopped) {        
        frameStartTime = glfwGetTime();
        glfwPollEvents();

        if(needRedisplay) {
            frameLock.lock();
            draw(currentFrame);
            glfwSwapBuffers(window);
            needRedisplay = false;
            frameLock.unlock();
        } else {
            glfwWaitEvents();
        }

        ensureFramerate();
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
