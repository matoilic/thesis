include(../Application.pri)
include(../../Libraries/opencv/opencv_includepath.pri)

INCLUDEPATH += \
    ../../Components/CameraWindow \
    ../../Libraries/glfw/include


LIBS += \
    -lCameraWindow \
    -lopencv_core \
    -lopencv_highgui \
    -lglfw

SOURCES += \
    main.cpp

macx {
    LIBS += -framework OpenGL
}
