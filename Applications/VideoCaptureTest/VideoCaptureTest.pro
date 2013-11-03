include(../Application.pri)
include(../../Libraries/opencv/opencv_includepath.pri)

LIBS += \
    -lopencv_core \
    -lopencv_calib3d \
    -lopencv_imgproc \
    -lopencv_highgui \

SOURCES += main.cpp

