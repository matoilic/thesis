#include <Ardoor/Capture/Capture.hpp>
#include <Ardoor/Rendering/ArRenderingContext.hpp>
#include <Ardoor/Rendering/OpenGLWindow.hpp>
#include <QApplication>
#include <QMessageBox>
#include "CalibrationProcessor.hpp"

using namespace std;

int main(int argc, char** argv)
{
    ArdoorContext ardoorContext;
    ArRenderingContext renderingContext(&ardoorContext);

    QApplication app(argc, argv);

    OpenGLWindow window(&renderingContext);
    window.resize(800, 600);
    window.show();
    window.setAnimating(true);

    QMessageBox msgBox;
    msgBox.setText("Your camera has been calibrated.");

    CalibrationProcessor calibrationProcessor(&ardoorContext, &renderingContext, &window);
    calibrationProcessor.start();

    QObject::connect(&calibrationProcessor, SIGNAL(finished()), &msgBox, SLOT(show()));
    QObject::connect(&msgBox, SIGNAL(finished(int)), &window, SLOT(close()));

    return app.exec();
}

