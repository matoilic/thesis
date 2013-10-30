#include <iostream>
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
    GLfloat ambientLight[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat diffuseLight[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat position[] = { 0.0f, 0.0f, 2.0f, 1.0f };

    // Assign created components to GL_LIGHT0
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
}

void reshape(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    initializePerspective();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    drawScene();
}
