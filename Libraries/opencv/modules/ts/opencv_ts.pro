include (../../opencv.pri)

TEMPLATE = lib
CONFIG += shared dll

DEFINES -= UNICODE
LIBS += \
    -lopencv_core \
    -lopencv_imgproc \
    -lopencv_features2d \
    -lopencv_flann \
    -lopencv_highgui

HEADERS += \
    include/opencv2/ts/ts_perf.hpp \
    include/opencv2/ts/ts.hpp \
    include/opencv2/ts/gpu_test.hpp \
    include/opencv2/ts/gpu_perf.hpp \
    include/opencv2/ts/ts_gtest.h

PRECOMPILED_HEADER += \
    src/precomp.hpp

SOURCES += \
    src/ts_perf.cpp \
    src/ts_gtest.cpp \
    src/ts_func.cpp \
    src/ts_arrtest.cpp \
    src/ts.cpp \
    src/precomp.cpp \
    src/gpu_test.cpp \
    src/gpu_perf.cpp

