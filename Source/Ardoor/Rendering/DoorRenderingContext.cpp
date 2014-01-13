#include <Ardoor/Rendering/DoorRenderingContext.hpp>
#include <Ardoor/PoseEstimation/DoorPoseEstimator.hpp>

DoorRenderingContext::~DoorRenderingContext()
{
    delete cube;
    delete simpleShader;
}

void DoorRenderingContext::initialize()
{
    ArRenderingContext::initialize();

    cube = new CubeGeometry();
    cube->setColor(QColor(255, 0, 0, 255));
    coordinateAxes = new CoordinateAxisGeometry();

    // Load, compile & link shaders
    simpleShader = new QOpenGLShaderProgram();
    simpleShader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/ardoor/shaders/SimpleColor.vert");
    simpleShader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/ardoor/shaders/SimpleColor.frag");
    simpleShader->link();
}


void DoorRenderingContext::drawAugmentedScene()
{
    drawCoordinateAxis();
    drawModel();
}

void DoorRenderingContext::drawModel()
{
    // The door pose is at the lower left corner of the door. To render
    // a canopy over the door, we have to translate!
    modelViewMatrix.translate(poseResult.width / 2.0f, poseResult.height);
    modelViewMatrix.scale(0.5, 0.05, 0.3);
    modelViewMatrix.translate(0, 1, -1);

    // Activate the shader programm
    simpleShader->bind();

    // Pass the mvp matrix
    simpleShader->setUniformValue("u_mvpMatrix", projectionMatrix * modelViewMatrix);

    // draw geometry
    cube->draw(simpleShader);
    simpleShader->release();
}

void DoorRenderingContext::drawCoordinateAxis()
{
    if (!getArdoorContext()->isDrawCoordinateAxes())
        return;

    QMatrix4x4 modelViewMatrixBackup = modelViewMatrix;
    modelViewMatrix.scale(poseResult.width, poseResult.height, poseResult.width);

    // Activate the shader programm
    simpleShader->bind();

    // Pass the mvp matrix
    simpleShader->setUniformValue("u_mvpMatrix", projectionMatrix * modelViewMatrix);

    // draw geometry
    coordinateAxes->draw(simpleShader);
    simpleShader->release();

    modelViewMatrix = modelViewMatrixBackup;
}
