#include <iostream>
#include "glUtils.hpp"
#include "ar.hpp"


using namespace std;


cv::Size boardSize(9, 6);
CameraCalib calib(boardSize);
CameraConfiguration camera(1726.7347f, 1732.9374f, 736.2678f, 593.4867f, 1600, 1200);

cv::Mat backgroundImage, scaledImage;
PoseEstimationResult poseResult;

float doorRatio = 1.0;
std::string shaderPath;

TextureOnlyProgram programTexture;
SimpleColorProgram programSimple;
AdsProgram programAds;

TexturedRectangle backgroundRectangle;
Line coordinationAxes[3];
Cube cube;

cv::Matx44f modelViewMatrix = cv::Matx44f::eye();
cv::Matx44f projectionMatrix = cv::Matx44f::eye();

float cubeColor[] = { 1.0f, 1.0f, 0.0f, 0.4f };


int doorIndex = 0;
DoorImage images[] = {
    { "/../../Data/Images/Doors/door-1.jpg", { cv::Point2f(127, 26), cv::Point2f(250, 6), cv::Point2f(281, 339), cv::Point2f(168, 326) }},
    { "/../../Data/Images/Doors/door-2.jpg", { cv::Point2f(266, 59), cv::Point2f(428, 106), cv::Point2f(371, 518), cv::Point2f(211, 555) }},
    { "/../../Data/Images/Doors/door-3.jpg", { cv::Point2f(422, 31), cv::Point2f(669, 30), cv::Point2f(610, 589), cv::Point2f(397, 521) }}
};


template<typename T, int m, int n>
void printMat(const cv::Matx<T, m, n> &mat)
{
    for (int r = 0; r < mat.rows; r++)
    {
        for (int c = 0; c < mat.cols; c++)
        {
            cout << mat.row(r).col(c) << "\t";
        }

        cout << endl;
    }
}

void printMat(const cv::Mat &mat)
{
    for (int r = 0; r < mat.rows; r++)
    {
        for (int c = 0; c < mat.cols; c++)
        {
            cout << mat.row(r).col(c) << "\t";
        }

        cout << endl;
    }
}

float getRatio(cv::Matx33f cameraMatrix, cv::Point2f c1, cv::Point2f c2, cv::Point2f c3, cv::Point2f c4)
{
    cv::Mat A(cameraMatrix);

    float k2, k3;
    cv::Mat _ratio;
    cv::Mat n2, n3;

    cv::Mat m1 = (cv::Mat_<float>(3,1) << (float) c1.x, (float) c1.y, 1);
    cv::Mat m2 = (cv::Mat_<float>(3,1) << (float) c4.x, (float) c4.y, 1);
    cv::Mat m3 = (cv::Mat_<float>(3,1) << (float) c2.x, (float) c2.y, 1);
    cv::Mat m4 = (cv::Mat_<float>(3,1) << (float) c3.x, (float) c3.y, 1);

    k2 = (m1.cross(m4).dot(m3)) / ((m2.cross(m4)).dot(m3));
    k3 = (m1.cross(m4).dot(m2)) / ((m3.cross(m4)).dot(m2));
    n2 = (k2*m2) - m1;
    n3 = (k3*m3) - m1;

    _ratio = (n2.t()*(A.inv().t())*(A.inv())*n2) / (n3.t()*(A.inv().t())*(A.inv())*n3);

    return sqrt(_ratio.at<float>(0,0));
}

void initializeAR()
{
    std::cout << "initializeAR()" << std::flush << std::endl;
}


