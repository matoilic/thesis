#include <iostream>
#include "glUtils.hpp"
#include "ar.hpp"


using namespace std;


cv::Size boardSize(9, 6);
CameraCalib calib(boardSize);
cv::Size calibrationSize(1600, 1200);
CameraConfiguration camera(1726.7347f, 1732.9374f, 736.2678f, 593.4867f, calibrationSize);
ChessboardPoseEstimator poseEstimator(calib, camera);
Capture capture;

cv::Mat backgroundImage;
PoseEstimationResult poseResult;

std::string shaderPath;

bool isTextureInitialized = false;
GLuint textureId = -1;

GLuint programTexture;
GLuint programAds;

GLfloat vboSquareVertices;
GLuint vboSquareIndices;
GLfloat vboTexCoords;

cv::Matx44d modelViewMatrix = cv::Matx44d::eye();
cv::Matx44d projectionMatrix = cv::Matx44d::eye();


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

void initializeAR()
{
    std::cout << "initializeAR()" << std::flush << std::endl;

    char cwd[FILENAME_MAX];
    getcwd(cwd, sizeof(cwd));
    std::stringstream inputFilePath;
    inputFilePath << cwd << "/../../Applications/ProjectionTest/board.avi";
    std::string inputFile = inputFilePath.str();
    std::cout << "using " << inputFile << " as video source" << std::endl;

    capture.setHandler(processFrame);
    capture.start(/*inputFile*/);
}


