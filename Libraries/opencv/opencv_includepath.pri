
MODULES = calib3d contrib core features2d flann gpu highgui imgproc legacy \
          ml nonfree objdetect ocl photo stitching superres ts video videostab

# OpenCV default includes
INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD/include

# OpenCV module includes
for (module, MODULES) {
    INCLUDEPATH += \
        $$PWD/modules/$$module/ \
        $$PWD/modules/$$module/include \
}
