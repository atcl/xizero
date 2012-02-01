// atCROSSLEVEL 2010,2011,2012
// licensed under zlib-license
// XZmath.hh
// Math Library 

///guard
#ifndef HH_XZMATH
#define HH_XZMATH
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
	/*OK*/ inline sint sgn(sint x) { return (x>>31)|(sint)(((uint)-x)>>31); }
	/*OK*/ inline sint abs(sint x) { return ((sint(x>=0)-1) ^ x) | (x<0); }
	/*OK*/ inline sint ndn(sint x) { return ((x>>24)&0x000000ff) | ((x<<8)&0x00ff0000) | ((x>>8)&0x0000ff00) | ((x<<24)&0xff000000); }
	/*OK*/ inline sint min(sint x,sint y) { return y^((x^y) & -(x<y)); }
	/*OK*/ inline sint max(sint x,sint y) { return x^((x^y) & -(x<y)); }
	/*OK*/ inline sint neg(sint x,bool y) { return ~(x^(y-1))+y; } 
	/*OK*/ inline sint set(sint x,bool y) { return ((sint(!y))-1) & x; }
	/*OK*/ inline sint set(sint x,sint y,bool z) { return (((sint(!z))-1) & x) | ((sint(z)-1) & y); }
	/*OK*/ inline sint lim(sint x,sint a,sint y) { return max(x,min(a,y)); }
	/*OK*/        sint sqr(uint x) { uint r=0; uint t=0; for(uint i=15;i!=0;--i) { t=(r+(1<<i))<<i; r=set(r|(2<<i),r,x>=t); x=set(x-t,x,x>=t); } return r>>1; } //HOTFN 
	/*OK*/        sint rnd(uint x) { static sint seed=SEED; ++seed; seed^=(seed<<15); seed^=(seed>>21); seed^=(seed<<4); return seed%x; }
}
///*

#endif

