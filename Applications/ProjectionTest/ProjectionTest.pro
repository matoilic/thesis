include (../../QMake/Application.pri)

INCLUDEPATH += \
    ../../Libraries/glfw/include \
    ../../Libraries/glfw/deps \
    ../../Libraries/glew/include


LIBS += \
    -lArdoor \
    -lopencv_core \
    -lopencv_calib3d \
    -lopencv_imgproc \
    -lopencv_highgui \
    -lglfw \
    -lglew

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
    main.cpp \
    glUtils.cpp

HEADERS += \
    rendering.hpp \
    ar.hpp \
    glUtils.hpp
