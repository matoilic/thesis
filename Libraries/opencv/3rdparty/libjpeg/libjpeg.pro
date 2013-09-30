include (../../opencv.pri)

TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    transupp.c \
    jutils.c \
    jquant2.c \
    jquant1.c \
    jmemmgr.c \
    jmemansi.c \
    jidctred.c \
    jidctint.c \
    jidctfst.c \
    jidctflt.c \
    jfdctint.c \
    jfdctfst.c \
    jfdctflt.c \
    jerror.c \
    jdtrans.c \
    jdsample.c \
    jdpostct.c \
    jdphuff.c \
    jdmerge.c \
    jdmaster.c \
    jdmarker.c \
    jdmainct.c \
    jdinput.c \
    jdhuff.c \
    jddctmgr.c \
    jdcolor.c \
    jdcoefct.c \
    jdatasrc.c \
    jdatadst.c \
    jdapistd.c \
    jdapimin.c \
    jctrans.c \
    jcsample.c \
    jcprepct.c \
    jcphuff.c \
    jcparam.c \
    jcomapi.c \
    jcmaster.c \
    jcmarker.c \
    jcmainct.c \
    jcinit.c \
    jchuff.c \
    jcdctmgr.c \
    jccolor.c \
    jccoefct.c \
    jcapistd.c \
    jcapimin.c

OTHER_FILES += \
    README \
    CMakeLists.txt

HEADERS += \
    transupp.h \
    jversion.h \
    jpeglib.h \
    jpegint.h \
    jmorecfg.h \
    jmemsys.h \
    jinclude.h \
    jerror.h \
    jdhuff.h \
    jdct.h \
    jconfig.h \
    jchuff.h
