#ifndef CHESSBOARDRENDERINGCONTEXT_HPP
#define CHESSBOARDRENDERINGCONTEXT_HPP

#include <opencv2/core/core.hpp>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>

#include <Ardoor/Context/ArdoorContext.hpp>
#include <Ardoor/Rendering/ArRenderingContext.hpp>
#include <Ardoor/Rendering/CubeGeometry.hpp>
#include <Ardoor/Rendering/CoordinateAxisGeometry.hpp>

class ARD_EXPORT ChessboardRenderingContext : public ArRenderingContext
{
public:
    ChessboardRenderingContext(ArdoorContext *ardoorContext) : ArRenderingContext(ardoorContext) {}
    ~ChessboardRenderingContext();

    virtual void initialize();

protected:
    virtual void drawAugmentedScene();
    void drawCoordinateAxis();
    void drawCubeModel();

private:
    CubeGeometry *cube;
    CoordinateAxisGeometry *coordinateAxes;
    QOpenGLShaderProgram *simpleShader;
};

#endif // CHESSBOARDRENDERINGCONTEXT_HPP
