include (../../opencv.pri)

TEMPLATE = lib
CONFIG += shared dll

DEFINES -= UNICODE

LIBS += \
    -lopencv_core

HEADERS += \
    include/opencv2/imgproc/types_c.h \
    include/opencv2/imgproc/imgproc_c.h \
    include/opencv2/imgproc/imgproc.hpp \
    src/gcgraph.hpp \
    src/_list.h \
    src/_geom.h

PRECOMPILED_HEADER += \
    src/precomp.hpp

SOURCES += \
    src/undistort.cpp \
    src/thresh.cpp \
    src/templmatch.cpp \
    src/sumpixels.cpp \
    src/subdivision2d.cpp \
    src/smooth.cpp \
    src/shapedescr.cpp \
    src/segmentation.cpp \
    src/samplers.cpp \
    src/rotcalipers.cpp \
    src/precomp.cpp \
    src/phasecorr.cpp \
    src/morph.cpp \
    src/moments.cpp \
    src/matchcontours.cpp \
    src/linefit.cpp \
    src/imgwarp.cpp \
    src/hough.cpp \
    src/histogram.cpp \
    src/grabcut.cpp \
    src/geometry.cpp \
    src/generalized_hough.cpp \
    src/gabor.cpp \
    src/floodfill.cpp \
    src/filter.cpp \
    src/featureselect.cpp \
    src/emd.cpp \
    src/distransform.cpp \
    src/deriv.cpp \
    src/cornersubpix.cpp \
    src/corner.cpp \
    src/convhull.cpp \
    src/contours.cpp \
    src/clahe.cpp \
    src/canny.cpp \
    src/approx.cpp \
    src/accum.cpp \
    src/tables_imgproc.cpp \
    src/utils_imgproc.cpp \
    src/pyramids_imageproc.cpp \
    src/color_imgproc.cpp

