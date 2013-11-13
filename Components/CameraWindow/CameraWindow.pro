include(../Component.pri)
include(../../Libraries/opencv/opencv_includepath.pri)

TEMPLATE = lib

CONFIG += shared dll

INCLUDEPATH += \
    ../../Libraries/glfw/include \
    ../../Libraries/glfw/deps \
    ../../Libraries/glew/include

LIBS += \
    -lopencv_core \
    -lopencv_highgui \
    -lglfw \
    -lglew

SOURCES += \
    camerawindow.cpp \
    glUtils.cpp \
    matgen.cpp

HEADERS += \
    camerawindow.h \
    glUtils.hpp \
    matgen.h

macx {
    LIBS += -framework OpenGL
}

win32 {
    LIBS += -lopengl32
}
