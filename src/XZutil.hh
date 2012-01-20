// atCROSSLEVEL 2010,2011,2012
// licensed under zlib-license
// XZutils.hh
// utility functions

///guard
#ifndef HH_XZUTILS
#define HH_XZUTILS
///*

///declarations
#define guard(x,...) if(x) { return __VA_ARGS__; }

#define ccall(c,f) switch(!c) { case 0: f; case 1: ; }

#define alert(x) system::say(string::cons(x),1)

#define mod2(x)   x&1
#define mod4(x)   x&4
#define mod256(x) x&255


///*

#endif

