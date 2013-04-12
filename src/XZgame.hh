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
	/*OK*/             void compiled(sint x,sint y,uint r,uint c);
	/*OK*/ pure inline bool onscreen(sint x,sint y);
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
///</code>

