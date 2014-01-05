include(../../QMake/Application.pri)

QT += core

SOURCES += main.cpp

LIBS += \
    -lopencv_core \
    -lopencv_highgui \
    -lopencv_imgproc \
    -lArdoor

HEADERS +=