void processFrame(cv::Mat frame)
{
    std::cout << "processFrame()" << std::endl << std::flush;

    char cwd[FILENAME_MAX];
    getcwd(cwd, sizeof(cwd));

    DoorImage doorImage = images[doorIndex];

    stringstream path;
    path << cwd << doorImage.path;

    cv::Mat sourceImage = cv::imread(path.str(), CV_LOAD_IMAGE_COLOR);
    backgroundImage = sourceImage;

    poseResult.isObjectPresent = true;

    CameraConfiguration c = camera.scale(sourceImage.cols, sourceImage.rows);
    cv::Matx33f M = c.getIntrinsics();
    cv::Mat_<float> D = c.getDistorsion();

    std::vector<cv::Point2f> imagePoints;
    imagePoints.push_back(doorImage.points[0]);
    imagePoints.push_back(doorImage.points[1]);
    imagePoints.push_back(doorImage.points[2]);
    imagePoints.push_back(doorImage.points[3]);

    doorRatio = getRatio(M, imagePoints.at(0), imagePoints.at(1), imagePoints.at(2), imagePoints.at(3));
    if (doorRatio < 1) {
        doorRatio = 1/doorRatio;
    }

    std::vector<cv::Point3f> objectPoints;
    objectPoints.push_back(cv::Point3f(0, doorRatio, 0));
    objectPoints.push_back(cv::Point3f(1, doorRatio, 0));
    objectPoints.push_back(cv::Point3f(1, 0, 0));
    objectPoints.push_back(cv::Point3f(0, 0, 0));

    cv::Vec<float, 3> R;
    cv::Vec<float, 3> T;
    cv::Mat rvec, tvec;

    cv::solvePnP(cv::Mat(objectPoints), cv::Mat(imagePoints), M, D, rvec, tvec); // Calculate the Rotation and Translation vector

    rvec.convertTo(R, CV_32FC1);
    tvec.convertTo(T, CV_32FC1);

    std::cout.precision(2);
    std::cout << "rvec = [" << R(0) << ", " << R(1) << ", " << R(2) << "]   ";
    std::cout << "tvec = [" << T(0) << ", " << T(1) << ", " << T(2) << "]   ";
    std::cout << std::endl;

    cv::Matx33f rotMat;
    cv::Rodrigues(R, rotMat);

    // Build combined rotation and translation matrix
    cv::Matx44f Rt = cv::Matx44f::eye();

    Rt(0, 0) = rotMat(0, 0);
    Rt(0, 1) = rotMat(0, 1);
    Rt(0, 2) = -rotMat(0, 2);
    Rt(0, 3) = T(0);

    Rt(1, 0) = rotMat(1, 0);
    Rt(1, 1) = rotMat(1, 1);
    Rt(1, 2) = -rotMat(1, 2);
    Rt(1, 3) = T(1);

    Rt(2, 0) = rotMat(2, 0);
    Rt(2, 1) = rotMat(2, 1);
    Rt(2, 2) = -rotMat(2, 2);
    Rt(2, 3) = T(2);

    // OpenGL has Y and Z axes reversed
    cv::Matx44f reverseYZ = cv::Matx44f::eye();
    reverseYZ(1, 1) = -1.0;
    reverseYZ(2, 2) = -1.0;

    poseResult.mvMatrix = reverseYZ * Rt;

    resizeWindow(backgroundImage.cols, backgroundImage.rows);
}


