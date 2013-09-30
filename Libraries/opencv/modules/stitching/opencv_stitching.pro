include (../../opencv.pri)

TEMPLATE = lib
CONFIG += shared dll

DEFINES -= UNICODE

LIBS += \
    -lopencv_core \
    -lopencv_features2d \
    -lopencv_flann \
    -lopencv_imgproc \
    -lopencv_gpu \
    -lopencv_objdetect \
    -lopencv_nonfree \
    -lopencv_calib3d

HEADERS += \
    include/opencv2/stitching/warpers.hpp \
    include/opencv2/stitching/stitcher.hpp \
    include/opencv2/stitching/detail/warpers_inl.hpp \
    include/opencv2/stitching/detail/warpers.hpp \
    include/opencv2/stitching/detail/util_inl.hpp \
    include/opencv2/stitching/detail/util.hpp \
    include/opencv2/stitching/detail/seam_finders.hpp \
    include/opencv2/stitching/detail/motion_estimators.hpp \
    include/opencv2/stitching/detail/matchers.hpp \
    include/opencv2/stitching/detail/exposure_compensate.hpp \
    include/opencv2/stitching/detail/camera.hpp \
    include/opencv2/stitching/detail/blenders.hpp \
    include/opencv2/stitching/detail/autocalib.hpp \

PRECOMPILED_HEADER += \
    src/precomp.hpp

SOURCES += \
    src/warpers.cpp \
    src/util.cpp \
    src/stitcher.cpp \
    src/seam_finders.cpp \
    src/precomp.cpp \
    src/motion_estimators.cpp \
    src/matchers.cpp \
    src/exposure_compensate.cpp \
    src/camera.cpp \
    src/blenders.cpp \
    src/autocalib.cpp

