include (../../opencv.pri)

TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    pngwutil.c \
    pngwtran.c \
    pngwrite.c \
    pngwio.c \
    pngtrans.c \
    pngset.c \
    pngrutil.c \
    pngrtran.c \
    pngrio.c \
    pngread.c \
    pngpread.c \
    pngmem.c \
    pngget.c \
    pngerror.c \
    png.c

OTHER_FILES += \
    README \
    LICENSE \
    CMakeLists.txt \
    CHANGES

HEADERS += \
    pngstruct.h \
    pngpriv.h \
    pnglibconf.h \
    pnginfo.h \
    pngdebug.h \
    pngconf.h \
    png.h \
