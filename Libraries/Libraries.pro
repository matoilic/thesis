include(../QMake/Ardoor.pri)

TEMPLATE  = subdirs
CONFIG   += ordered

SUBDIRS = \
    opencv \
   # freeglut
    glfw \
    glew \
    nvwa
