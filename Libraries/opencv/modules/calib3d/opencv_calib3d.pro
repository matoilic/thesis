include (../../opencv.pri)

TEMPLATE = lib
CONFIG += shared dll

DEFINES -= UNICODE

LIBS += \
    -lopencv_core \
    -lopencv_imgproc \
    -lopencv_features2d \
    -lopencv_flann \

HEADERS += \
    include/opencv2/calib3d/calib3d.hpp \
    src/precomp.hpp \
    src/polynom_solver.h \
    src/p3p.h \
    src/epnp.h \
    src/circlesgrid.hpp \
    src/_modelest.h \

PRECOMPILED_HEADER += \
    src/precomp.hpp

SOURCES += \
    src/triangulate.cpp \
    src/stereosgbm.cpp \
    src/stereobm.cpp \
    src/solvepnp.cpp \
    src/quadsubpix.cpp \
    src/precomp.cpp \
    src/posit.cpp \
    src/polynom_solver.cpp \
    src/p3p.cpp \
    src/modelest.cpp \
    src/fundam.cpp \
    src/epnp.cpp \
    src/circlesgrid.cpp \
    src/checkchessboard.cpp \
    src/calibration.cpp \
    src/calibinit.cpp

