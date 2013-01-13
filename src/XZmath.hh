///<header>
// atCROSSLEVEL 2010,2011,2012,2013
// released under 2-clause BSD license
// XZmath.hh
// Math Library 
#pragma once
///</header>

///<include>
#include "XZbasic.hh"
///</include>

///<declare>
#define SEED 22695477
///</declare>

///<code>
namespace math
{
	/*OK*/ pure inline sint sgn(sint x) { return (x>>31)|(sint)(((uint)-x)>>31); }
	/*OK*/ pure inline sint abs(sint x) { register const sint y=x>>31; return (x^y)-y; }
	/*OK*/ pure inline sint min(sint x,sint y) { return y^((x^y) & -(x<y)); }
	/*OK*/ pure inline sint max(sint x,sint y) { return x^((x^y) & -(x<y)); }
	/*OK*/ pure inline sint neg(sint x,bool y) { return ~(x^(y-1))+y; }
	/*OK*/ pure inline sint set(sint x,bool y) { return ((sint(!y))-1) & x; }
	/*OK*/ pure inline sint set(sint x,sint y,bool z) { return (((sint(!z))-1) & x) | ((sint(z)-1) & y); }
	/*OK*/ pure inline sint lim(sint x,sint a,sint y) { return max(x,min(a,y)); }
	       pure inline void swp(sint& x,sint& y,bool z) { z&&(x!=y);  } //TODO
	/*OK*/ pure        sint sqr(uint x) { register uint r=0; for(uint i=15,t=0;i!=0;--i) { t=(r+(1<<i))<<i; r=set(r|(2<<i),r,x>=t); x=set(x-t,x,x>=t); } return r>>1; }  
	/*OK*/             uint rnd(uint x) { static uint seed=SEED; ++seed; seed^=(seed<<15); seed^=(seed>>21); seed^=(seed<<4); return seed%x; } 
}
///</code>

