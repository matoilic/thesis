include (../../opencv.pri)

!win32 {
    QT += core gui widgets concurrent testlib opengl
}

TEMPLATE = lib
CONFIG += shared dll

DEFINES -= UNICODE

win32 {
    LIBS += \
        -lopengl32 \
        -ladvapi32 \
        -lole32 \
        -lcomctl32 \
        -lvfw32 \        
        -loleaut32 \
        #ffmpeg libs
        -lavcodec \
        -lavdevice \
        -lavfilter \
        -lavformat \
        -lavutil \
        -lpostproc \
        -lswresample \
        -lswscale
}

macx {
    LIBS += \
        -framework OpenGL \
        -framework GLUT \
        #-framework QTKit \
        -framework Foundation \
        -framework CoreVideo \
        -framework CoreMedia \
        -framework CoreFoundation \
        -framework AVFoundation \
        -framework ImageCaptureCore \
        -framework ImageIO \
        -framework CoreServices \
        -framework AppKit \
        -framework Cocoa
}

LIBS += \
    -lzlib \
    -lopencv_core \
    -lopencv_imgproc \
    -llibjpeg \
    -llibpng \
    -llibtiff

HEADERS += \
    ../../cvconfig.h \
    include/opencv2/highgui/ios.h \
    include/opencv2/highgui/highgui_c.h \
    include/opencv2/highgui/highgui.hpp \
    include/opencv2/highgui/cap_ios.h \
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

win32 {
    SOURCES += \
        src/window_w32.cpp \
        src/cap_dshow.cpp \
        src/cap_vfw.cpp \
}

macx {
    #HEADERS += \
    #    src/window_QT.h \

    #SOURCES += \
    #    src/window_QT.cpp \

    OBJECTIVE_SOURCES += \
        #src/cap_qtkit.mm \
        src/cap_avfoundation.mm \
        #src/window_cocoa.mm
}

unix:!macx {
    SOURCES += \
        src/window_gtk.cpp
}

SOURCES += \    
    src/window.cpp \
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
    #src/cap_v4l.cpp \
    #src/cap_unicap.cpp \
    #src/cap_tyzx.cpp \
    #src/cap_pvapi.cpp \
    #src/cap_openni.cpp \
    #src/cap_msmf.cpp \
    #src/cap_mil.cpp \
    #src/cap_libv4l.cpp \
    src/cap_images.cpp \
    #src/cap_gstreamer.cpp \
    #src/cap_giganetix.cpp \
    src/cap_ffmpeg.cpp \
    #src/cap_dc1394_v2.cpp \
    #src/cap_dc1394.cpp \
    src/cap_cmu.cpp \
    #src/cap_android.cpp \
    src/cap.cpp \
    src/bitstrm.cpp \
    src/utils_highgui.cpp
