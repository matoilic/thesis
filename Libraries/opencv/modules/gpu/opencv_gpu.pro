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
    -lopencv_calib3d \
    -lopencv_video

HEADERS += \
    include/opencv2/gpu/stream_accessor.hpp \
    include/opencv2/gpu/gpumat.hpp \
    include/opencv2/gpu/gpu.hpp \
    include/opencv2/gpu/devmem2d.hpp \
    include/opencv2/gpu/device/warp_shuffle.hpp \
    include/opencv2/gpu/device/warp_reduce.hpp \
    include/opencv2/gpu/device/warp.hpp \
    include/opencv2/gpu/device/vec_traits.hpp \
    include/opencv2/gpu/device/vec_math.hpp \
    include/opencv2/gpu/device/vec_distance.hpp \
    include/opencv2/gpu/device/utility.hpp \
    include/opencv2/gpu/device/type_traits.hpp \
    include/opencv2/gpu/device/transform.hpp \
    include/opencv2/gpu/device/static_check.hpp \
    include/opencv2/gpu/device/simd_functions.hpp \
    include/opencv2/gpu/device/scan.hpp \
    include/opencv2/gpu/device/saturate_cast.hpp \
    include/opencv2/gpu/device/reduce.hpp \
    include/opencv2/gpu/device/limits.hpp \
    include/opencv2/gpu/device/functional.hpp \
    include/opencv2/gpu/device/funcattrib.hpp \
    include/opencv2/gpu/device/filters.hpp \
    include/opencv2/gpu/device/emulation.hpp \
    include/opencv2/gpu/device/dynamic_smem.hpp \
    include/opencv2/gpu/device/datamov_utils.hpp \
    include/opencv2/gpu/device/common.hpp \
    include/opencv2/gpu/device/color.hpp \
    include/opencv2/gpu/device/border_interpolate.hpp \
    include/opencv2/gpu/device/block.hpp \
    include/opencv2/gpu/device/detail/vec_distance_detail.hpp \
    include/opencv2/gpu/device/detail/type_traits_detail.hpp \
    include/opencv2/gpu/device/detail/transform_detail.hpp \
    include/opencv2/gpu/device/detail/reduce_key_val.hpp \
    include/opencv2/gpu/device/detail/reduce.hpp \
    include/opencv2/gpu/device/detail/color_detail.hpp \
    src/video_parser.h \
    src/video_decoder.h \
    src/thread_wrappers.h \
    src/precomp.hpp \
    src/frame_queue.h \
    src/ffmpeg_video_source.h \
    src/cvt_color_internal.h \
    src/cuvid_video_source.h \
    src/cu_safe_call.h \
    src/nvidia/NCVHaarObjectDetection.hpp \
    src/nvidia/NCVBroxOpticalFlow.hpp \
    src/nvidia/NPP_staging/NPP_staging.hpp \
    src/nvidia/core/NCVRuntimeTemplates.hpp \
    src/nvidia/core/NCVPyramid.hpp \
    src/nvidia/core/NCVPixelOperations.hpp \
    src/nvidia/core/NCVColorConversion.hpp \
    src/nvidia/core/NCVAlg.hpp \
    src/nvidia/core/NCV.hpp

PRECOMPILED_HEADER += \
    src/precomp.hpp

SOURCES += \
    src/warp.cpp \
    src/video_writer.cpp \
    src/video_reader.cpp \
    src/video_parser.cpp \
    src/video_decoder.cpp \
    src/tvl1flow.cpp \
    src/thread_wrappers.cpp \
    src/stereocsbp.cpp \
    src/stereobp.cpp \
    src/stereobm.cpp \
    src/split_merge.cpp \
    src/speckle_filtering.cpp \
    src/resize.cpp \
    src/remap.cpp \
    src/pyrlk.cpp \
    src/pyramids.cpp \
    src/precomp.cpp \
    src/orb.cpp \
    src/optical_flow_farneback.cpp \
    src/optical_flow.cpp \
    src/optflowbm.cpp \
    src/mssegmentation.cpp \
    src/matrix_reductions.cpp \
    src/matrix_operations.cpp \
    src/match_template.cpp \
    src/imgproc.cpp \
    src/hough.cpp \
    src/hog.cpp \
    src/graphcuts.cpp \
    src/global_motion.cpp \
    src/gftt.cpp \
    src/frame_queue.cpp \
    src/filtering.cpp \
    src/fgd_bgfg.cpp \
    src/ffmpeg_video_source.cpp \
    src/fast.cpp \
    src/error.cpp \
    src/element_operations.cpp \
    src/denoising.cpp \
    src/cuvid_video_source.cpp \
    src/cudastream.cpp \
    src/cu_safe_call.cpp \
    src/color.cpp \
    src/cascadeclassifier.cpp \
    src/calib3d.cpp \
    src/brute_force_matcher.cpp \
    src/blend.cpp \
    src/bilateral_filter.cpp \
    src/bgfg_mog.cpp \
    src/bgfg_gmg.cpp \
    src/arithm.cpp

