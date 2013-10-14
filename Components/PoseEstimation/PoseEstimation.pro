include(../Component.pri)
include(../../Libraries/opencv/opencv_includepath.pri)

LIBS += \
    -lCameraCalib \
    -lopencv_calib3d \
    -lopencv_core

INCLUDEPATH += \
    ../CameraCalib/ \

SOURCES += \
    poseestimator.cpp \
    chessboardposeestimator.cpp

HEADERS += \
    poseestimator.h \
    chessboardposeestimator.h
