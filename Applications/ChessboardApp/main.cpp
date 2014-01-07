#include <Ardoor/Capture/Capture.hpp>
#include <Ardoor/Capture/FrameProcessor.hpp>
#include <Ardoor/PoseEstimation/ChessboardPoseEstimator.hpp>
#include <Ardoor/Rendering/ChessboardRenderingContext.hpp>
#include <Ardoor/Rendering/OpenGLWindow.hpp>
#include <QGuiApplication>

using namespace std;

int main(int argc, char** argv)
{
    ArdoorContext ardoorContext;
    ChessboardPoseEstimator poseEstimator(&ardoorContext);
    ChessboardRenderingContext renderingContext(&ardoorContext);

    QGuiApplication app(argc, argv);

    OpenGLWindow window(&renderingContext);
    window.resize(800, 600);
    window.show();
    window.setAnimating(true);

    FrameProcessor frameProcessor(&ardoorContext, &poseEstimator, &renderingContext, &window);

    Capture capture;
    capture.setHandler(&frameProcessor);
    capture.start();

    return app.exec();
}

