#include "ChessboardRenderingContext.hpp"

ChessboardRenderingContext::~ChessboardRenderingContext()
{
    delete cube;
    delete coordinateAxes;
    delete simpleShader;
}

void ChessboardRenderingContext::initialize()
{
    ArRenderingContext::initialize();

    cube = new CubeGeometry();
    cube->setColor(QColor(255, 255, 255, 255));
    coordinateAxes = new CoordinateAxisGeometry();

    // Load, compile & link shaders
    simpleShader = new QOpenGLShaderProgram();
    simpleShader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/ardoor/shaders/SimpleColor.vert");
    simpleShader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/ardoor/shaders/SimpleColor.frag");
    simpleShader->link();
}


void ChessboardRenderingContext::drawAugmentedScene()
{
    drawCoordinateAxis();
    drawCubeModel();
}

void ChessboardRenderingContext::drawCoordinateAxis()
{
    // Backup current matrix values
    QMatrix4x4 projectionBackup = projectionMatrix;
    QMatrix4x4 modelViewBackup = modelViewMatrix;

    // The central point of the chessboard is estimated. To display the coordinate
    // axes for the whole board, we have to scale and translate it.
    modelViewMatrix.translate(-poseResult.width / 2 - 1, -poseResult.height / 2 - 1);
    modelViewMatrix.scale(poseResult.width + 1, poseResult.height + 1, poseResult.height);

    // Activate the shader programm
    simpleShader->bind();

    // Pass the mvp matrix
    simpleShader->setUniformValue("u_mvpMatrix", projectionMatrix * modelViewMatrix);

    // draw geometry
    coordinateAxes->draw(simpleShader);
    simpleShader->release();

    // Restore previous matrix values
    projectionMatrix = projectionBackup;
    modelViewMatrix = modelViewBackup;
}


void ChessboardRenderingContext::drawCubeModel()
{
    // In the object coordinate system, the up-axis is the y-axis. On the chessboard, the z-axis
    // is the axis away from the chessboard (to the viewer). We want the object to "stand" on the chessboard
    // so we rotate the object, so that the both up-axes are aligned.
    modelViewMatrix.rotate(90, 1, 0, 0);

    // We create a 2x2 cube here around the (0, 0) point in the object coordinate system.
    // Because the chessboard is on the z=0 plane, we have to translate the cube so the lower bounds of the
    // objekt are on the z=0 plane.
    modelViewMatrix.translate(0, 1, 0);

    // Activate the shader programm
    simpleShader->bind();

    // Pass the mvp matrix
    simpleShader->setUniformValue("u_mvpMatrix", projectionMatrix * modelViewMatrix);

    // draw geometry
    cube->draw(simpleShader);
    simpleShader->release();
}
