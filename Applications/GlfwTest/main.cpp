#include <iostream>
#include <stdio.h>
#include <GL/glew.h>
#if defined(__APPLE__)
    #include <OpenGL/gl.h>
#else
    #include <windows.h>
    #include <GL/GL.h>
#endif
#include <glfw/glfw3.h>

using namespace std;

void paint()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glColor3f (1.0, 1.0, 1.0);
    glBegin(GL_POLYGON);
        glVertex3f (-0.25, -0.25, 0.0);
        glVertex3f ( 0.75, -0.25, 0.0);
        glVertex3f ( 0.75,  0.75, 0.0);
        glVertex3f (-0.25,  0.75, 0.0);
    glEnd();
}

int main()
{
    if(!glfwInit()) {
        cerr << "failed initializing glfw" << endl;
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow *window = glfwCreateWindow(400, 400, "GLFW Test", NULL, NULL);
    if(!window) {
        cerr << "failed to create a glfw window" << endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);

    GLenum status = glewInit();
    if(status != GLEW_OK) {
        cerr << "GLEW error: " << glewGetErrorString(status) << endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSwapInterval(1);

    glViewport(0, 0, 400, 400);

    while(!glfwWindowShouldClose(window)) {
        paint();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}

