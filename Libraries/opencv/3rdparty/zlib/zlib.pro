include (../../opencv.pri)

TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    zutil.c \
    uncompr.c \
    trees.c \
    inftrees.c \
    inflate.c \
    inffast.c \
    infback.c \
    gzwrite.c \
    gzread.c \
    gzlib.c \
    gzclose.c \
    deflate.c \
    crc32.c \
    compress.c \
    adler32.c

HEADERS += \
    zutil.h \
    zlib.h \
    zconf.h \
    trees.h \
    inftrees.h \
    inflate.h \
    inffixed.h \
    inffast.h \
    gzguts.h \
    deflate.h \
    crc32.h
