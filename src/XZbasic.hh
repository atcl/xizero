///<header>
// atCROSSLEVEL 2010,2011,2012,2013
// released under 2-clause BSD license
// XZbasic.hh
// Basic Library 
///</header>

///<include>
#pragma once
#include <cstdint> //int32_t,uint32_t
#include <cstdlib> //malloc,free
///</include>

///<declare>
#define FRMBUF

#ifdef __GNUC__

	#define pure        __attribute__((const))
	#define alignas(x)  __attribute__((aligned (x))) //remove when gcc supports alignas (4.8)
	#define vector      __attribute__((vector_size (16))); //simd
	#define hot         __attribute__((hot))
	#define prefetch(x) __builtin_prefetch(x)
	#define ifl(x)   if(__builtin_expect(!!(x),1))
	#define ifu(x)   if(__builtin_expect(!!(x),0))

	#ifdef ALWAYS
		#define inline __attribute__((always_inline)) inline
	#endif
#else
	#define pure
	#define alignas(x)
	#define hot
	#define prefetch(x)
	#define ifl(x)   if(x)
	#define ifu(x)   if(x)
#endif 

#define guard(x,...) ifu(x) { return __VA_ARGS__; }

#define VAL(x) #x
#define STR(x) VAL(x)

#define mod2(x)    (x&1)
#define mod4(x)    (x&3)
#define mod32(x)   (x&31)
#define mod256(x)  (x&255)
#define mod1024(x) (x&1023)

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

#define BLACK   0x00000000
#define BLUE    0x000000FF
#define GREEN   0x0000FF00
#define CYAN    0x0000FFFF
#define RED     0x00FF0000
#define PINK    0x00FF00FF
#define YELLOW  0x00FFFF00
#define DWHITE  0x00808080
#define DRED    0x00800000
#define DGREEN  0x00008000
#define DBLUE   0x00000080
#define OCHER   0x00808000
#define SKY     0x000080FF
#define ORANGE  0x00FF8000
#define WHITE   0x00FFFFFF
#define SYSCOL  0x00C0C0C0

#define TRANS   0xFF000000
///</declare>

///<define>
typedef unsigned char  byte;
typedef uint32_t       uint;
typedef int32_t        sint;

union packed
{
	uint d;
	byte b[4];
};
///</define>

///<code>
//global new + delete (required for gcc over g++)
inline void* operator new(uint s)       { return malloc(s); }
inline void* operator new[](uint s)     { return malloc(s); }
inline void  operator delete(void *p)   { free(p); }
inline void  operator delete[](void *p) { free(p); }
///</code>

