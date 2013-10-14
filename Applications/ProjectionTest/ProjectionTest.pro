include(../Application.pri)
include(../../Libraries/opencv/opencv_includepath.pri)

INCLUDEPATH += \
    ../../Components/CameraCalib \
    ../../Components/Capture \
    ../../Components/PoseEstimation \


LIBS += \
    -lCameraCalib \
    -lCapture \
    -lPoseEstimation \
    -lopencv_core \
    -lopencv_calib3d \
    -lopencv_imgproc \
    -lopencv_highgui \
    -lopengl32

SOURCES += main.cpp \
    rendering.cpp \
    ar.cpp

HEADERS += \
    rendering.hpp \
    ar.hpp

