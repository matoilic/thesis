include(../globals.pro)

TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += ../CameraCalib

LIBS += -L$$BUILDPATH -lCameraCalib

SOURCES += main.cpp

