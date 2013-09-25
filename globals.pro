release: ENVIRONMENT = "release"
debug:   ENVIRONMENT = "debug"

BUILDPATH = $$PWD/build/$$ENVIRONMENT
DESTDIR = $$BUILDPATH

OBJECTS_DIR = $$DESTDIR/obj
MOC_DIR = $$DESTDIR/moc
RCC_DIR = $$DESTDIR/qrc
UI_DIR = $$DESTDIR/ui

unix:!macx {
    CONFIG += link_pkgconfig
    PKGCONFIG += opencv
}

macx {
    QMAKE_LFLAGS += -F/Library/Frameworks
    LIBS += -framework opencv2
}


