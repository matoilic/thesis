include (../../QMake/Application.pri)

QT += core

SOURCES += main.cpp \
           LSWMS.cpp \
           LineSegment.cpp \
           LinePoint.cpp \
           DoorDetector.cpp

LIBS += \
    -lopencv_core \
    -lopencv_highgui \
    -lopencv_imgproc \
    -lopencv_legacy \
    -lopencv_photo \

HEADERS += \
    LSWMS.h \
    LineSegment.h \
    LinePoint.h \
    SegmentDistance.h \
    DoorDetector.h
