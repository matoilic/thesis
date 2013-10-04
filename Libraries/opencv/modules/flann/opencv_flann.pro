include (../../opencv.pri)

TEMPLATE = lib
CONFIG += shared dll

DEFINES -= UNICODE

LIBS += \
    -lopencv_core

HEADERS += \
    include/opencv2/flann/timer.h \
    include/opencv2/flann/simplex_downhill.h \
    include/opencv2/flann/saving.h \
    include/opencv2/flann/sampling.h \
    include/opencv2/flann/result_set.h \
    include/opencv2/flann/random.h \
    include/opencv2/flann/params.h \
    include/opencv2/flann/object_factory.h \
    include/opencv2/flann/nn_index.h \
    include/opencv2/flann/miniflann.hpp \
    include/opencv2/flann/matrix.h \
    include/opencv2/flann/lsh_table.h \
    include/opencv2/flann/lsh_index.h \
    include/opencv2/flann/logger.h \
    include/opencv2/flann/linear_index.h \
    include/opencv2/flann/kmeans_index.h \
    include/opencv2/flann/kdtree_single_index.h \
    include/opencv2/flann/kdtree_index.h \
    include/opencv2/flann/index_testing.h \
    include/opencv2/flann/hierarchical_clustering_index.h \
    include/opencv2/flann/heap.h \
    include/opencv2/flann/hdf5.h \
    include/opencv2/flann/ground_truth.h \
    include/opencv2/flann/general.h \
    include/opencv2/flann/flann_base.hpp \
    include/opencv2/flann/flann.hpp \
    include/opencv2/flann/dynamic_bitset.h \
    include/opencv2/flann/dummy.h \
    include/opencv2/flann/dist.h \
    include/opencv2/flann/defines.h \
    include/opencv2/flann/config.h \
    include/opencv2/flann/composite_index.h \
    include/opencv2/flann/autotuned_index.h \
    include/opencv2/flann/any.h \
    include/opencv2/flann/allocator.h \
    include/opencv2/flann/all_indices.h

PRECOMPILED_HEADER += \
    src/precomp.hpp

SOURCES += \
    src/precomp.cpp \
    src/miniflann.cpp \
    src/flann.cpp

