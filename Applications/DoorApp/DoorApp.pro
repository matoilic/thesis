include(../../QMake/Application.pri)

QT += core opengl

LIBS += \
    -lArdoor \
    -lopencv_core \
    -lopencv_calib3d \
    -lopencv_imgproc \
    -lopencv_highgui

win32 {
    LIBS += -lopengl32
}

macx {
    LIBS += -framework OpenGL
    LIBS += -framework Cocoa
}

SOURCES += \
    main.cpp
