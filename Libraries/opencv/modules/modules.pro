include(../opencv.pri)

TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS += \
    core/opencv_core.pro \
    flann/opencv_flann.pro \
    imgproc/opencv_imgproc.pro \
    features2d/opencv_features2d.pro \
    calib3d/opencv_calib3d.pro \
    highgui/opencv_highgui.pro \
    objdetect/opencv_objdetect.pro \
    video/opencv_video.pro \
    ml/opencv_ml.pro \
    contrib/opencv_contrib.pro \
    legacy/opencv_legacy.pro \
    gpu/opencv_gpu.pro \
    #ocl/opencv_ocl.pro \
    photo/opencv_photo.pro \
    nonfree/opencv_nonfree.pro \
    stitching/opencv_stitching.pro \
    superres/opencv_superres.pro \
    ts/opencv_ts.pro \
    videostab/opencv_videostab.pro
