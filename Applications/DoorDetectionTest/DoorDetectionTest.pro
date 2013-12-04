include(../Application.pri)
include(../../Libraries/opencv/opencv_includepath.pri)


INCLUDEPATH += \
    ../../Components/LineDetection \

LIBS += \
    -lLineDetection \
    -lopencv_core \
    -lopencv_highgui \
    -lopencv_imgproc \

SOURCES += \
    main.cpp

HEADERS +=

