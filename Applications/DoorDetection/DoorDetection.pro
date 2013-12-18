include (../../QMake/Application.pri)

SOURCES += main.cpp

LIBS += \
    -lopencv_core \
    -lopencv_highgui \
    -lopencv_imgproc \
    -lopencv_legacy \
