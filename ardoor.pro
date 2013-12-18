include(QMake/Ardoor.pri)

TEMPLATE  = subdirs
CONFIG   += ordered warn_off

SUBDIRS = \
	Libraries \
	Source \
	Applications

cache()
