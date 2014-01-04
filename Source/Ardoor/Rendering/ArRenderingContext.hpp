#ifndef ARRENDERINGCONTEXT_HPP
#define ARRENDERINGCONTEXT_HPP

#include <QMutex>
#include <QOpenGLFunctions>
#include <Ardoor/Context/ArdoorContext.hpp>
#include <Ardoor/Rendering/RenderingContext.hpp>
#include <Ardoor/Rendering/TexturedRectangleGeometry.hpp>
#include <Ardoor/PoseEstimation/Types.hpp>

class ARD_EXPORT ArRenderingContext : public RenderingContext, protected QOpenGLFunctions
{
public:
    ArRenderingContext(ArdoorContext *ardoorContext);
    ~ArRenderingContext();

    ArdoorContext *getArdoorContext();

    virtual void initialize();
    void resize(int width, int height);
    void display();
    void update(const cv::Mat &image, const PoseEstimationResult &result);

protected:
    void initializePerspective(int width, int height);
    void drawCameraFrame();
    virtual void drawAugmentedScene() {}

    PoseEstimationResult poseResult;

    QMatrix4x4 projectionMatrix;
    QMatrix4x4 modelViewMatrix;

private:
    ArdoorContext *ardoorContext;
    cv::Mat backgroundImage;
    QMutex mutex;
    TexturedRectangleGeometry *backgroundTexture;
    QOpenGLShaderProgram *textureShader;

};

#endif // ARRENDERINGCONTEXT_HPP
