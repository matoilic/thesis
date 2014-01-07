#include <Ardoor/Rendering/DoorRenderingContext.hpp>

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

    // Load, compile & link shaders
    simpleShader = new QOpenGLShaderProgram();
    simpleShader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/ardoor/shaders/SimpleColor.vert");
    simpleShader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/ardoor/shaders/SimpleColor.frag");
    simpleShader->link();
}


void DoorRenderingContext::drawAugmentedScene()
{
    modelViewMatrix.translate(poseResult.width / 2.0f, poseResult.height);
    modelViewMatrix.scale(0.6, 0.05, 0.3);
    modelViewMatrix.translate(0, 1, -1);

    drawModel();
}

void DoorRenderingContext::drawModel()
{
    // Activate the shader programm
    simpleShader->bind();

    // Pass the mvp matrix
    simpleShader->setUniformValue("u_mvpMatrix", projectionMatrix * modelViewMatrix);

    // draw geometry
    cube->draw(simpleShader);
    simpleShader->release();
}
