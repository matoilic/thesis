include (../../opencv.pri)

TEMPLATE = lib
CONFIG += shared dll

DEFINES -= UNICODE

LIBS += \
    -lopencv_core \
    -lopencv_imgproc \
    -lopencv_objdetect \
    -lopencv_features2d \
    -lopencv_flann \
    -lopencv_video \
    -lopencv_calib3d

macx {
    LIBS += \
        -framework OpenCL \
}

HEADERS += \
    include/opencv2/ocl/ocl.hpp \
    include/opencv2/ocl/matrix_operations.hpp \
    include/opencv2/ocl/private/util.hpp \
    src/safe_call.hpp \
    src/binarycaching.hpp

PRECOMPILED_HEADER += \
    src/precomp.hpp

SOURCES += \
    src/arithm_ocl.cpp \
    src/blend_ocl.cpp \
    src/brute_force_matcher_ocl.cpp \
    src/canny_ocl.cpp \
    src/color_ocl.cpp \
    src/error_ocl.cpp \
    src/fft_ocl.cpp \
    src/filtering_ocl.cpp \
    src/haar_ocl.cpp \
    src/hog_ocl.cpp \
    src/imgproc_ocl.cpp \
    src/match_template_ocl.cpp \
    src/matrix_operations_ocl.cpp \
    src/moments_ocl.cpp \
    src/mssegmentation_ocl.cpp \
    src/precomp_ocl.cpp \
    src/pyrdown_ocl.cpp \
    src/pyrlk_ocl.cpp \
    src/pyrup_ocl.cpp \
    src/split_merge_ocl.cpp \
    src/stereo_csbp_ocl.cpp \
    src/stereobm_ocl.cpp \
    src/stereobp_ocl.cpp \
    src/tvl1flow_ocl.cpp \
    src/columnsum_ocl.cpp \
    src/gemm_ocl.cpp \
    src/gfft_ocl.cpp \
    src/initialization_ocl.cpp \
    src/interpolate_frames_ocl.cpp \
    src/mcwutil_ocl.cpp \
    src/build_warps_ocl.cpp

