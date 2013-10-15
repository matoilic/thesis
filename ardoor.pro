include(ardoor.pri)

TEMPLATE  = subdirs
CONFIG   += ordered warn_off

SUBDIRS = \
	Libraries \
	Components \
	Applications

cache()