/*!
buildSquare creates the vertex attributes for a textured square and VBO.
*/
void buildSquare()
{
    std::cout << "buildSquare()" << std::flush << std::endl;

   GLfloat v[] = {-1, -1,
                  1, -1,
                  1, 1,
                  -1, 1};

   GLfloat t[] = {0, 1,
                  1, 1,
                  1, 0,
                  0, 0};

   GLuint i[] = {0, 1, 2, 0, 2, 3};

   vboSquareVertices = glUtils::buildVBO(v, 4, 2, sizeof(GLfloat), GL_ARRAY_BUFFER, GL_STATIC_DRAW);
   vboSquareIndices = glUtils::buildVBO(i, 6, 1, sizeof(GLuint), GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
   vboTexCoords = glUtils::buildVBO(t, 4, 2, sizeof(GLfloat), GL_ARRAY_BUFFER, GL_STATIC_DRAW);
}


void initializeGL()
{
    std::cout << "initializeGL()" << std::flush << std::endl;

    char cwd[FILENAME_MAX];
    getcwd(cwd, sizeof(cwd));
    std::stringstream path;
    path << cwd << "/../../Data/Shader";
    shaderPath = path.str();


    buildSquare();

    glClearColor(0.0f, 0.0f, 0.0f, 1);  // Set the background color
    glEnable(GL_DEPTH_TEST);            // Enables depth test
    glEnable(GL_CULL_FACE);             // Enables the culling of back faces

    // Load, compile & link shaders
    GLuint shaderVertID = glUtils::buildShader(shaderPath + string("/ADS.vert"), GL_VERTEX_SHADER);
    GLuint shaderFragID = glUtils::buildShader(shaderPath + string("/ADS.frag"), GL_FRAGMENT_SHADER);
    programAds = glUtils::buildProgram(shaderVertID, shaderFragID);

    shaderVertID = glUtils::buildShader(shaderPath + string("/TextureOnly.vert"), GL_VERTEX_SHADER);
    shaderFragID = glUtils::buildShader(shaderPath + string("/TextureOnly.frag"), GL_FRAGMENT_SHADER);
    programTexture = glUtils::buildProgram(shaderVertID, shaderFragID);
}


void processFrame(cv::Mat frame)
{
    cv::flip(frame, frame, 1);
    poseResult = poseEstimator.estimatePose(frame);

    frame.copyTo(backgroundImage);

    resizeWindow(backgroundImage.size().width, backgroundImage.size().height);
}

void initializePerspective()
{
    std::cout << "initializePerspective()" << std::flush << std::endl;

    projectionMatrix = cv::Matx44d::eye();

    if (!backgroundImage.empty()) {
        float near = 0.1f;
        float far = 1000.0f;
        float w = backgroundImage.cols;
        float h = backgroundImage.rows;

        CameraConfiguration *cam = camera.scale(backgroundImage.size());
        float fx = cam->getFocalLengthX();
        float fy = cam->getFocalLengthY();
        float cx = cam->getPrimaryPointX();
        float cy = cam->getPrimaryPointY();
        float x0 = cx - w/2;
        float y0 = cy - h/2;

        cv::Matx44d proj = cv::Matx44d::zeros();
        proj(0, 0) = fx;
        proj(0, 2) = -x0;
        proj(1, 1) = fy;
        proj(1, 2) = -y0;
        proj(2, 2) = near + far;
        proj(2, 3) = near * far;
        proj(3, 2) = -1.0f;

        projectionMatrix = ortho(-w/2, w/2, -h/2, h/2, near, far);
        projectionMatrix = projectionMatrix * proj;

        delete cam;
    }
}

void drawScene()
{
    modelViewMatrix = cv::Matx44d::eye();

    drawCameraFrame();
    drawDetectionResults();
    drawAugmentedScene();
}

void drawDetectionResults()
{
    float x = poseResult.mvMatrix(0, 3);
    float y = poseResult.mvMatrix(1, 3);
    float z = poseResult.mvMatrix(2, 3);

    std::stringstream s;
    s.precision(2);
    s << "Translation - x(" << x << ") y(" << y << ") z(" << z << ")";

    //drawString(40, 40, s.str());
}

void drawAugmentedScene()
{
    if (poseResult.isObjectPresent)
    {
        // Set the pattern transformation
        modelViewMatrix = poseResult.mvMatrix * modelViewMatrix;

        // Render model
        //drawCoordinateAxis();
        //drawCubeModel();
    }
}

//void drawString(GLfloat x, GLfloat y, std::string text, GLfloat scale)
//{
//    //begin othogonal projection
//    glMatrixMode(GL_PROJECTION);
//    glPushMatrix();
//    glLoadIdentity();
//    glOrtho(0.0, backgroundImage.cols, 0.0, backgroundImage.rows, 0.0, 1000.0);

//    glMatrixMode(GL_MODELVIEW);
//    glPushMatrix();
//    glLoadIdentity();

//    glEnable(GL_COLOR_MATERIAL);
//    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
//    glScalef(scale, scale, 1.0f);
//    glTranslatef(x, y, 0.0f);

//    //for (int i = 0; i < text.length(); i++) {
//    //    glutStrokeCharacter(GLUT_STROKE_ROMAN, text.at(i));
//    //}

//    //end orthogonal projection
//    glMatrixMode(GL_PROJECTION);
//    glPopMatrix();
//    glMatrixMode(GL_MODELVIEW);
//    glPopMatrix();
//}

void matToTexture(GLuint &textureId, const cv::Mat &mat)
{
    int width = mat.cols, height = mat.rows;

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glPixelStorei(GL_PACK_ALIGNMENT, 3);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 3);
    glBindTexture(GL_TEXTURE_2D, textureId);

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

    matToTexture(textureId, backgroundImage);

    // Begin othogonal projection
    cv::Matx44f orthoProj = ortho(-1, 1, -1, 1, -1, 1000);
    cv::Matx44f translation = translate(0, 0, -1000);
    cv::Matx44f mvp = orthoProj * translation;

    // Activate the shader programm
    glUseProgram(programTexture); GET_GL_ERROR;
//glsldevil

    // Get the variable locations (identifiers) within the program
    GLint positionLoc = glGetAttribLocation(programTexture, "a_position"); GET_GL_ERROR;
    GLint texCoordLoc = glGetAttribLocation(programTexture, "a_texCoord"); GET_GL_ERROR;
    GLint mvpMatrixLoc = glGetUniformLocation(programTexture, "u_mvpMatrix"); GET_GL_ERROR;
    GLint texture0Loc = glGetUniformLocation(programTexture, "u_texture0"); GET_GL_ERROR;

    // Pass the matrix uniform variables
    cv::Matx44f mvpT = mvp.t();
    glUniformMatrix4fv(mvpMatrixLoc, 1, GL_FALSE, (GLfloat*) mvpT.val); GET_GL_ERROR;

    // Pass the active texture unit
    glActiveTexture(GL_TEXTURE0); GET_GL_ERROR;
    glBindTexture(GL_TEXTURE_2D, textureId); GET_GL_ERROR;
    glUniform1i(texture0Loc, 0); GET_GL_ERROR;

    glEnableVertexAttribArray(positionLoc); GET_GL_ERROR;
    glEnableVertexAttribArray(texCoordLoc); GET_GL_ERROR;

    glBindBuffer(GL_ARRAY_BUFFER, vboSquareVertices); GET_GL_ERROR;
    glVertexAttribPointer(positionLoc, 2, GL_FLOAT, GL_FALSE, 0, 0); GET_GL_ERROR;

    glBindBuffer(GL_ARRAY_BUFFER, vboTexCoords); GET_GL_ERROR;
    glVertexAttribPointer(texCoordLoc, 2, GL_FLOAT, GL_FALSE, 0, 0); GET_GL_ERROR;

    // Draw cube with triangles by indexes
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboSquareIndices); GET_GL_ERROR;
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); GET_GL_ERROR;

    // Deactivate buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0); GET_GL_ERROR;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); GET_GL_ERROR;

    // Disable the vertex arrays
    glDisableVertexAttribArray(positionLoc); GET_GL_ERROR;
    glDisableVertexAttribArray(texCoordLoc); GET_GL_ERROR;
}

