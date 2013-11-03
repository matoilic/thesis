include(../Component.pri)
include(../../Libraries/opencv/opencv_includepath.pri)

INCLUDEPATH += \
    ../../Libraries/glfw/include

LIBS += \
    -lopencv_core \
    -lopencv_highgui \
    -lglfw

SOURCES += \
    camerawindow.cpp

HEADERS += \
    camerawindow.h

macx {
    LIBS += \
        -framework OpenGL
}
