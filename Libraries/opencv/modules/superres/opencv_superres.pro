include (../../opencv.pri)

TEMPLATE = lib
CONFIG += shared dll

DEFINES -= UNICODE

LIBS += \
    -lopencv_core \
    -lopencv_imgproc \
    -lopencv_video \
    -lopencv_gpu \
    -lopencv_objdetect \
    -lopencv_features2d \
    -lopencv_flann \
    -lopencv_highgui

HEADERS += \
    include/opencv2/superres/superres.hpp \
    include/opencv2/superres/optical_flow.hpp \
    src/ring_buffer.hpp \
    src/input_array_utility.hpp

PRECOMPILED_HEADER += \
    src/precomp.hpp

SOURCES += \
    src/super_resolution.cpp \
    src/precomp.cpp \
    src/btv_l1_gpu.cpp \
    src/btv_l1.cpp \
    src/optical_flow_superres.cpp \
    src/input_array_utility.cpp \
    src/frame_source_videores.cpp

