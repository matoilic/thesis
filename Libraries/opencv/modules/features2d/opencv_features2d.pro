include (../../opencv.pri)

TEMPLATE = lib
CONFIG += shared dll

DEFINES -= UNICODE

LIBS += \
    -lopencv_core \
    -lopencv_flann \
    -lopencv_imgproc \

HEADERS += \
    include/opencv2/features2d/features2d.hpp \
    src/fast_score.hpp \

PRECOMPILED_HEADER += \
    src/precomp.hpp

SOURCES += \
    src/stardetector.cpp \
    src/precomp.cpp \
    src/orb.cpp \
    src/mser.cpp \
    src/matchers.cpp \
    src/keypoint.cpp \
    src/freak.cpp \
    src/features2d_init.cpp \
    src/fast_score.cpp \
    src/fast.cpp \
    src/evaluation.cpp \
    src/dynamic.cpp \
    src/draw.cpp \
    src/detectors.cpp \
    src/descriptors.cpp \
    src/brisk.cpp \
    src/brief.cpp \
    src/blobdetector.cpp \
    src/bagofwords.cpp
