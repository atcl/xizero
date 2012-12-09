///<header>
// atCROSSLEVEL 2010,2011,2012
// released under 2-clause BSD license
// XZgame.hh
// Game Library 
#pragma once
///</header>

///<include>
#include "XZbasic.hh"
#include "XZmath.hh"
#include "XZvector.hh"
///</include>

///<define>
namespace game
{
	/*OK*/ pure inline bool onscreen(sint x,sint y);
	            inline bool collision(const fvector& x,const fvector& a,fixed r);
	                   sint angle(const fvector& x,const fvector& t,const fvector& m);
}
///</define>

///<code>
bool game::onscreen(sint x,sint y)
{
	return (x>0) && (y>0) && (x<XRES) && (y<YRES);
}

bool game::collision(const fvector& x,const fvector& a,fixed r)
{
	return (a.x>x.x-r)&&(a.x<x.x+r)&&(a.y>x.y-r)&&(a.y<x.y+r); 	//box works
	//const fvector t(a-x);						//component-wise?
	//return t.dot(t)<r*r;						//circle doesn't
}

sint game::angle(const fvector& x,const fvector& t,const fvector& m)
{
	return 0;
}
///</code>