//void drawCoordinateAxis()
//{
//    float w = boardSize.width;
//    float h = boardSize.height;
//    float w2 = (w+1)/2;
//    float h2 = (h+1)/2;

//    static float lineX[] = {-w2,h2,0,w2,h2,0};
//    static float lineY[] = {-w2,h2,0,-w2,-h2,0};
//    static float lineZ[] = {-w2,h2,0,-w2,h2,h2};

//    glLineWidth(2);

//    glEnable(GL_COLOR_MATERIAL);

//    glBegin(GL_LINES);

//    glColor3f(1.0f, 0.0f, 0.0f);
//    glVertex3fv(lineX);
//    glVertex3fv(lineX + 3);

//    glColor3f(0.0f, 1.0f, 0.0f);
//    glVertex3fv(lineY);
//    glVertex3fv(lineY + 3);

//    glColor3f(0.0f, 0.0f, 1.0f);
//    glVertex3fv(lineZ);
//    glVertex3fv(lineZ + 3);

//    glEnd();
//}


//void drawCubeModel()
//{
//  static const GLfloat LightAmbient[]=  { 0.25f, 0.25f, 0.25f, 1.0f };    // Ambient Light Values
//  static const GLfloat LightDiffuse[]=  { 0.1f, 0.1f, 0.1f, 1.0f };    // Diffuse Light Values
//  static const GLfloat LightPosition[]= { 0.0f, 0.0f, 2.0f, 1.0f };    // Light Position

//  glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT | GL_LIGHTING_BIT | GL_POLYGON_BIT);

//  glColor4f(0.2f,0.35f,0.3f,0.75f);         // Full Brightness, 50% Alpha ( NEW )
//  glBlendFunc(GL_ONE,GL_ONE_MINUS_SRC_ALPHA);       // Blending Function For Translucency Based On Source Alpha
//  glEnable(GL_BLEND);

//  glShadeModel(GL_SMOOTH);

//  glEnable(GL_LIGHTING);
//  glDisable(GL_LIGHT0);
//  glEnable(GL_LIGHT1);
//  glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
//  glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
//  glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);


//  // In the object coordinate system, the up-axis is the y-axis. On the chessboard, the y-axis
//  // is the axis away from the chessboard (to the viewer). We want the object to "stand" on the chessboard
//  // so we rotate the object, so that the both up-axes are aligned.
//  glRotatef(90, 1.0f, 0.0f, 0.0f);

//  // We create a 2x2 cube here around the (0, 0) point in the object coordinate system.
//  // Because the chessboard is on the z=0 plane, we have to translate the cube so the lower bounds of the
//  // objekt are on the z=0 plane.
//  glTranslatef(0.0f, 1.0f, 0.0f);


//  glEnable(GL_COLOR_MATERIAL);

//  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//  glBegin(GL_QUADS);
//  // Front Face
//  glNormal3f( 0.0f, 0.0f, 1.0f);    // Normal Pointing Towards Viewer
//  glVertex3f(-1.0f, -1.0f,  1.0f);  // Point 1 (Front)
//  glVertex3f( 1.0f, -1.0f,  1.0f);  // Point 2 (Front)
//  glVertex3f( 1.0f,  1.0f,  1.0f);  // Point 3 (Front)
//  glVertex3f(-1.0f,  1.0f,  1.0f);  // Point 4 (Front)
//  // Back Face
//  glNormal3f( 0.0f, 0.0f,-1.0f);    // Normal Pointing Away From Viewer
//  glVertex3f(-1.0f, -1.0f, -1.0f);  // Point 1 (Back)
//  glVertex3f(-1.0f,  1.0f, -1.0f);  // Point 2 (Back)
//  glVertex3f( 1.0f,  1.0f, -1.0f);  // Point 3 (Back)
//  glVertex3f( 1.0f, -1.0f, -1.0f);  // Point 4 (Back)
//  // Top Face
//  glNormal3f( 0.0f, 1.0f, 0.0f);    // Normal Pointing Up
//  glVertex3f(-1.0f,  1.0f, -1.0f);  // Point 1 (Top)
//  glVertex3f(-1.0f,  1.0f,  1.0f);  // Point 2 (Top)
//  glVertex3f( 1.0f,  1.0f,  1.0f);  // Point 3 (Top)
//  glVertex3f( 1.0f,  1.0f, -1.0f);  // Point 4 (Top)
//  // Bottom Face
//  glNormal3f( 0.0f,-1.0f, 0.0f);    // Normal Pointing Down
//  glVertex3f(-1.0f, -1.0f, -1.0f);  // Point 1 (Bottom)
//  glVertex3f( 1.0f, -1.0f, -1.0f);  // Point 2 (Bottom)
//  glVertex3f( 1.0f, -1.0f,  1.0f);  // Point 3 (Bottom)
//  glVertex3f(-1.0f, -1.0f,  1.0f);  // Point 4 (Bottom)
//  // Right face
//  glNormal3f( 1.0f, 0.0f, 0.0f);    // Normal Pointing Right
//  glVertex3f( 1.0f, -1.0f, -1.0f);  // Point 1 (Right)
//  glVertex3f( 1.0f,  1.0f, -1.0f);  // Point 2 (Right)
//  glVertex3f( 1.0f,  1.0f,  1.0f);  // Point 3 (Right)
//  glVertex3f( 1.0f, -1.0f,  1.0f);  // Point 4 (Right)
//  // Left Face
//  glNormal3f(-1.0f, 0.0f, 0.0f);    // Normal Pointing Left
//  glVertex3f(-1.0f, -1.0f, -1.0f);  // Point 1 (Left)
//  glVertex3f(-1.0f, -1.0f,  1.0f);  // Point 2 (Left)
//  glVertex3f(-1.0f,  1.0f,  1.0f);  // Point 3 (Left)
//  glVertex3f(-1.0f,  1.0f, -1.0f);  // Point 4 (Left)
//  glEnd();

