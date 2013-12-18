include (../../QMake/Application.pri)

INCLUDEPATH += \
    ../../Components/LineDetection \

LIBS += \
    -lArdoor \
    -lopencv_core \
    -lopencv_highgui \
    -lopencv_imgproc

SOURCES += \
    main.cpp

HEADERS +=

