#ifndef AR_HPP
#define AR_HPP

#include <stdio.h>
#if (defined WIN32 || defined _WIN32)
    #include <direct.h>
#else
    #include <unistd.h>
#endif
#include <iostream>
#include <sstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include "CameraCalib.h"
#include "cameraconfiguration.h"
#include "capture.h"
#include "chessboardposeestimator.h"
#if defined(__APPLE__) && defined(__MACH__)
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif
#include "rendering.hpp"


void initializeAR();
void processFrame(cv::Mat frame);
void initializePerspective();
cv::Matx44f computeMatrix(cv::Mat image);
void drawScene();
void drawCameraFrame();
void drawDetectionResults();
void drawAugmentedScene();
void drawCoordinateAxis();
void drawCubeModel();
void drawString(GLfloat x, GLfloat y, std::string text, GLfloat scale = 0.1f);

#endif // AR_HPP
