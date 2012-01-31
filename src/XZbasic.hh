// atCROSSLEVEL 2010,2011,2012
// licensed under zlib-license
// XZbasic.hh
// Basic Library 

///guard
#ifndef HH_XZBASIC
#define HH_XZBASIC
///*

///definitions
#ifdef __GNUC__
	#define LIKELY(x)   __builtin_expect((x),1)
	#define UNLIKELY(x) __builtin_expect((x),0)
	#define PREFETCH(x,y,z) __builtin_prefetch(x,y,z)

	#define FCALL __attribute__((fastcall))
	#define PMAIN __attribute__((constructor)) 
	#define CONST __attribute__((const))
	#define HOTFN __attribute__((hot))

	#ifdef ALWAYS
		#define INLINE __attribute__((always_inline))
		//#define inline __attribute__((always_inline))
	#else
		#define INLINE inline
	#endif
#else
	#define LIKELY(x) x
	#define UNLIKELY(x) x
	#define PREFETCH(x,y,z)

	#define FCALL
	#define PMAIN 
	#define CONST
	#define HOTFN
#endif

#ifndef TITLE
 #define TITLE "_"
#endif

#ifndef VERSION
 #define VERSION "0.0"
#endif

#define XZOK 'XZOK'

#define BLACK   0x00000000
#define BLUE    0x00FF0000
#define GREEN   0x0000FF00
#define CYAN    0x00FFFF00
#define RED     0x000000FF
#define PINK    0x00FF00FF
#define YELLOW  0x0000FFFF
#define DWHITE  0x00808080
#define GRAY    0x00404040
#define LBLUE   0x00FF8080
#define LGREEN  0x0080FF80
#define LCYAN   0x00FFFF80
#define LRED    0x008080FF
#define LPINK   0x00FF80FF
#define LYELLOW 0x0080FFFF
#define WHITE   0x00FFFFFF

#define SYSCOL  0x00C0C0C0
#define TRANS   0xFF000000
#define DRED    0x00000080
#define DGREEN  0x00008000
#define DBLUE   0x00800000
#define SKY     0x00FF8000
#define ORANGE  0x000080FF
#define OCHER   0x00008080

typedef const char*   ccstr;
typedef unsigned char uchar;
typedef unsigned long ulong;
typedef signed   long slong;

union packed
{
	ulong d;
	uchar b[4];
};

union color
{
	ulong c;
	uchar a;
	uchar r;
	uchar g;
	uchar b;
};
///*

#endif

