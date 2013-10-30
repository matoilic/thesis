include (../../ardoor.pri)

TEMPLATE = lib
CONFIG += shared dll

INCLUDEPATH += $$PWD/include
INCLUDEPATH += $$PWD/deps

SOURCES += \
    src/window.c \        
    src/time.c \
    src/monitor.c \
    src/joystick.c \
    src/input.c \
    src/init.c \    
    src/gamma.c \
    #src/egl_context.c \
    src/context.c \
    src/clipboard.c

macx {
    CONFIG -= c++11
    QMAKE_CXXFLAGS -= -stdlib=libc++
    QMAKE_CXXFLAGS -= -std=c++11
    QMAKE_CXXFLAGS += -std=c99

    SOURCES += \
        src/cocoa_time.c \
        src/cocoa_gamma.c \

    OBJECTIVE_SOURCES += \
        src/cocoa_clipboard.m \
        src/cocoa_init.m \
        src/cocoa_monitor.m \
        src/cocoa_window.m \
        src/nsgl_context.m \
        src/cocoa_joystick.m

    HEADERS += \
        src/cocoa_platform.h

    LIBS += \
        -framework IOKit \
        -framework OpenGL \
        -framework Foundation \
        -framework CoreFoundation \
        -framework QuartzCore \
        -framework Quartz \
        -framework ApplicationServices \
        -framework AppKit
}

win32 {
    SOURCES += \
        src/wgl_context.c \
        src/win32_window.c \
        src/win32_time.c \
        src/win32_monitor.c \
        src/win32_joystick.c \
        src/win32_init.c \
        src/win32_gamma.c \
        src/win32_clipboard.c

    HEADERS += \
        src/win32_platform.h
}

unix:!macx {
    SOURCES += \
        src/glx_context.c \
        src/x11_window.c \
        src/x11_unicode.c \
        src/x11_time.c \
        src/x11_monitor.c \
        src/x11_joystick.c \
        src/x11_init.c \
        src/x11_gamma.c \
        src/x11_clipboard.c

    HEADERS += \
        src/x11_platform.h
}

HEADERS += \   
    src/config.h \
    src/wgl_platform.h \
    src/nsgl_platform.h \
    src/internal.h \
    src/glx_platform.h \
    src/egl_platform.h

