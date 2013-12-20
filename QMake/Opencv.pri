
MODULES = calib3d contrib core features2d flann gpu highgui imgproc legacy \
          ml nonfree objdetect ocl photo stitching superres ts video videostab

OPENCV_DIR = $$PWD/../Libraries/opencv

# OpenCV default includes
INCLUDEPATH += $$OPENCV_DIR
INCLUDEPATH += $$OPENCV_DIR/include

# OpenCV module includes
for (module, MODULES) {
    INCLUDEPATH += \
        $$OPENCV_DIR/modules/$$module/ \
        $$OPENCV_DIR/modules/$$module/include \
}
