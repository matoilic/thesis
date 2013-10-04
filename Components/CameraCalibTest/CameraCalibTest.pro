include(../../globals.pro)

TEMPLATE = app
CONFIG += console qt
CONFIG -= app_bundle

INCLUDEPATH += ../CameraCalib

LIBS += -L$$BUILDPATH -lCameraCalib

SOURCES += main.cpp
