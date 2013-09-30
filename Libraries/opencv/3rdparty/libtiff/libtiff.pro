include (../../opencv.pri)

TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    tif_zip.c \
    tif_write.c \
    tif_win32.c \
    tif_warning.c \
    tif_version.c \
    tif_unix.c \
    tif_tile.c \
    tif_thunder.c \
    tif_swab.c \
    tif_strip.c \
    tif_stream.cxx \
    tif_read.c \
    tif_print.c \
    tif_predict.c \
    tif_pixarlog.c \
    tif_packbits.c \
    tif_open.c \
    tif_ojpeg.c \
    tif_next.c \
    tif_lzw.c \
    tif_lzma.c \
    tif_luv.c \
    tif_jpeg_12.c \
    tif_jpeg.c \
    tif_jbig.c \
    tif_getimage.c \
    tif_flush.c \
    tif_fax3sm.c \
    tif_fax3.c \
    tif_extension.c \
    tif_error.c \
    tif_dumpmode.c \
    tif_dirwrite.c \
    tif_dirread.c \
    tif_dirinfo.c \
    tif_dir.c \
    tif_compress.c \
    tif_color.c \
    tif_codec.c \
    tif_close.c \
    tif_aux.c

OTHER_FILES += \
    COPYRIGHT \
    CMakeLists.txt \
    ChangeLog

HEADERS += \
    uvcode.h \
    tiffvers.h \
    tiffiop.h \
    tiffio.hxx \
    tiffio.h \
    tiff.h \
    tif_predict.h \
    tif_fax3.h \
    tif_dir.h \
    tif_config.h.cmakein \
    tif_config.h \
    t4.h
