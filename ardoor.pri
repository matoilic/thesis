release: ENVIRONMENT = "release"
debug:   ENVIRONMENT = "debug"

ARDOOR_DIR = $$PWD
BUILDPATH = $$ARDOOR_DIR/Build/$$ENVIRONMENT
DESTDIR = $$BUILDPATH

OBJECTS_DIR = $$DESTDIR/obj
MOC_DIR = $$DESTDIR/moc
RCC_DIR = $$DESTDIR/qrc
UI_DIR = $$DESTDIR/ui

LIBS += -L$$DESTDIR
INCLUDEPATH += $$PWD/Common

debug {
    QMAKE_CXXFLAGS += -DDEBUG=1
}

macx {
    CONFIG -= app_bundle
    CONFIG += c++11
    QMAKE_CXXFLAGS += -mmacosx-version-min=10.7
    QMAKE_LFLAGS += -mmacosx-version-min=10.7
    QMAKE_CXXFLAGS += -stdlib=libc++
    QMAKE_CXXFLAGS += -std=c++11
    QMAKE_CXXFLAGS += -Wno-unused-parameter
    QMAKE_CXXFLAGS += -Wno-narrowing
    QMAKE_CXXFLAGS += -Wno-deprecated-declarations
    QMAKE_CXXFLAGS += -Wno-unused-private-field
    LIBS += -framework IOKit
    LIBS += -stdlib=libc++
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.8
}

win32 {
    DEFINES += WIN32
}
