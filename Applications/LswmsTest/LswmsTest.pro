include(../Application.pri)
include(../../Libraries/opencv/opencv_includepath.pri)

QT += core

SOURCES += main.cpp \
           LSWMS.cpp

LIBS += \
    -lopencv_core \
    -lopencv_highgui \
    -lopencv_imgproc \
    -lopencv_legacy \

HEADERS += \
    LSWMS.h
