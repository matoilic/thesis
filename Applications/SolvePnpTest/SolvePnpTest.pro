include(../../QMake/Application.pri)

LIBS += \
    -lArdoor \
    -lopencv_core \
    -lopencv_calib3d \

SOURCES += \
    main.cpp

HEADERS +=
