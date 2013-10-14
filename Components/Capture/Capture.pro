include(../Component.pri)
include(../../Libraries/opencv/opencv_includepath.pri)

LIBS += \
    -lopencv_core \
    -lopencv_highgui

HEADERS += \
    capture.h

SOURCES += \
    capture.cpp

