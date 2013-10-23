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

win32 {
    LIBS += -lopengl32
    LIBS += -lglut32
}

macx {
    LIBS += -framework OpenGL
    LIBS += -framework GLUT
    LIBS += -framework Cocoa
}

SOURCES += \
    rendering.cpp \
    ar.cpp \
    main.cpp

HEADERS += \
    rendering.hpp \
    ar.hpp

