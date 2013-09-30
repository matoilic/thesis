include (../../opencv.pri)

TEMPLATE = lib
CONFIG += shared dll

DEFINES -= UNICODE

LIBS += \
    -lopencv_core \
    -lopencv_imgproc \
    -lopencv_features2d \
    -lopencv_objdetect \
    -lopencv_video \
    -lopencv_highgui \
    -lopencv_ml \
    -lopencv_calib3d \
    -lopencv_flann \

HEADERS += \
    include/opencv2/contrib/retina.hpp \
    include/opencv2/contrib/openfabmap.hpp \
    include/opencv2/contrib/hybridtracker.hpp \
    include/opencv2/contrib/detection_based_tracker.hpp \
    include/opencv2/contrib/contrib.hpp \
    src/templatebuffer.hpp \
    src/retinafilter.hpp \
    src/retinacolor.hpp \
    src/parvoretinafilter.hpp \
    src/magnoretinafilter.hpp \
    src/imagelogpolprojection.hpp \
    src/basicretinafilter.hpp

PRECOMPILED_HEADER += \
    src/precomp.hpp

SOURCES += \
    src/stereovar.cpp \
    src/spinimages.cpp \
    src/selfsimilarity.cpp \
    src/rgbdodometry.cpp \
    src/retinafilter.cpp \
    src/retinacolor.cpp \
    src/retina.cpp \
    src/precomp.cpp \
    src/polyfit.cpp \
    src/parvoretinafilter.cpp \
    src/openfabmap.cpp \
    src/octree.cpp \
    src/magnoretinafilter.cpp \
    src/logpolar_bsm.cpp \
    src/lda.cpp \
    src/inputoutput.cpp \
    src/imagelogpolprojection.cpp \
    src/hybridtracker.cpp \
    src/gencolors.cpp \
    src/fuzzymeanshifttracker.cpp \
    src/featuretracker.cpp \
    src/facerec.cpp \
    src/detection_based_tracker.cpp \
    src/contrib_init.cpp \
    src/colortracker.cpp \
    src/colormap.cpp \
    src/chowliutree.cpp \
    src/chamfermatching.cpp \
    src/bowmsctrainer.cpp \
    src/basicretinafilter.cpp \
    src/ba.cpp \
    src/adaptiveskindetector.cpp
