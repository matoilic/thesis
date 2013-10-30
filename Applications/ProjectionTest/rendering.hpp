#ifndef RENDERING_HPP
#define RENDERING_HPP

#include <GLFW/glfw3.h>

void initializeRendering();
void resizeWindow(int width, int height);
void initializeGL();
void worldReshape(int width, int height);
void worldDisplay();
void reshape(GLFWwindow* window, int width, int height);
void display();
void drawAxes();

#endif // RENDERING_HPP
