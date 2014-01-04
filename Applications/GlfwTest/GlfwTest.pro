include(../../QMake/Application.pri)
include(../../QMake/Opencv.pri)

INCLUDEPATH += \
    ../../Libraries/glfw/include/ \
    ../../Libraries/glfw/deps \
    ../../Libraries/glew/include

LIBS += \
    -lopencv_core \
    -lopencv_calib3d \
    -lopencv_imgproc \
    -lopencv_highgui \
    -lglfw \
    -lglew \
    -framework OpenGL \

SOURCES += \
    main.cpp \
