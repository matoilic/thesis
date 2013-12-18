include (../../QMake/Application.pri)

INCLUDEPATH += \
    ../../Libraries/glfw/include \
    ../../Libraries/glfw/deps \
    ../../Libraries/glew/include

LIBS += \
    -lArdoor \
    -lopencv_core \
    -lopencv_highgui \
    -lglfw

macx {
    LIBS += -framework OpenGL
}

win32 {
    LIBS += -lopengl32
}

SOURCES += \
    main.cpp

