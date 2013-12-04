include(../Component.pri)
include(../../Libraries/opencv/opencv_includepath.pri)

LIBS += \
    -lopencv_core \
    -lopencv_imgproc \

HEADERS += \
    LineDetector.hpp \
    Types.hpp \
    PointQuadTree.hpp \
    lsd.h \
    LSDLineDetector.hpp

SOURCES += \
    lsd.c \
    LSDLineDetector.cpp


