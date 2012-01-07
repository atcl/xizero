// atCROSSLEVEL 2010,2011,2012
// licensed under zlib-license
// XZbasic.hh
// Basic Library 

///guard
#ifndef HH_XZBASIC
#define HH_XZBASIC
///*

///definitions
#define VERSION "0.1"

#if __GNUC__ <= 4 && __GNUC_MINOR__ < 5
 #define u8 
#endif

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
// #elsedef NEVER
//  #define INLINE
 #else
  #define INLINE inline
 #endif
 
#else

 #define LIKELY(x)
 #define UNLIKELY(x)
 #define PREFETCH(x,y,z)

 #define FCALL
 #define PMAIN 
 #define CONST
 #define HOTFN
 
 #ifdef ALWAYS_INLINE
  #define INLINE inline
// #elsedef NEVER_INLINE
//  #define INLINE
 #else
  #define INLINE inline
 #endif
 
#endif

#define TITLE "XiZero"

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
#define LPNK    0x00FF80FF
#define LYELLOW 0x0080FFFF
#define WHITE   0x00FFFFFF
#define SYSCOL  0x00C0C0C0
#define TRANS   0xFF000000
#define DRED    0x00000080

typedef const char*   cstr;
typedef unsigned char uchar;
typedef unsigned long ulong;
typedef signed long   slong;

union packed
{
	long  d;
	short w[2];
	uchar b[4];
};
///*

#endif
