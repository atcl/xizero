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

#define BWIDTH  16
#define MAXSTEP  5

///<define>
namespace game
{
	/*OK*/             void compiled(sint x,sint y,uint r,uint c);
	/*OK*/ pure inline bool onscreen(sint x,sint y);
	            inline bool collision(const fvector& x,const fvector& a,const fixed r);
	                   sint angle(const char** m,const fvector& p,const fvector& t,fixed r);
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

bool game::collision(const fvector& x,const fvector& a,const fixed r)
{
	return (a.x>x.x-r)&&(a.x<x.x+r)&&(a.y>x.y-r)&&(a.y<x.y+r); 	//box works
	//const fvector t(a-x);						//component-wise?
	//return t.dot(t)<r*r;						//circle doesn't
}

sint game::angle(const char** m,const fvector& p,const fvector& t,fixed r) //TODO actual angle computation
{
	const uint l0 = fx::f2l(p.x+r)/BWIDTH;
	const uint r0 = fx::f2l(p.x-r)/BWIDTH;
	const uint u0 = fx::f2l(p.y+r)/BWIDTH;
	const uint d0 = fx::f2l(p.y-r)/BWIDTH;

	const uint l1 = fx::f2l(t.x+r)/BWIDTH;
	const uint r1 = fx::f2l(t.x-r)/BWIDTH;
	const uint u1 = fx::f2l(t.y+r)/BWIDTH;
	const uint d1 = fx::f2l(t.y-r)/BWIDTH;
//REWORK!!! map members are corners!!!
	return	math::abs(m[l0][u0]-m[l1][u1])<=MAXSTEP &&
		math::abs(m[r0][u0]-m[r1][u1])<=MAXSTEP &&
		math::abs(m[l0][d0]-m[l1][d1])<=MAXSTEP &&
		math::abs(m[r0][d0]-m[r1][d1])<=MAXSTEP;
}
///</code>

