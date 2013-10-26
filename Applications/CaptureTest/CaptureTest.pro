include(../Application.pri)
include(../../Libraries/opencv/opencv_includepath.pri)

INCLUDEPATH += ../../Components/Capture

LIBS += \
    -lCameraCalib \
    -lCapture \
    -lopencv_core \
    -lopencv_highgui \

SOURCES += \
    main.cpp
