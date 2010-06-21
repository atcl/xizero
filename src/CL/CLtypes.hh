///license
//atCROSSLEVEL studios 2009,2010
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLTYPES
#define HH_CLTYPES
///*

//Compiler check for unicode extension
#if __GNUC__ <= 4 && __GNUC_MINOR__ < 5
	#define u8 
#endif
//*

///includes
#include <inttypes.h>
#include <cstring>
///*

///declarations
#define CLversion 0x00020000 
///*

///declarations
//32bit types
typedef char		 xchar;
typedef uint8_t		uxchar;
typedef int16_t		 xshort;
typedef uint16_t	uxshort;
typedef int32_t		 xlong;
typedef uint32_t	uxlong;
typedef int64_t		 xhyper;
typedef uint64_t	uxhyper;
//***
///*

#endif
