include (../../opencv.pri)

TEMPLATE = lib
CONFIG += shared dll

DEFINES -= UNICODE

LIBS += \
    -lopengl32 \
    -ladvapi32 \
    -lole32 \
    -lcomctl32 \
    -lvfw32 \
    -lzlib \
    -loleaut32 \
    -lopencv_core \
    -lopencv_imgproc \
    -llibjpeg \
    -llibpng \
    -llibtiff \

HEADERS += \
    include/opencv2/highgui/ios.h \
    include/opencv2/highgui/highgui_c.h \
    include/opencv2/highgui/highgui.hpp \
    include/opencv2/highgui/cap_ios.h \
    #src/window_QT.h \
    src/utils.hpp \
    src/grfmts.hpp \
    src/grfmt_tiff.hpp \
    src/grfmt_sunras.hpp \
    src/grfmt_pxm.hpp \
    src/grfmt_png.hpp \
    src/grfmt_jpeg2000.hpp \
    src/grfmt_jpeg.hpp \
    src/grfmt_imageio.hpp \
    src/grfmt_exr.hpp \
    src/grfmt_bmp.hpp \
    src/grfmt_base.hpp \
    src/ffmpeg_codecs.hpp \
    src/cap_ffmpeg_impl.hpp \
    src/cap_ffmpeg_api.hpp \
    src/bitstrm.hpp

PRECOMPILED_HEADER += \
    src/precomp.hpp

SOURCES += \
    src/window_w32.cpp \
    #src/window_QT.cpp \
    #src/window_gtk.cpp \
    #src/window_carbon.cpp \
    src/window.cpp \
    src/utils.cpp \
    src/precomp.cpp \
    src/loadsave.cpp \
    src/grfmt_tiff.cpp \
    src/grfmt_sunras.cpp \
    src/grfmt_pxm.cpp \
    src/grfmt_png.cpp \
    src/grfmt_jpeg2000.cpp \
    src/grfmt_jpeg.cpp \
    src/grfmt_imageio.cpp \
    src/grfmt_exr.cpp \
    src/grfmt_bmp.cpp \
    src/grfmt_base.cpp \
    #src/cap_xine.cpp \
    #src/cap_ximea.cpp \
    src/cap_vfw.cpp \
    #src/cap_v4l.cpp \
    #src/cap_unicap.cpp \
    #src/cap_tyzx.cpp \
    #src/cap_qt.cpp \
    #src/cap_pvapi.cpp \
    #src/cap_openni.cpp \
    #src/cap_msmf.cpp \
    #src/cap_mil.cpp \
    #src/cap_libv4l.cpp \
    src/cap_images.cpp \
    #src/cap_gstreamer.cpp \
    #src/cap_giganetix.cpp \
    src/cap_ffmpeg.cpp \
    src/cap_dshow.cpp \
    #src/cap_dc1394_v2.cpp \
    #src/cap_dc1394.cpp \
    src/cap_cmu.cpp \
    #src/cap_android.cpp \
    src/cap.cpp \
    src/bitstrm.cpp
