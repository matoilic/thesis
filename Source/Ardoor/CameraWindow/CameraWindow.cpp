#include <iostream>
#include <stdio.h>
#include <GL/glew.h>
#if defined(__APPLE__)
    #include <OpenGL/gl.h>
#else
    #include <windows.h>
    #include <GL/GL.h>
#endif
#if (defined WIN32 || defined _WIN32)
    #include <direct.h>
#else
    #include <unistd.h>
#endif

#include <opencv2/highgui/highgui.hpp>
#include <Ardoor/CameraWindow/CameraWindow.hpp>
#include "glUtils.hpp"
#include "matgen.h"

using namespace std;

void onGlfwError(int error, const char* description)
{
    cerr << description << endl;
}

CameraWindow::CameraWindow(int frameRate)
{
    desiredFrameRate = frameRate;
    needRedisplay = false;
    isStopped = true;
}

void CameraWindow::buildPlane()
{
   backgroundNumIndices = 6;

   float vertices[] = {-1, -1,
                        1, -1,
                        1,  1,
                       -1,  1};
   backgroundVboVertices = glUtils::buildVBO(vertices, 4, 2, sizeof(GLfloat), GL_ARRAY_BUFFER, GL_STATIC_DRAW);

   GLuint indices[] = {0, 1, 2, 0, 2, 3};
   backgroundVboIndices = glUtils::buildVBO(indices, backgroundNumIndices, 1, sizeof(GLuint), GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);

   GLfloat texture[] = {0, 1,
                        1, 1,
                        1, 0,
                        0, 0};
   backgroundVboTexture = glUtils::buildVBO(texture, 4, 2, sizeof(GLfloat), GL_ARRAY_BUFFER, GL_STATIC_DRAW);

   GET_GL_ERROR;
}

void CameraWindow::captureLoop()
{
    while(!isStopped) {
        if(needRedisplay)  {
            this_thread::yield();
            continue;
        }

        frameLock.lock();

        if (capture->read(currentFrame)) {
            needRedisplay = true;
        } else {
            cerr << "failed to grab frame" << endl;
        }

        frameLock.unlock();

        this_thread::yield();
    }
}

void CameraWindow::draw(const cv::Mat &frame)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Begin othogonal projection
    cv::Matx44f orthoProj = MatGen::ortho(-1, 1, -1, 1, -1, 1000);
    cv::Matx44f translation = MatGen::translate(0, 0, -1000);
    cv::Matx44f mvp = orthoProj * translation;

    // Pass the matrix uniform variables
    cv::Matx44f mvpT = mvp.t();
    glUniformMatrix4fv(u_mvpMatrix, 1, GL_FALSE, (GLfloat*) mvpT.val);
    GET_GL_ERROR;

    // Pass the active texture unit
    glActiveTexture(GL_TEXTURE0);
    matToTexture(frame);
    glUniform1i(u_texture0, 0);
    GET_GL_ERROR;

    glEnableVertexAttribArray(a_position);
    glEnableVertexAttribArray(a_texCoord);

    glBindBuffer(GL_ARRAY_BUFFER, backgroundVboVertices);
    glVertexAttribPointer(a_position, 2, GL_FLOAT, GL_FALSE, 0, 0);
    GET_GL_ERROR;

    glBindBuffer(GL_ARRAY_BUFFER, backgroundVboTexture);
    glVertexAttribPointer(a_texCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);
    GET_GL_ERROR;

    // Draw plane with triangles by indexes
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, backgroundVboIndices);
    glDrawElements(GL_TRIANGLES, backgroundNumIndices, GL_UNSIGNED_INT, 0);
    GET_GL_ERROR;

    // Deactivate buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Disable the vertex arrays
    glDisableVertexAttribArray(a_position);
    glDisableVertexAttribArray(a_texCoord);

    GET_GL_ERROR;
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
    char cwd[FILENAME_MAX];
    getcwd(cwd, sizeof(cwd));

    stringstream shaderPath;
    shaderPath << cwd << "/../../Data/Shader/TextureOnly.";

    string vertextShaderPath = shaderPath.str() + string("vert");
    string fragmentShaderPath = shaderPath.str() + string("frag");

    if(!glfwInit()) {
        cerr << "Failed to initialize GLFW" << endl;
        exit(EXIT_FAILURE);
    }

    glfwSetErrorCallback(onGlfwError);

    int viewportWidth = capture->get(CV_CAP_PROP_FRAME_WIDTH);
    int viewportHeight = capture->get(CV_CAP_PROP_FRAME_HEIGHT);

    window = glfwCreateWindow(
                viewportWidth,
                viewportHeight,
                "Camera Window",
                NULL,
                NULL
    );

    if(!window) {
        cerr << "Failed to create window" << endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glViewport(0, 0, viewportWidth, viewportHeight);

    GLenum result = glewInit();
    if(result != GLEW_OK) {
        cerr << "Failed to initialize GLEW" << endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSwapInterval(1);

    shaderVertexId = glUtils::buildShader(vertextShaderPath, GL_VERTEX_SHADER);
    shaderFragmentId = glUtils::buildShader(fragmentShaderPath, GL_FRAGMENT_SHADER);
    shaderProgramId = glUtils::buildProgram(shaderVertexId, shaderFragmentId);

    glUseProgram(shaderProgramId);

    a_position = glGetAttribLocation (shaderProgramId, "a_position");
    a_texCoord = glGetAttribLocation (shaderProgramId, "a_texCoord");
    u_mvpMatrix = glGetUniformLocation(shaderProgramId, "u_mvpMatrix");
    u_texture0 = glGetUniformLocation(shaderProgramId, "u_texture0");

    glGenTextures(1, &backroundTextureId);

    glClearColor(0.0f, 0.0f, 0.0f, 1); // Set the background color
    glEnable(GL_DEPTH_TEST); // Enables depth test
    glEnable(GL_CULL_FACE); // Enables the culling of back faces

    GET_GL_ERROR;
}

void CameraWindow::matToTexture(const cv::Mat &mat)
{
    int width = mat.cols, height = mat.rows;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, backroundTextureId);

    // Upload new texture data:
    if (mat.channels() == 3) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, mat.data);
    } else if(mat.channels() == 4) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, mat.data);
    } else if (mat.channels() == 1) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, mat.data);
    }
}

void CameraWindow::startCapturing(const string &inputFile)
{
    isStopped = false;

    capture = new cv::VideoCapture();
    bool success = (inputFile.size() > 0) ? capture->open(inputFile) : capture->open(CV_CAP_ANY);
    if(!success) {
        delete capture;
        cerr << "failed to capture from video source" << endl;
    }

    initGL();
    buildPlane();

    auto threadFunction = std::bind(&CameraWindow::captureLoop, this);
    captureThread = std::thread(threadFunction);

    while(!glfwWindowShouldClose(window)) {
        if(needRedisplay) {
            glfwPollEvents();
            frameLock.lock();
            draw(currentFrame);
            needRedisplay = false;
            frameLock.unlock();
            glfwSwapBuffers(window);
        }

        this_thread::yield();
    }

    stopCapturing();

    exit(EXIT_SUCCESS);
}

void CameraWindow::stopCapturing()
{
    isStopped = true;

    // Delete shaders & programs on GPU
   glDeleteShader(shaderVertexId);
   glDeleteShader(shaderFragmentId);
   glDeleteProgram(shaderProgramId);

   // Delete arrays & buffers on GPU
   glDeleteBuffers(1, &backgroundVboVertices);
   glDeleteBuffers(1, &backgroundVboIndices);

   glfwDestroyWindow(window);
   glfwTerminate();

   capture->release();
   delete capture;
}
