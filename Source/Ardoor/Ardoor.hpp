#ifndef ARDOOR_H
#define ARDOOR_H

#include <QtCore/qglobal.h>
#include <Ardoor/Rendering/GLUtils.hpp>

using namespace std;


#if defined(ARDOOR_LIBRARY)
#  define ARD_EXPORT Q_DECL_EXPORT
#else
#  define ARD_EXPORT Q_DECL_IMPORT
#endif

#if !defined(ARDOOR_DLL)
#  undef ARD_EXPORT
#  define ARD_EXPORT
#endif

#if (defined DEBUG && defined NVWA)
#include <debug_new.h>
#endif


#endif