void initializeGL()
{
    std::cout << "initializeGL()" << std::flush << std::endl;

    char cwd[FILENAME_MAX];
    getcwd(cwd, sizeof(cwd));
    std::stringstream path;
    path << cwd << "/../../Data/Shader";
    shaderPath = path.str();


    buildTexturedRectangle();
    buildCoordinateAxes();
    buildCube();

    glEnable(GL_DEPTH_TEST);            // Enables depth test
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Load, compile & link shaders

    // ADS Shader
    GLuint shaderVertID = glUtils::buildShader(shaderPath + string("/ADS.vert"), GL_VERTEX_SHADER);
    GLuint shaderFragID = glUtils::buildShader(shaderPath + string("/ADS.frag"), GL_FRAGMENT_SHADER);
    programAds.program = glUtils::buildProgram(shaderVertID, shaderFragID);

    programAds.position      = glGetAttribLocation(programAds.program, "a_position"); GET_GL_ERROR;
    programAds.normal        = glGetAttribLocation(programAds.program, "a_normal"); GET_GL_ERROR;

    programAds.mvMatrix      = glGetUniformLocation(programAds.program, "u_mvMatrix"); GET_GL_ERROR;
    programAds.mvpMatrix     = glGetUniformLocation(programAds.program, "u_mvpMatrix"); GET_GL_ERROR;
    programAds.nMatrix       = glGetUniformLocation(programAds.program, "u_nMatrix"); GET_GL_ERROR;

    programAds.globalAmbi    = glGetUniformLocation(programAds.program, "u_globalAmbi"); GET_GL_ERROR;
    programAds.lightPosVS    = glGetUniformLocation(programAds.program, "u_lightPosVS"); GET_GL_ERROR;
    programAds.lightDirVS    = glGetUniformLocation(programAds.program, "u_lightDirVS"); GET_GL_ERROR;
    programAds.lightAmbi     = glGetUniformLocation(programAds.program, "u_lightAmbi"); GET_GL_ERROR;
    programAds.lightDiff     = glGetUniformLocation(programAds.program, "u_lightDiff"); GET_GL_ERROR;
    programAds.lightSpec     = glGetUniformLocation(programAds.program, "u_lightSpec"); GET_GL_ERROR;
    programAds.lightSpotCut  = glGetUniformLocation(programAds.program, "u_lightSpotCut"); GET_GL_ERROR;
    programAds.lightSpotCos  = glGetUniformLocation(programAds.program, "u_lightSpotCos"); GET_GL_ERROR;
    programAds.lightSpotExp  = glGetUniformLocation(programAds.program, "u_lightSpotExp"); GET_GL_ERROR;
    programAds.lightAtt      = glGetUniformLocation(programAds.program, "u_lightAtt"); GET_GL_ERROR;
    programAds.matAmbi       = glGetUniformLocation(programAds.program, "u_matAmbi"); GET_GL_ERROR;
    programAds.matDiff       = glGetUniformLocation(programAds.program, "u_matDiff"); GET_GL_ERROR;
    programAds.matSpec       = glGetUniformLocation(programAds.program, "u_matSpec"); GET_GL_ERROR;
    programAds.matEmis       = glGetUniformLocation(programAds.program, "u_matEmis"); GET_GL_ERROR;
    programAds.matShine      = glGetUniformLocation(programAds.program, "u_matShine"); GET_GL_ERROR;


    // TextureOnly Shader
    shaderVertID = glUtils::buildShader(shaderPath + string("/TextureOnly.vert"), GL_VERTEX_SHADER);
    shaderFragID = glUtils::buildShader(shaderPath + string("/TextureOnly.frag"), GL_FRAGMENT_SHADER);
    programTexture.program = glUtils::buildProgram(shaderVertID, shaderFragID);

    programTexture.position   = glGetAttribLocation(programTexture.program, "a_position"); GET_GL_ERROR;
    programTexture.texCoord   = glGetAttribLocation(programTexture.program, "a_texCoord"); GET_GL_ERROR;
    programTexture.mvpMatrix  = glGetUniformLocation(programTexture.program, "u_mvpMatrix"); GET_GL_ERROR;
    programTexture.texture0   = glGetUniformLocation(programTexture.program, "u_texture0"); GET_GL_ERROR;


    // SimpleLine Shader
    shaderVertID = glUtils::buildShader(shaderPath + string("/SimpleColor.vert"), GL_VERTEX_SHADER);
    shaderFragID = glUtils::buildShader(shaderPath + string("/SimpleColor.frag"), GL_FRAGMENT_SHADER);
    programSimple.program = glUtils::buildProgram(shaderVertID, shaderFragID);

    programSimple.position   = glGetAttribLocation(programSimple.program, "a_position"); GET_GL_ERROR;
    programSimple.mvpMatrix  = glGetUniformLocation(programSimple.program, "u_mvpMatrix"); GET_GL_ERROR;
    programSimple.color      = glGetUniformLocation(programSimple.program, "u_color"); GET_GL_ERROR;
}

