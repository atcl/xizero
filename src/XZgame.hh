///<header>
// atCROSSLEVEL 2010,2011,2012,2013
// released under 2-clause BSD license
// XZgame.hh
// Game Library 
///</header>

///<include>
#pragma once
#include "XZbasic.hh"
#include "XZfrmbuf.hh"
///</include>

///<define>
namespace game
{
	/*OK*/             void compiled(xint x,xint y,yint r,yint c);
	/*OK*/ pure inline bool onscreen(xint x,xint y);
}
///</define>

///<code>
void game::compiled(xint x,xint y,yint r,yint c)
{
	//guard(game::onscreen(x,y)==1);
	yint o = ((y-2)*XRES)+(x-2);

	screen::frame[o++] = r; screen::frame[o++] = r; screen::frame[o++] = r; screen::frame[o] = r; o += XRES-3;
	screen::frame[o++] = r; screen::frame[o++] = c; screen::frame[o++] = c; screen::frame[o] = r; o += XRES-3;
	screen::frame[o++] = r; screen::frame[o++] = c; screen::frame[o++] = c; screen::frame[o] = r; o += XRES-3;
	screen::frame[o++] = r; screen::frame[o++] = r; screen::frame[o++] = r; screen::frame[o] = r; o += XRES-3;
}

bool game::onscreen(xint x,xint y)
{
	return (x>0) && (y>0) && (x<XRES) && (y<YRES);
}
///</code>

