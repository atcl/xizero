///<header>
// atCROSSLEVEL 2010-2014
// released under 2-clause BSD license
// XZbasic.hh
// Basic Library 
#pragma once
///</header>

///<include>
#include <cstdint> // int32_t,uint32_t
#include <cstdlib> // malloc,free

#include "XZcolors.hh"
///</include>

///<declare>
#define FRMBUF

#ifdef __GNUC__

	#define pure        __attribute__((const))
	#define hot         __attribute__((hot))
	#define prefetch(x) __builtin_prefetch(x)
	#define ifl(x)   if(__builtin_expect(!!(x),1))
	#define ifu(x)   if(__builtin_expect(!!(x),0))

	#ifdef ALWAYS
		#define inline __attribute__((always_inline)) inline
	#endif
#else
	#define pure
	#define hot
	#define prefetch(x)
	#define ifl(x)   if(x)
	#define ifu(x)   if(x)
#endif 

#define guard(x,...) ifu(x) { return __VA_ARGS__; }

#define VAL(x) #x
#define STR(x) VAL(x)

#ifndef TITLE
	#define TITLE "_"
#endif

#ifndef VERSION
	#define VERSION "0.0"
#endif

#ifndef XRES
	#define XRES 640
#endif

#ifndef YRES
	#define YRES 480
#endif

#define XZOK    'XZOK'
///</declare>

///<define>
typedef unsigned char byte;
typedef int32_t       xint;
typedef uint32_t      yint;

union  rgba  { yint d; byte b[4]; };
struct tuple { xint x; xint y; xint z; xint e; };
///</define>

///<code>
// global new + delete (required for gcc over g++)
inline void* operator new(size_t s)     { return malloc(s); }
inline void* operator new[](size_t s)   { return malloc(s); }
inline void  operator delete(void *p)   { free(p); }
inline void  operator delete[](void *p) { free(p); }
///</code>

