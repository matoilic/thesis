include(../ardoor.pri)

TEMPLATE  = subdirs
CONFIG   += ordered

SUBDIRS = \
    CameraCalib \
    Capture \
    PoseEstimation \
    CameraWindow

