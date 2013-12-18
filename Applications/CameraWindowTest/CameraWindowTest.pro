include (../../QMake/Application.pri)

INCLUDEPATH += \
    ../../Libraries/glfw/include


LIBS += \
    -lArdoor \
    -lopencv_core \
    -lopencv_highgui \
    -lglfw

SOURCES += \
    main.cpp

macx {
    LIBS += -framework OpenGL
}
