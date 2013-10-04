include (../../opencv.pri)

TEMPLATE = lib
CONFIG += shared dll

DEFINES -= UNICODE

LIBS += \
    -lzlib

win32 {
    LIBS += \
        -lopengl32
}

macx {
    LIBS += \
        -framework Carbon \
        -framework OpenGL \
        -framework GLUT
}

unix:!macx {
    LIBS += \
        -lGL \
        -lGLU \
        -lglut
}

HEADERS += \
    include/opencv2/core/wimage.hpp \
    include/opencv2/core/version.hpp \
    include/opencv2/core/types_c.h \
    include/opencv2/core/operations.hpp \
    include/opencv2/core/opengl_interop_deprecated.hpp \
    include/opencv2/core/opengl_interop.hpp \
    include/opencv2/core/mat.hpp \
    include/opencv2/core/internal.hpp \
    include/opencv2/core/gpumat.hpp \
    include/opencv2/core/eigen.hpp \
    include/opencv2/core/devmem2d.hpp \
    include/opencv2/core/cuda_devptrs.hpp \
    include/opencv2/core/core_c.h \
    include/opencv2/core/core.hpp

PRECOMPILED_HEADER += \
    src/precomp.hpp

SOURCES += \
    src/system.cpp \
    src/stat.cpp \
    src/rand.cpp \
    src/precomp.cpp \
    src/persistence.cpp \
    src/parallel.cpp \
    src/out.cpp \
    src/opengl_interop_deprecated.cpp \
    src/opengl_interop.cpp \
    src/matrix.cpp \
    src/matop.cpp \
    src/matmul.cpp \
    src/mathfuncs.cpp \
    src/lapack.cpp \
    src/gpumat.cpp \
    src/glob.cpp \
    src/gl_core_3_1.cpp \
    src/dxt.cpp \
    src/drawing.cpp \
    src/datastructs.cpp \
    src/copy.cpp \
    src/convert.cpp \
    src/cmdparser.cpp \
    src/array.cpp \
    src/arithm.cpp \
    src/alloc.cpp \
    src/algorithm.cpp \
    src/gl_core_3_1.hpp \
    src/tables_core.cpp