//  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//  glColor4f(0.2f,0.65f,0.3f,0.35f); // Full Brightness, 50% Alpha ( NEW )
//  glBegin(GL_QUADS);
//  // Front Face
//  glNormal3f( 0.0f, 0.0f, 1.0f);    // Normal Pointing Towards Viewer
//  glVertex3f(-1.0f, -1.0f,  1.0f);  // Point 1 (Front)
//  glVertex3f( 1.0f, -1.0f,  1.0f);  // Point 2 (Front)
//  glVertex3f( 1.0f,  1.0f,  1.0f);  // Point 3 (Front)
//  glVertex3f(-1.0f,  1.0f,  1.0f);  // Point 4 (Front)
//  // Back Face
//  glNormal3f( 0.0f, 0.0f,-1.0f);    // Normal Pointing Away From Viewer
//  glVertex3f(-1.0f, -1.0f, -1.0f);  // Point 1 (Back)
//  glVertex3f(-1.0f,  1.0f, -1.0f);  // Point 2 (Back)
//  glVertex3f( 1.0f,  1.0f, -1.0f);  // Point 3 (Back)
//  glVertex3f( 1.0f, -1.0f, -1.0f);  // Point 4 (Back)
//  // Top Face
//  glNormal3f( 0.0f, 1.0f, 0.0f);    // Normal Pointing Up
//  glVertex3f(-1.0f,  1.0f, -1.0f);  // Point 1 (Top)
//  glVertex3f(-1.0f,  1.0f,  1.0f);  // Point 2 (Top)
//  glVertex3f( 1.0f,  1.0f,  1.0f);  // Point 3 (Top)
//  glVertex3f( 1.0f,  1.0f, -1.0f);  // Point 4 (Top)
//  // Bottom Face
//  glNormal3f( 0.0f,-1.0f, 0.0f);    // Normal Pointing Down
//  glVertex3f(-1.0f, -1.0f, -1.0f);  // Point 1 (Bottom)
//  glVertex3f( 1.0f, -1.0f, -1.0f);  // Point 2 (Bottom)
//  glVertex3f( 1.0f, -1.0f,  1.0f);  // Point 3 (Bottom)
//  glVertex3f(-1.0f, -1.0f,  1.0f);  // Point 4 (Bottom)
//  // Right face
//  glNormal3f( 1.0f, 0.0f, 0.0f);    // Normal Pointing Right
//  glVertex3f( 1.0f, -1.0f, -1.0f);  // Point 1 (Right)
//  glVertex3f( 1.0f,  1.0f, -1.0f);  // Point 2 (Right)
//  glVertex3f( 1.0f,  1.0f,  1.0f);  // Point 3 (Right)
//  glVertex3f( 1.0f, -1.0f,  1.0f);  // Point 4 (Right)
//  // Left Face
//  glNormal3f(-1.0f, 0.0f, 0.0f);    // Normal Pointing Left
//  glVertex3f(-1.0f, -1.0f, -1.0f);  // Point 1 (Left)
//  glVertex3f(-1.0f, -1.0f,  1.0f);  // Point 2 (Left)
//  glVertex3f(-1.0f,  1.0f,  1.0f);  // Point 3 (Left)
//  glVertex3f(-1.0f,  1.0f, -1.0f);  // Point 4 (Left)
//  glEnd();

//  glPopAttrib();
//}
