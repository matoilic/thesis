#ifndef ARDOOR_H
#define ARDOOR_H

#if (defined WIN32 || defined _WIN32)
#  define ARD_EXPORT __declspec(dllexport)
#else
#  define ARD_EXPORT
#endif

#ifdef DEBUG
#include <debug_new.h>
#endif

#endif
