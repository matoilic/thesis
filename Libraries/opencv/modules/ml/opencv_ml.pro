include (../../opencv.pri)

TEMPLATE = lib
CONFIG += shared dll

DEFINES -= UNICODE

LIBS += \
    -lopencv_core \
    -lzlib

HEADERS += \
    include/opencv2/ml/ml.hpp \

PRECOMPILED_HEADER += \
    src/precomp.hpp

SOURCES += \
    src/tree.cpp \
    src/testset.cpp \
    src/svm.cpp \
    src/rtrees.cpp \
    src/precomp.cpp \
    src/nbayes.cpp \
    src/ml_init.cpp \
    src/knearest.cpp \
    src/inner_functions.cpp \
    src/gbt.cpp \
    src/estimate.cpp \
    src/ertrees.cpp \
    src/em.cpp \
    src/data.cpp \
    src/cnn.cpp \
    src/boost.cpp \
    src/ann_mlp.cpp

