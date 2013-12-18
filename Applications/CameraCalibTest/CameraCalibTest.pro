include (../../QMake/Application.pri)

LIBS += \
    -lArdoor \
    -lopencv_core \
    -lopencv_highgui

SOURCES += \
    main.cpp

