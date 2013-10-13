#include <iostream>
#include "ar.hpp"

using namespace std;


cv::Size boardSize(9, 6);
CameraCalib calib(boardSize);
CameraConfiguration camera(526.58037684199849f, 524.65577209994706f, 318.41744018680112f, 202.96659047014398f);
ChessboardPoseEstimator poseEstimator(calib, camera);
Capture capture;

cv::Mat backgroundImage;
PoseEstimationResult poseResult;
cv::Matx44f projectionMatrix;

bool isTextureInitialized = false;
unsigned int textureId;


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
    capture.setHandler(processFrame);
    capture.start();
}

void processFrame(cv::Mat frame)
{
    cv::Mat grayscale;

    cv::flip(frame, backgroundImage, 1);
    cv::cvtColor(backgroundImage, grayscale, CV_BGR2GRAY);

    poseResult = poseEstimator.estimatePose(grayscale);

    resizeWindow(backgroundImage.size().width, backgroundImage.size().height);
    redisplay();
}

void initializePerspective()
{
    //cv::Matx44f projectionMatrixTransposed = projectionMatrix.t();
    //glLoadMatrixf(reinterpret_cast<const GLfloat*>(&projectionMatrixTransposed.val));

    if (!backgroundImage.empty()) {
        double fovx, fovy, focalLength, aspectRatio;
        cv::Point2d principalPoint;
        cv::calibrationMatrixValues(camera.getIntrinsics(), backgroundImage.size(), 0.0, 0.0, fovx, fovy, focalLength, principalPoint, aspectRatio);
        gluPerspective(fovy, 1.0/aspectRatio, 0.01, 1000.0);
    }
}

void drawScene()
{
    drawCameraFrame();
    drawAugmentedScene();
}

void drawAugmentedScene()
{
    if (poseResult.isObjectPresent)
    {
        // Set the pattern transformation
        cv::Matx44f transposed = poseResult.mvMatrix.t();
        glLoadMatrixf(reinterpret_cast<const GLfloat*>(&transposed.val[0]));

        // Render model
        //glRotatef(-90, -1.0, 0.0, 0.0);
        drawCubeModel();
        drawCoordinateAxis();
    }
}

void drawCameraFrame()
{
    if (backgroundImage.empty())
        return;

    // Initialize texture for background image
    //if (!isTextureInitialized)
    //{
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        isTextureInitialized = true;
    //}

    int w = backgroundImage.cols;
    int h = backgroundImage.rows;

    glPixelStorei(GL_PACK_ALIGNMENT, 3);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 3);
    glBindTexture(GL_TEXTURE_2D, textureId);

    // Upload new texture data:
    if (backgroundImage.channels() == 3)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, backgroundImage.data);
    else if(backgroundImage.channels() == 4)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, backgroundImage.data);
    else if (backgroundImage.channels()==1)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, backgroundImage.data);

    const GLfloat bgTextureVertices[] = { 0, 0, w, 0, 0, h, w, h };
    const GLfloat bgTextureCoords[]   = { 0, 1, 1, 1, 0, 0, 1, 0 };

    //begin othogonal projection
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0, w, 0.0, h, 0.01, 1000.0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -1000.0);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureId);

    // Update attribute values.
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(2, GL_FLOAT, 0, bgTextureVertices);
    glTexCoordPointer(2, GL_FLOAT, 0, bgTextureCoords);

    glColor4f(1,1,1,1);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_TEXTURE_2D);

    //end orthogonal projection
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void drawCoordinateAxis()
{
  static float lineX[] = {0,0,0,1,0,0};
  static float lineY[] = {0,0,0,0,1,0};
  static float lineZ[] = {0,0,0,0,0,1};

  glLineWidth(2);

  glBegin(GL_LINES);

  glColor3f(1.0f, 0.0f, 0.0f);
  glVertex3fv(lineX);
  glVertex3fv(lineX + 3);

  glColor3f(0.0f, 1.0f, 0.0f);
  glVertex3fv(lineY);
  glVertex3fv(lineY + 3);

  glColor3f(0.0f, 0.0f, 1.0f);
  glVertex3fv(lineZ);
  glVertex3fv(lineZ + 3);

  glEnd();
}


