include (../../opencv.pri)

TEMPLATE = lib
CONFIG += shared dll

DEFINES -= UNICODE

LIBS += \
    -lopencv_core \
    -lopencv_imgproc \
    -lopencv_video \
    -lopencv_features2d \
    -lopencv_flann \
    -lopencv_gpu \
    -lopencv_highgui \
    -lopencv_objdetect \
    -lopencv_photo

HEADERS += \
    include/opencv2/videostab/videostab.hpp \
    include/opencv2/videostab/stabilizer.hpp \
    include/opencv2/videostab/optical_flow.hpp \
    include/opencv2/videostab/motion_stabilizing.hpp \
    include/opencv2/videostab/log.hpp \
    include/opencv2/videostab/inpainting.hpp \
    include/opencv2/videostab/global_motion.hpp \
    include/opencv2/videostab/frame_source.hpp \
    include/opencv2/videostab/fast_marching_inl.hpp \
    include/opencv2/videostab/fast_marching.hpp \
    include/opencv2/videostab/deblurring.hpp \

PRECOMPILED_HEADER += \
    src/precomp.hpp

SOURCES += \
    src/stabilizer.cpp \
    src/precomp.cpp \
    src/motion_stabilizing.cpp \
    src/log.cpp \
    src/inpainting.cpp \
    src/fast_marching.cpp \
    src/deblurring.cpp \
    src/optical_flow_videostab.cpp \
    src/global_motion_videostab.cpp \
    src/frame_source_videostab.cpp

