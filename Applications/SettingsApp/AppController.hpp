#ifndef APPCONTROLLER_HPP
#define APPCONTROLLER_HPP

#include <QObject>
#include <Ardoor/Context/ArdoorContext.hpp>

class CalibrationProcessor;

class AppController : public QObject
{
    Q_OBJECT

public:
    Q_PROPERTY(bool flipBackground READ flipBackground WRITE setFlipBackground)
    Q_PROPERTY(int boardWidth READ boardWidth WRITE setBoardWidth)
    Q_PROPERTY(int boardHeight READ boardHeight WRITE setBoardHeight)

    explicit AppController(QObject *parent = 0);
    int execute(int argc, char **argv);

    Q_INVOKABLE void saveSettings();
    Q_INVOKABLE void loadSettings();

    bool flipBackground() { return m_flipBackground; }
    void setFlipBackground(bool value) { m_flipBackground = value; }

    int boardWidth() { return m_boardWidth; }
    void setBoardWidth(int value) { m_boardWidth = value; }

    int boardHeight() { return m_boardHeight; }
    void setBoardHeight(int value) { m_boardHeight = value; }

private:
    ArdoorContext ardoorContext;

    bool m_flipBackground;
    int m_boardWidth;
    int m_boardHeight;
};

#endif // APPCONTROLLER_HPP
