release: ENVIRONMENT = "release"
debug:   ENVIRONMENT = "debug"

ARDOOR_DIR = $$PWD

win32 {
    BUILDPATH = $$ARDOOR_DIR/Build-Win32/$$ENVIRONMENT
}

!win32 {
    BUILDPATH = $$ARDOOR_DIR/Build-Unix/$$ENVIRONMENT
}

macx {
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
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.7
}

DESTDIR = $$BUILDPATH

OBJECTS_DIR = $$DESTDIR/obj/$$TARGET
MOC_DIR = $$DESTDIR/moc/$$TARGET
RCC_DIR = $$DESTDIR/qrc/$$TARGET
UI_DIR = $$DESTDIR/ui/$$TARGET

LIBS += -L$$DESTDIR
INCLUDEPATH += $$ARDOOR_DIR/Common
win32 {
    DEFINES += __STDC_CONSTANT_MACROS
    DEFINES += _CRT_SECURE_NO_WARNINGS
    DEFINES += snprintf="_snprintf"
    INCLUDEPATH += $$ARDOOR_DIR/Libraries/msinttypes
}

debug {
    QMAKE_CXXFLAGS += -DDEBUG=1
}

win32 {
    DEFINES += WIN32
}
