include (../../QMake/Application.pri)

QT += core gui opengl multimedia

LIBS += \
    -lArdoor \
    -lopencv_core \
    -lopencv_calib3d \
    -lopencv_imgproc \

win32 {
    LIBS += -lopengl32
}

macx {
    LIBS += -framework OpenGL
    LIBS += -framework Cocoa
}

SOURCES += \
    main.cpp

HEADERS += \

OTHER_FILES += \
    android/AndroidManifest.xml


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
