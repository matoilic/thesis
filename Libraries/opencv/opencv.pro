include(opencv.pri)

TEMPLATE = subdirs
CONFIG += ordered

LIBS += -L$$BUILDPATH

SUBDIRS += 3rdparty modules
