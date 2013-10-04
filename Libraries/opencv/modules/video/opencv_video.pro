include (../../opencv.pri)

TEMPLATE = lib
CONFIG += shared dll

DEFINES -= UNICODE

LIBS += \
    -lopencv_core \
    -lopencv_imgproc \
    -lopencv_calib3d \
    -lzlib

HEADERS += \
    include/opencv2/video/video.hpp \
    include/opencv2/video/tracking.hpp \
    include/opencv2/video/background_segm.hpp \
    src/simpleflow.hpp \
    src/lkpyramid.hpp

PRECOMPILED_HEADER += \
    src/precomp.hpp

SOURCES += \
    src/video_init.cpp \
    src/simpleflow.cpp \
    src/precomp.cpp \
    src/optflowgf.cpp \
    src/motempl.cpp \
    src/lkpyramid.cpp \
    src/kalman.cpp \
    src/camshift.cpp \
    src/bgfg_gmg.cpp \
    src/bgfg_gaussmix2.cpp \
    src/bgfg_gaussmix_video.cpp \
    src/tvl1flow_video.cpp

