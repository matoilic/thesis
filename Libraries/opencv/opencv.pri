release: ENVIRONMENT = "release"
debug:   ENVIRONMENT = "debug"

OPENCV_DIR = $$PWD
BUILDPATH = $$OPENCV_DIR/build/$$ENVIRONMENT
DESTDIR = $$BUILDPATH

OBJECTS_DIR = $$DESTDIR/obj
MOC_DIR = $$DESTDIR/moc
RCC_DIR = $$DESTDIR/qrc
UI_DIR = $$DESTDIR/ui

LIBS += -L$$DESTDIR

DEFINES += CVAPI_EXPORTS=1
win32 {
    DEFINES += _VARIADIC_MAX=10
}

MODULES = calib3d contrib core features2d flann gpu highgui imgproc legacy \
          ml nonfree objdetect ocl photo stitching superres ts video videostab

INCLUDEPATH += $$OPENCV_DIR
INCLUDEPATH += $$OPENCV_DIR/include

for (module, MODULES) {
    INCLUDEPATH += \
        $$OPENCV_DIR/modules/$$module/ \
        $$OPENCV_DIR/modules/$$module/include \
}

INCLUDEPATH += $$OPENCV_DIR/3rdparty/zlib
