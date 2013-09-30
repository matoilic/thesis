include (../../opencv.pri)

TEMPLATE = lib
CONFIG += shared dll

DEFINES -= UNICODE

LIBS += \
    -lopencv_core \
    -lopencv_imgproc

HEADERS += \
    include/opencv2/photo/photo_c.h \
    include/opencv2/photo/photo.hpp \
    src/fast_nlmeans_multi_denoising_invoker.hpp \
    src/fast_nlmeans_denoising_invoker_commons.hpp \
    src/fast_nlmeans_denoising_invoker.hpp \
    src/arrays.hpp

PRECOMPILED_HEADER += \
    src/precomp.hpp

SOURCES += \
    src/precomp.cpp \
    src/inpaint.cpp \
    src/denoising.cpp

