#include "AppController.hpp"

#include <QGuiApplication>
#include <QtQml>
#include <QtQml/QQmlApplicationEngine>

AppController::AppController(QObject *parent)
    : QObject(parent)
{
    loadSettings();
}

int AppController::execute(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("controller", this);
    engine.load(QUrl("qrc:///ardoor/views/main.qml"));

    return app.exec();
}

void AppController::loadSettings()
{
    m_flipBackground = ardoorContext.isFrontCamera();
    m_boardWidth = ardoorContext.getChessboardSize().width;
    m_boardHeight = ardoorContext.getChessboardSize().height;
}

void AppController::saveSettings()
{
    ardoorContext.setFrontCamera(m_flipBackground);

    cv::Size size(m_boardWidth, m_boardHeight);
    ardoorContext.setChessboardSize(size);

    ardoorContext.saveSettings();
}
