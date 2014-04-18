///<header>
// Ξ0 - xizero ( Version 0.2 ) 
// atCROSSLEVEL 2010-2014 ( http://atcrosslevel.de )
// released under 2-clause BSD license
// Math Library ( XZmath.hh )
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
	pure inline xint sgn(xint x) { return (x>>31)|1; }
	pure inline xint abs(xint x) { register const xint y=x>>31; return (x^y)-y; }
	pure inline xint min(xint x,xint y) { return y^((x^y) & -(x<y)); }
	pure inline xint max(xint x,xint y) { return x^((x^y) & -(x<y)); }
	pure inline xint neg(xint x,bool y) { return (x^(-y))+y; }
	pure inline xint set(xint x,bool y) { return ((xint(!y))-1) & x; }
	pure inline xint set(xint x,xint y,bool z) { return (((xint(!z))-1) & x) | ((xint(z)-1) & y); }
	pure inline xint lim(xint x,xint a,xint y) { return max(x,min(a,y)); }
	pure        yint sqr(yint x) { register yint r=0; for(yint i=15,t=0;i!=0;--i) { t=(r+(1<<i))<<i; r=set(r|(2<<i),r,x>=t); x=set(x-t,x,x>=t); } return r>>1; }  
	            yint rnd(yint x) { static yint seed=SEED; ++seed; seed^=(seed<<15); seed^=(seed>>21); seed^=(seed<<4); return seed%x; } 
}
///</code>

