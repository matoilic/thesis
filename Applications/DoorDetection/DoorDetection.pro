include(../Application.pri)
include(../../Libraries/opencv/opencv_includepath.pri)


SOURCES += main.cpp

LIBS += \
    -lopencv_core \
    -lopencv_highgui \
    -lopencv_imgproc \
    -lopencv_legacy \