void initializePerspective()
{
    std::cout << "initializePerspective()" << std::flush << std::endl;

    projectionMatrix = cv::Matx44f::eye();

    if (!backgroundImage.empty()) {
        float near = 0.1f;
        float far = 1000.0f;
        float w = backgroundImage.cols;
        float h = backgroundImage.rows;

        CameraConfiguration cam = camera.scale(backgroundImage.cols, backgroundImage.rows);
        float fx = cam.getFocalLengthX();
        float fy = cam.getFocalLengthY();
        float cx = cam.getPrimaryPointX();
        float cy = cam.getPrimaryPointY();
        float x0 = cx - w/2;
        float y0 = cy - h/2;

        cv::Matx44f proj = cv::Matx44f::zeros();
        proj(0, 0) = fx;
        proj(0, 2) = -x0;
        proj(1, 1) = fy;
        proj(1, 2) = -y0;
        proj(2, 2) = near + far;
        proj(2, 3) = near * far;
        proj(3, 2) = -1.0f;

        projectionMatrix = ortho(-w/2, w/2, -h/2, h/2, near, far);
        projectionMatrix = projectionMatrix * proj;
    }
}



void buildTexturedRectangle()
{
   std::cout << "buildSquare()" << std::flush << std::endl;

   GLfloat v[] = {  -1, -1,  0,     0,  0,  1,     0,  1
                 ,   1, -1,  0,     0,  0,  1,     1,  1
                 ,   1,  1,  0,     0,  0,  1,     1,  0
                 ,  -1,  1,  0,     0,  0,  1,     0,  0
                 };

   GLuint i[] = {0, 1, 2, 0, 2, 3};

   backgroundRectangle.vboVertices = glUtils::buildVBO(v, 4, 8, sizeof(GLfloat), GL_ARRAY_BUFFER, GL_STATIC_DRAW);
   backgroundRectangle.vboIndices = glUtils::buildVBO(i, 6, 1, sizeof(GLuint), GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
}

void buildCoordinateAxes()
{
    GLfloat v[] = {   0,  0,  0     // Origin of all axes
                  ,   1,  0,  0     // 2nd point x-axis
                  ,   0,  1,  0     // 2nd point y-axis
                  ,   0,  0,  1     // 2nd point z-axis
                  };

    GLuint indices[3][2] = { { 0, 1 }
                           , { 0, 2 }
                           , { 0, 3 }
                           };

    GLfloat colors[3][4] = { { 1, 0, 0, 1 }   // red
                           , { 0, 1, 0, 1 }   // green
                           , { 0, 0, 1, 1 }   // blue
                           };

    GLuint vboVertices = glUtils::buildVBO(v, 4, 3, sizeof(GLfloat), GL_ARRAY_BUFFER, GL_STATIC_DRAW);


    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 4; ++j) {
            coordinationAxes[i].color[j] = colors[i][j];
        }
        coordinationAxes[i].vboVertices = vboVertices;
        coordinationAxes[i].vboIndices = glUtils::buildVBO(indices[i], 2, 1, sizeof(GLuint), GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
    }
}

