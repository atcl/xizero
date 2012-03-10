// atCROSSLEVEL 2010,2011,2012
// released under zlib/libpng license
// XZmath.hh
// Math Library 

///guard
#ifndef HH_XZMATH
#define HH_XZMATH
//#pragma message "Compiling " __FILE__ "..." " TODO: ."
///*

///includes
#include "XZbasic.hh"
///*

///declaration
#define SEED 22695477
///*

///definition
namespace math
{
	/*OK*/ pure inline sint sgn(sint x) { return (x>>31)|(sint)(((uint)-x)>>31); }
	/*OK*/ pure inline sint abs(sint x) { sint y=x>>31; x^=y; x-=y; return x; }
	/*OK*/ pure inline sint ndn(sint x) { return ((x>>24)&0x000000ff) | ((x<<8)&0x00ff0000) | ((x>>8)&0x0000ff00) | ((x<<24)&0xff000000); }
	/*OK*/ pure inline sint min(sint x,sint y) { return y^((x^y) & -(x<y)); }
	/*OK*/ pure inline sint max(sint x,sint y){ return x^((x^y) & -(x<y)); }
	/*OK*/ pure inline sint neg(sint x,bool y) { return ~(x^(y-1))+y; }
	/*OK*/ pure inline sint set(sint x,bool y) { return ((sint(!y))-1) & x; }
	/*OK*/ pure inline sint set(sint x,sint y,bool z) { return (((sint(!z))-1) & x) | ((sint(z)-1) & y); }
	/*OK*/ pure inline sint lim(sint x,sint a,sint y) { return max(x,min(a,y)); }
	/*OK*/ pure        sint sqr(uint x) { uint r=0; uint t=0; for(uint i=15;i!=0;--i) { t=(r+(1<<i))<<i; r=set(r|(2<<i),r,x>=t); x=set(x-t,x,x>=t); } return r>>1; } //HOTFN 
	/*OK*/             uint rnd(uint x) { static uint seed=SEED; ++seed; seed^=(seed<<15); seed^=(seed>>21); seed^=(seed<<4); return seed%x; } 
	inline pure void swp(void*& a,void*& b) { __asm__ __volatile__ ("xchgl %0,%1;": "=a"(a),"=b"(b) :"a"(a),"b"(b): ); }
}
///*

#endif

