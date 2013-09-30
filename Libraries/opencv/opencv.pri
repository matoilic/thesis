include (../../ardoor.pri)

OPENCV_DIR = $$PWD

DEFINES += HAVE_CVCONFIG_H CVAPI_EXPORTS
win32 {
    DEFINES += _VARIADIC_MAX=10
}

MODULES = calib3d contrib core features2d flann gpu highgui imgproc legacy \
          ml nonfree objdetect ocl photo stitching superres ts video videostab

# OpenCV default includes
INCLUDEPATH += $$OPENCV_DIR
INCLUDEPATH += $$OPENCV_DIR/include

# OpenCV module includes
for (module, MODULES) {
    INCLUDEPATH += \
        $$OPENCV_DIR/modules/$$module/ \
        $$OPENCV_DIR/modules/$$module/include \
}

# 3rdparty library includes
INCLUDEPATH += $$OPENCV_DIR/3rdparty/libjpeg
INCLUDEPATH += $$OPENCV_DIR/3rdparty/libpng
INCLUDEPATH += $$OPENCV_DIR/3rdparty/libtiff
INCLUDEPATH += $$OPENCV_DIR/3rdparty/zlib
