include(../Application.pri)
include(../../Libraries/opencv/opencv_includepath.pri)

INCLUDEPATH += \
    ../../Components/Capture \
    ../../Libraries/glfw/include

LIBS += \
    -lCameraCalib \
    -lCapture \
    -lopencv_core \
    -lopencv_highgui \
    -lglfw \

macx {
    LIBS += \
        -framework OpenGL
}

SOURCES += \
    main.cpp

