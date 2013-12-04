include(../ardoor.pri)

TEMPLATE  = subdirs
CONFIG   += ordered

SUBDIRS = \
	CameraCalibTest \
        CaptureTest \
        ProjectionTest \
        CameraWindowTest \
        DoorDetectionTest \
        DoorDetection \
        LswmsTest
