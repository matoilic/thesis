#include <QCoreApplication>
#include "ArdoorContext.hpp"

ArdoorContext::ArdoorContext()
{
    QCoreApplication::setOrganizationName("BFH");
    QCoreApplication::setOrganizationDomain("bfh.ch");
    QCoreApplication::setApplicationName("Ardoor");

    m_settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "BFH", "Ardoor");
    loadSettings();
}

ArdoorContext::~ArdoorContext()
{
    delete m_settings;
}

CameraConfiguration ArdoorContext::getCameraConfiguration(int width, int height)
{
    return m_camera.scale(width, height);
}

void ArdoorContext::loadSettings()
{
    m_settings->sync();

    m_frontCamera = m_settings->value("camera/front", true).toBool();
    m_drawCoordinateAxes = m_settings->value("rendering/coordinateAxes", true).toBool();

    int boardWidth = m_settings->value("board/width", 9).toInt();
    int boardHeight = m_settings->value("board/height", 6).toInt();
    m_boardSize = cv::Size(boardWidth, boardHeight);

    m_settings->beginGroup("camera/intrinsics");
    float fx = m_settings->value("fx", 0).toFloat();
    float fy = m_settings->value("fy", 0).toFloat();
    float cx = m_settings->value("cx", 0).toFloat();
    float cy = m_settings->value("cy", 0).toFloat();
    int width = m_settings->value("width", 0).toInt();
    int height = m_settings->value("height", 0).toInt();
    m_settings->endGroup();

    std::vector<float> distCoeffs;
    int size = m_settings->beginReadArray("camera/distCoeffs");
    for (int i = 0; i < size; ++i) {
        m_settings->setArrayIndex(i);
        distCoeffs.push_back(m_settings->value("number").toFloat());
    }
    m_settings->endArray();

    m_camera.update(fx, fy, cx, cy, width, height, distCoeffs);
}

void ArdoorContext::saveSettings()
{
    m_settings->setValue("camera/front", m_frontCamera);
    m_settings->setValue("rendering/coordinateAxes", m_drawCoordinateAxes);

    m_settings->setValue("board/width", m_boardSize.width);
    m_settings->setValue("board/height", m_boardSize.height);

    m_settings->beginGroup("camera/intrinsics");
    m_settings->setValue("fx", m_camera.getFocalLengthX());
    m_settings->setValue("fy", m_camera.getFocalLengthY());
    m_settings->setValue("cx", m_camera.getPrimaryPointX());
    m_settings->setValue("cy", m_camera.getPrimaryPointY());
    m_settings->setValue("width", m_camera.getCalibrationWidth());
    m_settings->setValue("height", m_camera.getCalibrationHeight());
    m_settings->endGroup();

    std::vector<float> distCoeffs = m_camera.getDistorsion();
    m_settings->beginWriteArray("camera/distCoeffs");
    for (int i = 0; i < distCoeffs.size(); i++) {
        m_settings->setArrayIndex(i);
        m_settings->setValue("number", distCoeffs.at(i));
    }
    m_settings->endArray();

    m_settings->sync();
}

