#ifndef AR_HPP
#define AR_HPP

#include "glUtils.hpp"

#include <stdio.h>
#if (defined WIN32 || defined _WIN32)
    #include <direct.h>
#else
    #include <unistd.h>
#endif
#include <iostream>
#include <sstream>
#if defined(__APPLE__)
    #include <OpenGL/glext.h>
#endif
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <Ardoor/CameraCalib/CameraCalib.hpp>
#include <Ardoor/CameraCalib/CameraConfiguration.hpp>
#include <Ardoor/Capture/Capture.hpp>
#include <Ardoor/PoseEstimation/ChessboardPoseEstimator.hpp>
#include "rendering.hpp"

#define SHADER_PATH "../Data/Shader"
#define SCALE_HEIGHT 800
#define GRID_WIDTH 5
#define GRID_HEIGHT 8

typedef struct {
    std::string path;
    cv::Point2f points[4];
} DoorImage;

typedef struct {
    // program reference
    GLuint program;

    // attribute locations
    GLint position;
    GLint texCoord;

    // uniform locations
    GLint mvpMatrix;
    GLint texture0;
} TextureOnlyProgram;


typedef struct {
    // program reference
    GLuint program;

    // attribute locations
    GLint position;

    // uniform locations
    GLint mvpMatrix;
    GLint color;
} SimpleColorProgram;


typedef struct {
    // program reference
    GLuint program;

    // attribute locations
    GLint position;
    GLint normal;

    // uniform locations
    GLint mvMatrix;
    GLint mvpMatrix;
    GLint nMatrix;

    GLint globalAmbi;
    GLint lightPosVS;
    GLint lightDirVS;
    GLint lightAmbi;
    GLint lightDiff;
    GLint lightSpec;
    GLint lightSpotCut;
    GLint lightSpotCos;
    GLint lightSpotExp;
    GLint lightAtt;
    GLint matAmbi;
    GLint matDiff;
    GLint matSpec;
    GLint matEmis;
    GLint matShine;
} AdsProgram;

typedef struct {
    GLuint vboVertices;
    GLuint vboIndices;
    GLfloat color[4];
} Line;

typedef struct {
    GLuint vboVertices;
    GLuint vboIndices;
    GLfloat color[4];
} Rectangle;

typedef struct {
    GLuint vboVertices;
    GLuint vboIndices;
    GLuint textureId;
} TexturedRectangle;

typedef struct {
    Rectangle walls[6];
} Cube;


void initializeAR();
void processFrame(cv::Mat frame);

void initializeGL();
void initializePerspective();

void buildTexturedRectangle();
void buildCoordinateAxes();
void buildCube();

void drawScene();
void drawCameraFrame();
void drawAugmentedScene();
void drawCoordinateAxis();
void drawCubeModel();

void drawLine(const Line &line);
void drawRectangle(const Rectangle &rectangle);
void drawTexturedRectangle(const TexturedRectangle &texturedRectangle);
void drawCube(const Cube &cube);

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

#endif // AR_HPP
