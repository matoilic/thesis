include(../Component.pri)
include(../../Libraries/opencv/opencv_includepath.pri)

LIBS += \
    -lopencv_core \
    -lopencv_calib3d \
    -lopencv_imgproc \
    -lopencv_highgui

SOURCES += \
    cameracalib.cpp \
    cameraconfiguration.cpp

HEADERS += \
    cameracalib.h \
    cameraconfiguration.h
