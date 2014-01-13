#ifndef CHESSBOARDRENDERINGCONTEXT_HPP
#define CHESSBOARDRENDERINGCONTEXT_HPP

#include <opencv2/core/core.hpp>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>

#include <Ardoor/Context/ArdoorContext.hpp>
#include <Ardoor/Rendering/ArRenderingContext.hpp>
#include <Ardoor/Rendering/CubeGeometry.hpp>
#include <Ardoor/Rendering/CoordinateAxisGeometry.hpp>

class ARD_EXPORT DoorRenderingContext : public ArRenderingContext
{
public:
    DoorRenderingContext(ArdoorContext *ardoorContext) : ArRenderingContext(ardoorContext) {}
    ~DoorRenderingContext();

    virtual void initialize();

protected:
    virtual void drawAugmentedScene();
    void drawModel();
    void drawCoordinateAxis();

private:
    CubeGeometry *cube;
    CoordinateAxisGeometry *coordinateAxes;
    QOpenGLShaderProgram *simpleShader;
};

#endif // CHESSBOARDRENDERINGCONTEXT_HPP
