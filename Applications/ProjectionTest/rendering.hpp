#ifndef RENDERING_HPP
#define RENDERING_HPP

#include "glUtils.hpp"
#include <GLFW/glfw3.h>
#include <opencv2/core/core.hpp>

void initializeRendering();
void resizeWindow(int width, int height);
void initializeGL();
void worldReshape(int width, int height);
void worldDisplay();
void reshape(GLFWwindow* window, int width, int height);
void display();
void drawAxes();

cv::Matx44f ortho(float left, float right, float bottom, float top, float near, float far);
cv::Matx44f translate(float x, float y, float z);
cv::Matx44f rotate(float angle, float x, float y, float z);

#endif // RENDERING_HPP
