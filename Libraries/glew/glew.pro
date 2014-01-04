include (../../QMake/Ardoor.pri)

TEMPLATE = lib
CONFIG += shared dll

INCLUDEPATH += $$PWD/include

SOURCES += \
    #src/glewinfo.c \
    src/glew.c

macx {
    LIBS += \
        -framework OpenGL\
        -framework AGL
}

win32 {
    LIBS += \
        -lopengl32
}
