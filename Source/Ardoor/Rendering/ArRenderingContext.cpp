#include "ArRenderingContext.hpp"


ArRenderingContext::ArRenderingContext(ArdoorContext *ardoorContext)
    : ardoorContext(ardoorContext)
{
}

ArRenderingContext::~ArRenderingContext()
{
    delete backgroundTexture;
    delete textureShader;
}

ArdoorContext* ArRenderingContext::getArdoorContext()
{
    return ardoorContext;
}

void ArRenderingContext::update(const cv::Mat &image, const PoseEstimationResult &result)
{
    mutex.lock();

    poseResult = result;
    image.copyTo(backgroundImage);

    mutex.unlock();
}

void ArRenderingContext::initialize()
{
    qDebug() << "ArRenderingContext initialize...";

    initializeOpenGLFunctions();

    glClearColor(0, 0, 0, 1);
    //glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    backgroundTexture = new TexturedRectangleGeometry();

    // Load, compile & link shaders
    textureShader = new QOpenGLShaderProgram();
    textureShader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/ardoor/shaders/TextureOnly.vert");
    textureShader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/ardoor/shaders/TextureOnly.frag");
    textureShader->link();

}

void ArRenderingContext::resize(int width, int height)
{
    qDebug() << "ArRenderingContext resize(" << width << ", " << height << ")";

    glViewport(0, 0, width, height);
    initializePerspective(width, height);
}

void ArRenderingContext::display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    modelViewMatrix.setToIdentity();

    drawCameraFrame();

    if (poseResult.isObjectPresent) {
        // Set the extrinsic transformation
        modelViewMatrix = poseResult.mvMatrix * modelViewMatrix;

        // The solvePnP results with the floating point ratio were inaccurate.
        // Scaling the objectPoints improved results. We have to scale everything in OpenGL now
        // or the rendering will be wrong.
        modelViewMatrix.scale(ARD_POSEESTIMATION_SCALE_FACTOR);

        // Draw augmented scene
        drawAugmentedScene();
    }
}

void ArRenderingContext::initializePerspective(int width, int height)
{
    projectionMatrix.setToIdentity();

    float zNear = 0.1f;
    float zFar = 1000.0f;

    CameraConfiguration cam = getArdoorContext()->getCameraConfiguration(width, height);
    float fx = cam.getFocalLengthX();
    float fy = cam.getFocalLengthY();
    float cx = cam.getPrimaryPointX();
    float cy = cam.getPrimaryPointY();
    float x0 = cx - width/2;
    float y0 = cy - height/2;

    QMatrix4x4 proj;
    proj.setToIdentity();
    proj(0, 0) = fx;
    proj(0, 2) = -x0;
    proj(1, 1) = fy;
    proj(1, 2) = -y0;
    proj(2, 2) = zNear + zFar;
    proj(2, 3) = zNear * zFar;
    proj(3, 2) = -1.0f;

    projectionMatrix.ortho(-width/2, width/2, -height/2, height/2, zNear, zFar);
    projectionMatrix *= proj;
}

void ArRenderingContext::drawCameraFrame()
{
    if (backgroundImage.empty())
        return;

    backgroundTexture->update(backgroundImage);

    // Backup current matrix values
    QMatrix4x4 projectionBackup = projectionMatrix;
    QMatrix4x4 modelViewBackup = modelViewMatrix;

    // Begin othogonal projection
    projectionMatrix.setToIdentity();
    projectionMatrix.ortho(-1, 1, -1, 1, -1, 1000);
    modelViewMatrix.setToIdentity();
    modelViewMatrix.translate(0, 0, -1000);

    // Activate the shader programm
    textureShader->bind();

    // Pass the mvp matrix
    textureShader->setUniformValue("u_mvpMatrix", projectionMatrix * modelViewMatrix);

    // draw geometry
    backgroundTexture->draw(textureShader);
    textureShader->release();

    // Restore previous matrix values
    projectionMatrix = projectionBackup;
    modelViewMatrix = modelViewBackup;
}
