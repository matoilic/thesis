include (../../opencv.pri)

TEMPLATE = lib
CONFIG += shared dll

DEFINES -= UNICODE
LIBS += \
    -lopencv_core \
    -lopencv_imgproc \
    -lopencv_flann \
    -lopencv_highgui \
    -lopencv_features2d \
    -lopencv_calib3d \
    -lopencv_objdetect \
    -lopencv_video \
    -lopencv_photo \
    -lopencv_ml \
    -lopencv_legacy \
    -lopencv_gpu \
    -lzlib \

HEADERS += \
    include/opencv2/nonfree/ocl.hpp \
    include/opencv2/nonfree/nonfree.hpp \
    include/opencv2/nonfree/gpu.hpp \
    include/opencv2/nonfree/features2d.hpp

PRECOMPILED_HEADER += \
    src/precomp.hpp

SOURCES += \
    src/surf_gpu.cpp \
    src/surf.ocl.cpp \
    src/surf.cpp \
    src/sift.cpp \
    src/precomp.cpp \
    src/nonfree_init.cpp

