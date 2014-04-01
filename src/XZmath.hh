///<header>
// atCROSSLEVEL 2010-2014
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
	/*OK*/ pure inline xint sgn(xint x) { return (x>>31)|1; }
	/*OK*/ pure inline xint abs(xint x) { register const xint y=x>>31; return (x^y)-y; }
	/*OK*/ pure inline xint min(xint x,xint y) { return y^((x^y) & -(x<y)); }
	/*OK*/ pure inline xint max(xint x,xint y) { return x^((x^y) & -(x<y)); }
	/*OK*/ pure inline xint neg(xint x,bool y) { return (x^(-y))+y; }
	/*OK*/ pure inline xint set(xint x,bool y) { return ((xint(!y))-1) & x; }
	/*OK*/ pure inline xint set(xint x,xint y,bool z) { return (((xint(!z))-1) & x) | ((xint(z)-1) & y); }
	/*OK*/ pure inline xint lim(xint x,xint a,xint y) { return max(x,min(a,y)); }
	/*OK*/ pure        xint sqr(yint x) { register yint r=0; for(yint i=15,t=0;i!=0;--i) { t=(r+(1<<i))<<i; r=set(r|(2<<i),r,x>=t); x=set(x-t,x,x>=t); } return r>>1; }  
	/*OK*/             yint rnd(yint x) { static yint seed=SEED; ++seed; seed^=(seed<<15); seed^=(seed>>21); seed^=(seed<<4); return seed%x; } 
}
///</code>

