#include <iostream>
#include <opencv2/core/core.hpp>
#include <GLFW/glfw3.h>
#if defined(__APPLE__)
    #include <OpenGl/gl.h>
    #include <OpenGL/glext.h>
#else
    #include <GL/gl.h>
    #include <GL/glext.h>
#endif
#include <thread>
#include <mutex>
#include <Ardoor/Capture/Capture.hpp>

using namespace std;

#define WINDOW_NAME "Capture"
GLFWwindow* window;
GLuint textureId;
cv::Mat currentFrame;
bool needRedisplay = false;
std::mutex redisplayLock;

void initializeGL()
{
    // Default GL configuration
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Init Lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Create light components
    static GLfloat ambientLight[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
    static GLfloat diffuseLight[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
    static GLfloat position[] = { 0.0f, 0.0f, 2.0f, 1.0f };

    // Assign created components to GL_LIGHT0
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glPixelStorei(GL_PACK_ALIGNMENT, 3);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 3);
    glBindTexture(GL_TEXTURE_2D, textureId);
}

void processFrame(cv::Mat& frame)
{
    if (frame.empty() || glfwWindowShouldClose(window))
        return;

    redisplayLock.lock();
    frame.copyTo(currentFrame);
    needRedisplay = true;
    redisplayLock.unlock();
}

void display()
{
    int w = currentFrame.cols;
    int h = currentFrame.rows;

    // Upload new texture data:
    if (currentFrame.channels() == 3)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, currentFrame.data);
    else if(currentFrame.channels() == 4)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, currentFrame.data);
    else if (currentFrame.channels() == 1)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, currentFrame.data);

    const GLfloat bgTextureVertices[] = { 0, 0, w, 0, 0, h, w, h };
    const GLfloat bgTextureCoords[]   = { 0, 1, 1, 1, 0, 0, 1, 0 };

    //begin othogonal projection
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0, w, 0.0, h, 0.01, 1000.0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -1000.0);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureId);

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
}

void reshape()
{
    glViewport(0, 0, currentFrame.cols, currentFrame.rows);
    glfwSetWindowSize(window, currentFrame.cols, currentFrame.rows);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main()
{
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }

    window = glfwCreateWindow(500, 500, "Screen", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to open GLFW window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSetWindowTitle(window, "Capture Test");
    initializeGL();

    Capture capture;
    capture.setHandler(processFrame);
    capture.start();

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        if(needRedisplay) {
            redisplayLock.lock();

            reshape();
            display();
            glfwSwapBuffers(window);
            glfwPollEvents();

            needRedisplay = false;
            redisplayLock.unlock();
        }
    }

    capture.stop();
    // Terminate GLFW
    glfwTerminate();

    exit(EXIT_SUCCESS);
}
