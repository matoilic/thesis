#include <iostream>
#include "glUtils.hpp"
#include "rendering.hpp"
#include "ar.hpp"

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
