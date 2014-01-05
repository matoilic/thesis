include (../QMake/Ardoor.pri)

TEMPLATE  = subdirs
CONFIG   += ordered


SUBDIRS = \
    SettingsApp \
    CalibrationApp \
    ChessboardApp \
    CameraCalibTest \
    CaptureTest \
    #CameraWindowTest \
    #GlfwTest \
    DoorDetectorTest \
    #DoorProjectionTest \
    SolvePnpTest \
    VideoCaptureTest
