include (../QMake/Ardoor.pri)

TEMPLATE  = subdirs
CONFIG   += ordered


SUBDIRS = \
    SettingsApp \
    CalibrationApp \
    ChessboardApp \
    DoorApp \
    CameraCalibTest \
    CaptureTest \
    #CameraWindowTest \
    #GlfwTest \
    DoorDetectorTest \
    #DoorProjectionTest \
    SolvePnpTest \
    VideoCaptureTest
