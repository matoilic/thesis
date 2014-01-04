include (../../QMake/Ardoor.pri)
include (../../QMake/OpenCv.pri)

QT += qml quick opengl

LIBS += \
    -lArdoor \
    -lopencv_core

OTHER_FILES += \
    main.qml \
    android/AndroidManifest.xml

HEADERS += \
    AppController.hpp

SOURCES += \
    main.cpp \
    AppController.cpp

RESOURCES += \
    resources.qrc


ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

ANDROID_EXTRA_LIBS = \
    $$BUILDPATH/libopencv_core.so \
    $$BUILDPATH/libopencv_imgproc.so \
    $$BUILDPATH/libopencv_flann.so \
    $$BUILDPATH/libopencv_features2d.so \
    $$BUILDPATH/libopencv_calib3d.so \
    $$BUILDPATH/libopencv_video.so \
    $$BUILDPATH/libopencv_highgui.so \
    $$BUILDPATH/libArdoor.so \