void buildCube()
{
    GLfloat v[6][24] = { // bottom rectangle
                        {  -1, -1, -1,     0, -1,  0
                        ,   1, -1, -1,     0, -1,  0
                        ,   1, -1,  1,     0, -1,  0
                        ,  -1, -1,  1,     0, -1,  0
                        }
                        // top rectangle
                      , {  -1,  1,  1,     0,  1,  0
                        ,   1,  1,  1,     0,  1,  0
                        ,   1,  1, -1,     0,  1,  0
                        ,  -1,  1, -1,     0,  1,  0
                        }
                        // left rectangle
                      , {  -1, -1, -1,    -1,  0,  0
                        ,  -1, -1,  1,    -1,  0,  0
                        ,  -1,  1,  1,    -1,  0,  0
                        ,  -1,  1, -1,    -1,  0,  0
                        }
                        // right rectangle
                      , {   1, -1,  1,     1,  0,  0
                        ,   1, -1, -1,     1,  0,  0
                        ,   1,  1, -1,     1,  0,  0
                        ,   1,  1,  1,     1,  0,  0
                        }
                        // front rectangle
                      , {  -1, -1,  1,     0,  0,  1
                        ,   1, -1,  1,     0,  0,  1
                        ,   1,  1,  1,     0,  0,  1
                        ,  -1,  1,  1,     0,  0,  1
                        }
                        // back rectangle
                      , {  -1, -1, -1,     0,  0, -1
                        ,   1, -1, -1,     0,  0, -1
                        ,   1,  1, -1,     0,  0, -1
                        ,  -1,  1, -1,     0,  0, -1
                        }
                      };

    GLuint indices[] = {0, 1, 2, 0, 2, 3};

    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 4; ++j) {
            cube.walls[i].color[j] = cubeColor[j];
        }
        cube.walls[i].vboVertices = glUtils::buildVBO(v[i], 4, 6, sizeof(GLfloat), GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
        cube.walls[i].vboIndices = glUtils::buildVBO(indices, 6, 1, sizeof(GLuint), GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
    }
}

void drawScene()
{
    modelViewMatrix = cv::Matx44f::eye();

    drawCameraFrame();
    drawAugmentedScene();
}

void drawAugmentedScene()
{
    processFrame(scaledImage);

    if (poseResult.isObjectPresent)
    {
        // Set the pattern transformation
        modelViewMatrix = poseResult.mvMatrix * modelViewMatrix;

        // Render model
        drawCoordinateAxis();
        drawCubeModel();
    }
}

void matToTexture(GLuint &textureId, const cv::Mat &mat)
{
    int width = mat.cols, height = mat.rows;

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // use fast 4-byte alignment (default anyway) if possible
    glPixelStorei(GL_UNPACK_ALIGNMENT, (mat.step & 3) ? 1 : 4);

    // set length of one complete row in data (doesn't need to equal image.cols)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, mat.step / mat.elemSize());

    // Upload new texture data:
    if (mat.channels() == 3)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, mat.data);
    else if(mat.channels() == 4)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, mat.data);
    else if (mat.channels() == 1)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, mat.data);
}

void drawCameraFrame()
{
    if (backgroundImage.empty())
        return;

    matToTexture(backgroundRectangle.textureId, backgroundImage);

    // Backup current matrix values
    cv::Matx44f projectionBackup = projectionMatrix;
    cv::Matx44f modelViewBackup = modelViewMatrix;

    // Begin othogonal projection
    cv::Matx44f projectionMatrix = ortho(-1, 1, -1, 1, -1, 1000);
    cv::Matx44f modelViewMatrix = translate(0, 0, -1000);

    // Activate the shader programm
    glUseProgram(programTexture.program); GET_GL_ERROR;

    // Pass the matrix uniform variables
    cv::Matx44f mvp = projectionMatrix * modelViewMatrix;
    cv::Matx44f mvpT = mvp.t();
    glUniformMatrix4fv(programTexture.mvpMatrix, 1, GL_FALSE, (GLfloat*) mvpT.val); GET_GL_ERROR;

    // Pass the active texture unit
    glActiveTexture(GL_TEXTURE0); GET_GL_ERROR;
    glBindTexture(GL_TEXTURE_2D, backgroundRectangle.textureId); GET_GL_ERROR;
    glUniform1i(programTexture.texture0, 0); GET_GL_ERROR;

    glEnableVertexAttribArray(programTexture.position); GET_GL_ERROR;
    glEnableVertexAttribArray(programTexture.texCoord); GET_GL_ERROR;

    GLsizei stride = 8 * sizeof(GLfloat);
    GLsizei offsetTexCoords = 6 * sizeof(GLfloat);

    glBindBuffer(GL_ARRAY_BUFFER, backgroundRectangle.vboVertices); GET_GL_ERROR;
    glVertexAttribPointer(programTexture.position, 3, GL_FLOAT, GL_FALSE, stride, 0); GET_GL_ERROR;

    glBindBuffer(GL_ARRAY_BUFFER, backgroundRectangle.vboVertices); GET_GL_ERROR;
    glVertexAttribPointer(programTexture.texCoord, 2, GL_FLOAT, GL_FALSE, stride, (void*) offsetTexCoords); GET_GL_ERROR;

    // Draw cube with triangles by indexes
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, backgroundRectangle.vboIndices); GET_GL_ERROR;
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); GET_GL_ERROR;

    // Deactivate buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0); GET_GL_ERROR;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); GET_GL_ERROR;

    // Disable the vertex arrays
    glDisableVertexAttribArray(programTexture.position); GET_GL_ERROR;
    glDisableVertexAttribArray(programTexture.texCoord); GET_GL_ERROR;


    // Restore previous matrix values
    projectionMatrix = projectionBackup;
    modelViewMatrix = modelViewBackup;
}


