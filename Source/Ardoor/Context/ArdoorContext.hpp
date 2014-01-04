#ifndef ARDOORCONTEXT_HPP
#define ARDOORCONTEXT_HPP

#include <QSettings>
#include <Ardoor/Ardoor.hpp>
#include <Ardoor/CameraCalib/CameraConfiguration.hpp>


class ARD_EXPORT ArdoorContext
{
public:
    ArdoorContext();
    ~ArdoorContext();

    QSettings* getSettings() { return m_settings; }
    void loadSettings();
    void saveSettings();

    cv::Size getChessboardSize() { return m_boardSize; }
    void setChessboardSize(cv::Size size) { m_boardSize = size; }

    CameraConfiguration getCameraConfiguration() { return m_camera; }
    CameraConfiguration getCameraConfiguration(int width, int height);
    void setCameraConfiguration(const CameraConfiguration &camera) { m_camera = camera; }

    bool isFrontCamera() { return m_frontCamera; }
    void setFrontCamera(bool frontCamera) { m_frontCamera = frontCamera; }

private:
    QSettings *m_settings;
    cv::Size m_boardSize;
    CameraConfiguration m_camera;
    bool m_frontCamera;
};

#endif // ARDOORCONTEXT_HPP
