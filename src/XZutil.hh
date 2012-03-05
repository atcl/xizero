// atCROSSLEVEL 2010,2011,2012
// released under zlib/libpng license
// XZutils.hh
// utility functions

///guard
#ifndef HH_XZUTILS
#define HH_XZUTILS
///*

///declarations
#define guard(x,...) if(x) { return __VA_ARGS__; } //switch(!x) { case 0: return __VA_ARGS__; case 1: ; }

#define VAL(x) #x
#define STR(x) VAL(x)

#define alert(x) system::say(string::int2str(x),1)

#define mod2(x)   x&1
#define mod4(x)   x&3
#define mod32(x)  x&31
#define mod256(x) x&255
///*

#endif

