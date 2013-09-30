include (../../opencv.pri)

TEMPLATE = lib
CONFIG += shared dll

DEFINES -= UNICODE

LIBS += \
    -lopencv_core \
    -lopencv_imgproc \
    -lopencv_highgui \

HEADERS += \
    include/opencv2/objdetect/objdetect.hpp \
    src/cascadedetect.hpp \
    src/_lsvmparser.h \
    src/_lsvm_types.h \
    src/_lsvm_tbbversion.h \
    src/_lsvm_routine.h \
    src/_lsvm_resizeimg.h \
    src/_lsvm_matching.h \
    src/_lsvm_fft.h \
    src/_lsvm_error.h \
    src/_lsvm_distancetransform.h \
    src/_latentsvm.h

PRECOMPILED_HEADER += \
    src/precomp.hpp

SOURCES += \
    src/routine.cpp \
    src/resizeimg.cpp \
    src/precomp.cpp \
    src/objdetect_init.cpp \
    src/matching.cpp \
    src/lsvmtbbversion.cpp \
    src/lsvmparser.cpp \
    src/linemod.cpp \
    src/latentsvmdetector.cpp \
    src/latentsvm.cpp \
    src/hog.cpp \
    src/haar.cpp \
    src/fft.cpp \
    src/featurepyramid.cpp \
    src/distancetransform.cpp \
    src/datamatrix.cpp \
    src/cascadedetect.cpp
