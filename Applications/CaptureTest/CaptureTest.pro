include(../Application.pri)
include(../../Libraries/opencv/opencv_includepath.pri)

INCLUDEPATH += \
    ../../Components/Capture \
    ../../Libraries/glfw/include \
    ../../Libraries/glfw/deps \
    ../../Libraries/glew/include

LIBS += \
    -lCameraCalib \
    -lCapture \
    -lopencv_core \
    -lopencv_highgui \
    -lglfw \

macx {
    LIBS += -framework OpenGL
}

win32 {
    LIBS += -lopengl32
}

SOURCES += \
    main.cpp

