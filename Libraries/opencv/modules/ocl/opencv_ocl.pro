include (../../opencv.pri)

TEMPLATE = lib
CONFIG += shared dll

DEFINES -= UNICODE

LIBS += \
    -lopencv_core \
    -lopencv_imgproc \
    -lopencv_objdetect \
    -lopencv_features2d \
    -lopencv_flann

HEADERS += \
    include/opencv2/ocl/ocl.hpp \
    include/opencv2/ocl/matrix_operations.hpp \
    include/opencv2/ocl/private/util.hpp \
    src/safe_call.hpp \
    src/binarycaching.hpp

PRECOMPILED_HEADER += \
    src/precomp.hpp

SOURCES += \
    src/arithm.cpp \
    src/tvl1flow.cpp \
    src/stereobp.cpp \
    src/stereobm.cpp \
    src/stereo_csbp.cpp \
    src/split_merge.cpp \
    src/pyrup.cpp \
    src/pyrlk.cpp \
    src/pyrdown.cpp \
    src/precomp.cpp \
    src/mssegmentation.cpp \
    src/moments.cpp \
    src/mcwutil.cpp \
    src/matrix_operations.cpp \
    src/match_template.cpp \
    src/interpolate_frames.cpp \
    src/initialization.cpp \
    src/imgproc.cpp \
    src/hog.cpp \
    src/haar.cpp \
    src/gfft.cpp \
    src/gemm.cpp \
    src/filtering.cpp \
    src/fft.cpp \
    src/error.cpp \
    src/columnsum.cpp \
    src/color.cpp \
    src/canny.cpp \
    src/build_warps.cpp \
    src/brute_force_matcher.cpp \
    src/blend.cpp

