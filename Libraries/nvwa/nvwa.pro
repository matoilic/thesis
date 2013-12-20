include (../../QMake/Ardoor.pri)

TEMPLATE = lib
CONFIG += staticlib
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    static_mem_pool.cpp \
    mem_pool_base.cpp \
    debug_new.cpp \
    bool_array.cpp

HEADERS += \
    static_mem_pool.h \
    static_assert.h \
    set_assign.h \
    pctimer.h \
    object_level_lock.h \
    mem_pool_base.h \
    fixed_mem_pool.h \
    fc_queue.h \
    fast_mutex.h \
    debug_new.h \
    cont_ptr_utils.h \
    class_level_lock.h \
    bool_array.h
