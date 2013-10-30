include(../Application.pri)
include(../../Libraries/opencv/opencv_includepath.pri)

INCLUDEPATH += \
    ../../Components/CameraCalib \
    ../../Components/Capture \
    ../../Components/PoseEstimation \
    ../../Libraries/glfw/include/ \


LIBS += \
    -lCameraCalib \
    -lCapture \
    -lPoseEstimation \
    -lopencv_core \
    -lopencv_calib3d \
    -lopencv_imgproc \
    -lopencv_highgui \
    -lglfw \

win32 {
    LIBS += -lopengl32
}

macx {
    LIBS += -framework OpenGL
    LIBS += -framework Cocoa
}

SOURCES += \
    rendering.cpp \
    ar.cpp \
    main.cpp

HEADERS += \
    rendering.hpp \
    ar.hpp

