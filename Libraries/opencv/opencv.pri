include (../../QMake/Ardoor.pri)

OPENCV_DIR = $$PWD

DEFINES += HAVE_CVCONFIG_H
win32 {
    DEFINES += _VARIADIC_MAX=10 CVAPI_EXPORTS
}

MODULES = calib3d contrib core features2d flann gpu highgui imgproc legacy \
          ml nonfree objdetect ocl photo stitching superres ts video videostab

# OpenCV default includes
INCLUDEPATH += $$OPENCV_DIR
INCLUDEPATH += $$OPENCV_DIR/include

# OpenCV module includes
for (module, MODULES) {
    INCLUDEPATH += \
        $$OPENCV_DIR/modules/$$module \
        $$OPENCV_DIR/modules/$$module/include \
        $$OPENCV_DIR/modules/$$module/src
}
#INCLUDEPATH += $$OPENCV_DIR/modules/highgui/src

# 3rdparty library includes
INCLUDEPATH += $$OPENCV_DIR/3rdparty/libjpeg
INCLUDEPATH += $$OPENCV_DIR/3rdparty/libpng
INCLUDEPATH += $$OPENCV_DIR/3rdparty/libtiff
INCLUDEPATH += $$OPENCV_DIR/3rdparty/zlib
INCLUDEPATH += $$OPENCV_DIR/3rdparty/include/ffmpeg_
