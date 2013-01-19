///<header>
// atCROSSLEVEL 2010,2011,2012,2013
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
	/*OK*/             void compiled(sint x,sint y,uint r,uint c);
	/*OK*/ pure inline bool onscreen(sint x,sint y);
	            inline bool collision(const fvector& x,const fvector& a,fixed r);
	                   sint angle(const char** l,const fvector& p,uint m);
}
///</define>

///<code>
void game::compiled(sint x,sint y,uint r,uint c)
{
	//guard(game::onscreen(x,y)==1);
	uint o = ((y-2)*XRES)+(x-2);

	screen::back[o++] = r; screen::back[o++] = r; screen::back[o++] = r; screen::back[o] = r; o += XRES-3;
	screen::back[o++] = r; screen::back[o++] = c; screen::back[o++] = c; screen::back[o] = r; o += XRES-3;
	screen::back[o++] = r; screen::back[o++] = c; screen::back[o++] = c; screen::back[o] = r; o += XRES-3;
	screen::back[o++] = r; screen::back[o++] = r; screen::back[o++] = r; screen::back[o] = r; o += XRES-3;
}

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

sint game::angle(const char** l,const fvector& p,uint m)
{
	//const uint x = p.x/BWIDTH;
	//const uint y = (p.y-m)/BWIDTH;

	return 0;
}
///</code>

