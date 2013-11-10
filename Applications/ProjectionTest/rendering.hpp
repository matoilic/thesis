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

cv::Matx44d ortho(double left, double right, double bottom, double top, double near, double far);
cv::Matx44d translate(double x, double y, double z);

#endif // RENDERING_HPP
