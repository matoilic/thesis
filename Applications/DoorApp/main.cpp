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

    QGuiApplication app(argc, argv);

    OpenGLWindow window(&renderingContext);
    window.setTitle("Door-tracked augmented reality");
    window.resize(800, 600);
    window.show();
    window.setAnimating(true);

    FrameProcessor frameProcessor(&ardoorContext, &poseEstimator, &renderingContext, &window);

    Capture capture;
    capture.setHandler(&frameProcessor);
    capture.startImageFile("D:\\BFH\\Semester8\\thesis\\Data\\Images\\Doors\\webcam-toy-foto3.jpg");

    return app.exec();
}

