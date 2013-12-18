#include <iostream>
#include "glUtils.hpp"
#include "rendering.hpp"
#include "ar.hpp"

#define _USE_MATH_DEFINES
#include <math.h>

bool windowInitialized = false;
GLFWwindow* window;


void initializeRendering()
{
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_DEPTH_BITS, 16);

    window = glfwCreateWindow(500, 500, "Screen", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to open GLFW window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Set callback functions
    glfwSetFramebufferSizeCallback(window, reshape);

    // Set key callback function
    glfwSetKeyCallback(window, &keyCallback);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);


    glewExperimental = GL_TRUE;
    GLint glew = glewInit();
    if(glew != GLEW_OK) {
        cerr << "Failed to initialize GLEW" << endl;
        cerr << glewGetErrorString(glew) << endl;
        exit(EXIT_FAILURE);
    }


    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    reshape(window, width, height);

    // Initialize OpenGL
    initializeGL();

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        display();

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Terminate GLFW
    glfwTerminate();

    exit(EXIT_SUCCESS);
}


void resizeWindow(int width, int height)
{
    if (!windowInitialized) {
        glfwSetWindowSize(window, width, height);
        windowInitialized = true;
    }
}

void reshape(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    initializePerspective();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawScene();
}

cv::Matx44f ortho(float left, float right, float bottom, float top, float near, float far)
{
    cv::Matx44f ortho = cv::Matx44f::eye();
    ortho(0, 0) = 2/(right-left);
    ortho(0, 3) = -(right+left)/(right-left);
    ortho(1, 1) = 2/(top-bottom);
    ortho(1, 3) = -(top+bottom)/(top-bottom);
    ortho(2, 2) = -2/(far-near);
    ortho(2, 3) = -(far+near)/(far-near);
    return ortho;
}

cv::Matx44f translate(float x, float y, float z)
{
    cv::Matx44f translate = cv::Matx44f::eye();
    translate(0, 3) = x;
    translate(1, 3) = y;
    translate(2, 3) = z;
    return translate;
}

cv::Matx44f scale(float x, float y, float z)
{
    cv::Matx44f scale = cv::Matx44f::eye();
    scale(0, 0) = x;
    scale(1, 1) = y;
    scale(2, 2) = z;
    return scale;
}


cv::Matx44f rotate(float angle, float axisx, float axisy, float axisz)
{
    cv::Matx44f rotate = cv::Matx44f::eye();

    float l = axisx*axisx + axisy*axisy + axisz*axisz;  // length squared
    float x = axisx, y = axisy, z = axisz;
    x = axisx, y = axisy, z = axisz;
    if ((l > float(1.0001) || l < float(0.9999)) && l != 0) {
        l = float(1.0) / sqrt(l);
        x *= l; y *= l; z *= l;
    }

    float xy = x*y, yz = y*z, xz = x*z, xx = x*x, yy = y*y, zz = z*z;
    float ca = cosf(angle / 180.0f * M_PI), sa = sinf(angle / 180.0f * M_PI);

    rotate(0, 0) = xx * (1-ca) + ca;
    rotate(0, 1) = xy * (1-ca) - z*sa;
    rotate(0, 2) = xz * (1-ca) + y*sa;
    rotate(1, 0) = xy * (1-ca) + z*sa;
    rotate(1, 1) = yy * (1-ca) + ca;
    rotate(1, 2) = yz * (1-ca) - x*sa;
    rotate(2, 0) = xz * (1-ca) - y*sa;
    rotate(2, 1) = yz * (1-ca) + x*sa;
    rotate(2, 2) = zz * (1-ca) + ca;

    return rotate;
}
