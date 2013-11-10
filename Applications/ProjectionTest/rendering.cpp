#include <iostream>
#include "glUtils.hpp"
#include "rendering.hpp"
#include "ar.hpp"

GLdouble projection[16], inverse[16];
bool windowInitialized = false;
GLFWwindow* window;


void initializeRendering()
{
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_DEPTH_BITS, 16);

    window = glfwCreateWindow(500, 500, "Screen", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to open GLFW window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Set callback functions
    glfwSetFramebufferSizeCallback(window, reshape);

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

cv::Matx44d ortho(double left, double right, double bottom, double top, double near, double far)
{
    cv::Matx44d ortho = cv::Matx44d::eye();
    ortho(0, 0) = 2/(right-left);
    ortho(0, 3) = -(right+left)/(right-left);
    ortho(1, 1) = 2/(top-bottom);
    ortho(1, 3) = -(top+bottom)/(top-bottom);
    ortho(2, 2) = -2/(far-near);
    ortho(2, 3) = -(far+near)/(far-near);
    return ortho;
}

cv::Matx44d translate(double x, double y, double z)
{
    cv::Matx44d translate = cv::Matx44d::eye();
    translate(0, 3) = x;
    translate(1, 3) = y;
    translate(2, 3) = z;
    return translate;
}