void drawCoordinateAxis()
{
    //modelViewMatrix = modelViewMatrix * scale(1, doorRatio, 1);

    for (int i = 0; i < 3; ++i) {
        drawLine(coordinationAxes[i]);
    }
}


void drawCubeModel()
{
    // In the object coordinate system, the up-axis is the y-axis. On the chessboard, the y-axis
    // is the axis away from the chessboard (to the viewer). We want the object to "stand" on the chessboard
    // so we rotate the object, so that the both up-axes are aligned.
   // modelViewMatrix = modelViewMatrix * rotate(90, 1, 0, 0);


    // We create a 2x2 cube here around the (0, 0) point in the object coordinate system.
    // Because the chessboard is on the z=0 plane, we have to translate the cube so the lower bounds of the
    // objekt are on the z=0 plane.
    //modelViewMatrix = modelViewMatrix * translate(0, 1, 0);

    //modelViewMatrix = modelViewMatrix * scale(1, 1/doorRatio, 1);
    //modelViewMatrix = modelViewMatrix * translate(0, doorRatio, 0);
    //modelViewMatrix = modelViewMatrix * scale(1, 0.05, 1);


    modelViewMatrix = modelViewMatrix * translate(0.5, doorRatio, 0);
    modelViewMatrix = modelViewMatrix * scale(0.5, 0.05, 0.3);
    modelViewMatrix = modelViewMatrix * translate(0, 2, -1);

    //modelViewMatrix = modelViewMatrix * translate(1, 1, -1);

    drawCube(cube);
}


void drawLine(const Line &line)
{
    // Activate the shader programm
    glUseProgram(programSimple.program); GET_GL_ERROR;

    // Pass the matrix uniform variables
    cv::Matx44f mvp = projectionMatrix * modelViewMatrix;
    cv::Matx44f mvpT = mvp.t();
    glUniformMatrix4fv(programSimple.mvpMatrix, 1, GL_FALSE, (GLfloat*) mvpT.val); GET_GL_ERROR;

    glLineWidth(2);
    glUniform4fv(programSimple.color, 1, line.color); GET_GL_ERROR;

    glEnableVertexAttribArray(programSimple.position); GET_GL_ERROR;
    glBindBuffer(GL_ARRAY_BUFFER, line.vboVertices); GET_GL_ERROR;

    // draw line
    glVertexAttribPointer(programSimple.position, 3, GL_FLOAT, GL_FALSE, 0, 0); GET_GL_ERROR;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, line.vboIndices); GET_GL_ERROR;
    glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0); GET_GL_ERROR;

    // Deactivate buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0); GET_GL_ERROR;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); GET_GL_ERROR;

    // Disable the vertex arrays
    glDisableVertexAttribArray(programSimple.position); GET_GL_ERROR;
}

