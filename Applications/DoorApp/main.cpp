#include <Ardoor/Capture/Capture.hpp>
#include <Ardoor/Capture/FrameProcessor.hpp>
#include <Ardoor/PoseEstimation/DoorPoseEstimator.hpp>
#include <Ardoor/Rendering/DoorRenderingContext.hpp>
#include <Ardoor/Rendering/OpenGLWindow.hpp>
#include <QGuiApplication>

using namespace std;

int main(int argc, char** argv)
{
    ArdoorContext ardoorContext;
    DoorPoseEstimator poseEstimator(&ardoorContext);
    DoorRenderingContext renderingContext(&ardoorContext);

    FrameProcessor frameProcessor(&ardoorContext, &poseEstimator, &renderingContext);

    Capture capture;
    capture.setHandler(&frameProcessor);
    capture.start();

    QGuiApplication app(argc, argv);

    OpenGLWindow window(&renderingContext);
    window.setTitle("Door-tracked augmented reality");
    window.resize(800, 600);
    window.show();
    window.setAnimating(true);

    return app.exec();
}