void drawCubeModel()
{
  static const GLfloat LightAmbient[]=  { 0.25f, 0.25f, 0.25f, 1.0f };    // Ambient Light Values
  static const GLfloat LightDiffuse[]=  { 0.1f, 0.1f, 0.1f, 1.0f };    // Diffuse Light Values
  static const GLfloat LightPosition[]= { 0.0f, 0.0f, 2.0f, 1.0f };    // Light Position

  glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT | GL_LIGHTING_BIT | GL_POLYGON_BIT);

  glColor4f(0.2f,0.35f,0.3f,0.75f);         // Full Brightness, 50% Alpha ( NEW )
  glBlendFunc(GL_ONE,GL_ONE_MINUS_SRC_ALPHA);       // Blending Function For Translucency Based On Source Alpha
  glEnable(GL_BLEND);

  glShadeModel(GL_SMOOTH);

  glEnable(GL_LIGHTING);
  glDisable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
  glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
  glEnable(GL_COLOR_MATERIAL);

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glBegin(GL_QUADS);
  // Front Face
  glNormal3f( 0.0f, 0.0f, 1.0f);    // Normal Pointing Towards Viewer
  glVertex3f(-1.0f, -1.0f,  1.0f);  // Point 1 (Front)
  glVertex3f( 1.0f, -1.0f,  1.0f);  // Point 2 (Front)
  glVertex3f( 1.0f,  1.0f,  1.0f);  // Point 3 (Front)
  glVertex3f(-1.0f,  1.0f,  1.0f);  // Point 4 (Front)
  // Back Face
  glNormal3f( 0.0f, 0.0f,-1.0f);    // Normal Pointing Away From Viewer
  glVertex3f(-1.0f, -1.0f, -1.0f);  // Point 1 (Back)
  glVertex3f(-1.0f,  1.0f, -1.0f);  // Point 2 (Back)
  glVertex3f( 1.0f,  1.0f, -1.0f);  // Point 3 (Back)
  glVertex3f( 1.0f, -1.0f, -1.0f);  // Point 4 (Back)
  // Top Face
  glNormal3f( 0.0f, 1.0f, 0.0f);    // Normal Pointing Up
  glVertex3f(-1.0f,  1.0f, -1.0f);  // Point 1 (Top)
  glVertex3f(-1.0f,  1.0f,  1.0f);  // Point 2 (Top)
  glVertex3f( 1.0f,  1.0f,  1.0f);  // Point 3 (Top)
  glVertex3f( 1.0f,  1.0f, -1.0f);  // Point 4 (Top)
  // Bottom Face
  glNormal3f( 0.0f,-1.0f, 0.0f);    // Normal Pointing Down
  glVertex3f(-1.0f, -1.0f, -1.0f);  // Point 1 (Bottom)
  glVertex3f( 1.0f, -1.0f, -1.0f);  // Point 2 (Bottom)
  glVertex3f( 1.0f, -1.0f,  1.0f);  // Point 3 (Bottom)
  glVertex3f(-1.0f, -1.0f,  1.0f);  // Point 4 (Bottom)
  // Right face
  glNormal3f( 1.0f, 0.0f, 0.0f);    // Normal Pointing Right
  glVertex3f( 1.0f, -1.0f, -1.0f);  // Point 1 (Right)
  glVertex3f( 1.0f,  1.0f, -1.0f);  // Point 2 (Right)
  glVertex3f( 1.0f,  1.0f,  1.0f);  // Point 3 (Right)
  glVertex3f( 1.0f, -1.0f,  1.0f);  // Point 4 (Right)
  // Left Face
  glNormal3f(-1.0f, 0.0f, 0.0f);    // Normal Pointing Left
  glVertex3f(-1.0f, -1.0f, -1.0f);  // Point 1 (Left)
  glVertex3f(-1.0f, -1.0f,  1.0f);  // Point 2 (Left)
  glVertex3f(-1.0f,  1.0f,  1.0f);  // Point 3 (Left)
  glVertex3f(-1.0f,  1.0f, -1.0f);  // Point 4 (Left)
  glEnd();

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glColor4f(0.2f,0.65f,0.3f,0.35f); // Full Brightness, 50% Alpha ( NEW )
  glBegin(GL_QUADS);
  // Front Face
  glNormal3f( 0.0f, 0.0f, 1.0f);    // Normal Pointing Towards Viewer
  glVertex3f(-1.0f, -1.0f,  1.0f);  // Point 1 (Front)
  glVertex3f( 1.0f, -1.0f,  1.0f);  // Point 2 (Front)
  glVertex3f( 1.0f,  1.0f,  1.0f);  // Point 3 (Front)
  glVertex3f(-1.0f,  1.0f,  1.0f);  // Point 4 (Front)
  // Back Face
  glNormal3f( 0.0f, 0.0f,-1.0f);    // Normal Pointing Away From Viewer
  glVertex3f(-1.0f, -1.0f, -1.0f);  // Point 1 (Back)
  glVertex3f(-1.0f,  1.0f, -1.0f);  // Point 2 (Back)
  glVertex3f( 1.0f,  1.0f, -1.0f);  // Point 3 (Back)
  glVertex3f( 1.0f, -1.0f, -1.0f);  // Point 4 (Back)
  // Top Face
  glNormal3f( 0.0f, 1.0f, 0.0f);    // Normal Pointing Up
  glVertex3f(-1.0f,  1.0f, -1.0f);  // Point 1 (Top)
  glVertex3f(-1.0f,  1.0f,  1.0f);  // Point 2 (Top)
  glVertex3f( 1.0f,  1.0f,  1.0f);  // Point 3 (Top)
  glVertex3f( 1.0f,  1.0f, -1.0f);  // Point 4 (Top)
  // Bottom Face
  glNormal3f( 0.0f,-1.0f, 0.0f);    // Normal Pointing Down
  glVertex3f(-1.0f, -1.0f, -1.0f);  // Point 1 (Bottom)
  glVertex3f( 1.0f, -1.0f, -1.0f);  // Point 2 (Bottom)
  glVertex3f( 1.0f, -1.0f,  1.0f);  // Point 3 (Bottom)
  glVertex3f(-1.0f, -1.0f,  1.0f);  // Point 4 (Bottom)
  // Right face
  glNormal3f( 1.0f, 0.0f, 0.0f);    // Normal Pointing Right
  glVertex3f( 1.0f, -1.0f, -1.0f);  // Point 1 (Right)
  glVertex3f( 1.0f,  1.0f, -1.0f);  // Point 2 (Right)
  glVertex3f( 1.0f,  1.0f,  1.0f);  // Point 3 (Right)
  glVertex3f( 1.0f, -1.0f,  1.0f);  // Point 4 (Right)
  // Left Face
  glNormal3f(-1.0f, 0.0f, 0.0f);    // Normal Pointing Left
  glVertex3f(-1.0f, -1.0f, -1.0f);  // Point 1 (Left)
  glVertex3f(-1.0f, -1.0f,  1.0f);  // Point 2 (Left)
  glVertex3f(-1.0f,  1.0f,  1.0f);  // Point 3 (Left)
  glVertex3f(-1.0f,  1.0f, -1.0f);  // Point 4 (Left)
  glEnd();

  glPopAttrib();
}
