// atCROSSLEVEL 2010,2011,2012
// released under zlib/libpng license
// XZbasic.hh
// Basic Library 

///guard
#ifndef HH_XZBASIC
#define HH_XZBASIC
//#pragma message "Compiling " __FILE__ "..." " TODO: ."
///*

///includes
#include <cstdint>
#include <cstdlib> //malloc,free
#include <malloc.h>
///*

///declarations
#ifdef __GNUC__

	#define pure __attribute__((pure))

	#define prefetch(x) __builtin_prefetch(x)

	#define ifl(x) if(__builtin_expect(!!(x),1))

	#define ifu(x) if(__builtin_expect(!!(x),0))

	#ifdef ALWAYS
		#define inline __attribute__((always_inline))
	#endif
#else
	#define pure

	#define prefetch(x)

	#define ifl(x) if(x)

	#define ifu(x) if(x)
#endif 

#define guard(x,...) if(__builtin_expect(!!(x),0)) { return __VA_ARGS__; }

#define VAL(x) #x
#define STR(x) VAL(x)

#define alert(x) system::say(string::int2str(x),1)
#define alerf(x) system::say(string::fix2str(x),1)

#define mod2(x)   x&1
#define mod4(x)   x&3
#define mod32(x)  x&31
#define mod256(x) x&255


#ifndef TITLE
	#define TITLE "_"
#endif

#ifndef VERSION
	#define VERSION "0.0"
#endif

#ifndef XRES
	#define XRES 800
#endif

#ifndef YRES
	#define YRES 600
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
#define GRAY    0x00404040
#define LBLUE   0x008080FF
#define LGREEN  0x0080FF80
#define LCYAN   0x0080FFFF
#define LRED    0x00FF8080
#define LPINK   0x00FF80FF
#define LYELLOW 0x00FFFF80
#define WHITE   0x00FFFFFF

#define SYSCOL  0x00C0C0C0
#define TRANS   0xFF000000
#define DRED    0x00800000
#define DGREEN  0x00008000
#define DBLUE   0x00000080
#define SKY     0x000080FF
#define ORANGE  0x00FF8000
#define OCHER   0x00808000

typedef const    char* cstr;
typedef unsigned char  byte;
typedef uint32_t       uint;
typedef int32_t        sint;

union packed
{
	uint d;
	byte b[4];
};

//global new + delete overloading
inline void* operator new(uint s)   { return malloc(s); }
inline void* operator new[](uint s) { return malloc(s); }
inline void  operator delete(void *p)   { free(p); }
inline void  operator delete[](void *p) { free(p); }
//*

///*

#endif

