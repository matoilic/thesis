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
    LswmsTest \
    #DoorProjectionTest \
    SolvePnpTest \
    VideoCaptureTest