void drawRectangle(const Rectangle &rectangle)
{
    // Activate the shader programm
    glUseProgram(programSimple.program); GET_GL_ERROR;

    // Pass the matrix uniform variables
    cv::Matx44f mvp = projectionMatrix * modelViewMatrix;
    cv::Matx44f mvpT = mvp.t();
    glUniformMatrix4fv(programSimple.mvpMatrix, 1, GL_FALSE, (GLfloat*) mvpT.val); GET_GL_ERROR;

    glUniform4fv(programSimple.color, 1, rectangle.color); GET_GL_ERROR;

    glEnableVertexAttribArray(programSimple.position); GET_GL_ERROR;

    GLsizei stride = 6 * sizeof(GLfloat);
    glBindBuffer(GL_ARRAY_BUFFER, rectangle.vboVertices); GET_GL_ERROR;
    glVertexAttribPointer(programSimple.position, 3, GL_FLOAT, GL_FALSE, stride, 0); GET_GL_ERROR;

    // Draw cube with triangles by indexes
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectangle.vboIndices); GET_GL_ERROR;
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); GET_GL_ERROR;

    // Deactivate buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0); GET_GL_ERROR;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); GET_GL_ERROR;

    // Disable the vertex arrays
    glDisableVertexAttribArray(programSimple.position); GET_GL_ERROR;

}

void drawTexturedRectangle(const TexturedRectangle &texturedRectangle)
{
    // Activate the shader programm
    glUseProgram(programTexture.program); GET_GL_ERROR;

    // Pass the matrix uniform variables
    cv::Matx44f mvp = projectionMatrix * modelViewMatrix;
    cv::Matx44f mvpT = mvp.t();
    glUniformMatrix4fv(programTexture.mvpMatrix, 1, GL_FALSE, (GLfloat*) mvpT.val); GET_GL_ERROR;

    // Pass the active texture unit
    glActiveTexture(GL_TEXTURE0); GET_GL_ERROR;
    glBindTexture(GL_TEXTURE_2D, texturedRectangle.textureId); GET_GL_ERROR;
    glUniform1i(programTexture.texture0, 0); GET_GL_ERROR;

    glEnableVertexAttribArray(programTexture.position); GET_GL_ERROR;
    glEnableVertexAttribArray(programTexture.texCoord); GET_GL_ERROR;

    GLsizei stride = 8 * sizeof(GLfloat);
    GLsizei offsetTexCoords = 6 * sizeof(GLfloat);

    glBindBuffer(GL_ARRAY_BUFFER, texturedRectangle.vboVertices); GET_GL_ERROR;
    glVertexAttribPointer(programTexture.position, 3, GL_FLOAT, GL_FALSE, stride, 0); GET_GL_ERROR;

    glBindBuffer(GL_ARRAY_BUFFER, texturedRectangle.vboVertices); GET_GL_ERROR;
    glVertexAttribPointer(programTexture.texCoord, 2, GL_FLOAT, GL_FALSE, stride, (void*) offsetTexCoords); GET_GL_ERROR;

    // Draw cube with triangles by indexes
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, texturedRectangle.vboIndices); GET_GL_ERROR;
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); GET_GL_ERROR;

    // Deactivate buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0); GET_GL_ERROR;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); GET_GL_ERROR;

    // Disable the vertex arrays
    glDisableVertexAttribArray(programTexture.position); GET_GL_ERROR;
    glDisableVertexAttribArray(programTexture.texCoord); GET_GL_ERROR;
}


void drawCube(const Cube &cube)
{
    for (int i = 0; i < 6; ++i) {
        drawRectangle(cube.walls[i]);
    }
}


void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_1) doorIndex = 0;
    if (key == GLFW_KEY_2) doorIndex = 1;
    if (key == GLFW_KEY_3) doorIndex = 2;
}

