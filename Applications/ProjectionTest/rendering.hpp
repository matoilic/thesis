#ifndef RENDERING_HPP
#define RENDERING_HPP

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#if defined(__APPLE__) && defined(__MACH__)
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

#include "ar.hpp"

void initializeGL(int argc, char** argv);
void resizeWindow(int width, int height);
void redisplay();
void worldReshape(int width, int height);
void worldDisplay();
void reshape(int width, int height);
void display();
void drawAxes();



#endif // RENDERING_HPP
