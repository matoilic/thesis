include(../Application.pri)
include(../../Libraries/opencv/opencv_includepath.pri)

INCLUDEPATH += ../../Components/CameraCalib

LIBS += \
    -lCameraCalib \
    -lopencv_core \
    -lopencv_highgui \

SOURCES += main.cpp

