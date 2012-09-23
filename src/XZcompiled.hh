///<header>
// atCROSSLEVEL 2010,2011,2012
// released under zlib/libpng license
// XZcompiled.hh
// Compiled Sprites Library 
#pragma once
//#pragma message "Compiling " __FILE__ "..." " TODO: tracks"
///</header>

///<include>
#include "XZbasic.hh"
#include "XZgame.hh"

#ifdef FRMBUF
#include "XZfrmbuf.hh"
#else
#include "XZskms.hh"
#endif
///</include>

///<define>
namespace compiled
{
	/*OK*/ void ammo(sint x,sint y,uint r,uint c);
	       void tracks(uint x,uint y,uint d,uint c);
}
///</define>

///<code>
void compiled::ammo(sint x,sint y,uint r,uint c)
{
	//guard(game::onscreen(x,y)==1);
	uint o = ((y-4)*XRES)+(x-3);

	screen::back[o++] = r; screen::back[o++] = r; screen::back[o++] = r; screen::back[o] = r;
	o += XRES-4;
	screen::back[o++] = r;
	screen::back[o++] = c; screen::back[o++] = c; screen::back[o++] = c; screen::back[o++] = c;
	screen::back[o]   = r;
	o += XRES-5;
	screen::back[o++] = r;
	screen::back[o++] = c; screen::back[o++] = c; screen::back[o++] = c; screen::back[o++] = c; 
	screen::back[o]   = r;
	o += XRES-5;
	screen::back[o++] = r;
	screen::back[o++] = c; screen::back[o++] = c; screen::back[o++] = c; screen::back[o++] = c;
	screen::back[o]   = r;
	o += XRES-5;
	screen::back[o++] = r;
	screen::back[o++] = c; screen::back[o++] = c; screen::back[o++] = c; screen::back[o++] = c;
	screen::back[o]   = r;
	o += XRES-4;
	screen::back[o++] = r; screen::back[o++] = r; screen::back[o++] = r; screen::back[o] = r;
}

void compiled::tracks(uint x,uint y,uint d,uint c)
{
	//guard(game::onscreen(x,y)==1);
	uint o = ((y-1)*XRES)+(x-1);

	switch(d/5)
	{
		case 0:
			screen::back[o++] = ORANGE; screen::back[o++] = ORANGE; screen::back[o++] = ORANGE;  
			o += XRES-1;
			screen::back[o++] = LRED; screen::back[o++] = LRED;  screen::back[o++] = LRED;
			o += XRES-1;
			screen::back[o++] = ORANGE; screen::back[o++] = ORANGE; screen::back[o++] = ORANGE;
		case 1: ;

		case 2: ;

		case 3: ;

		case 4: ;
	}
}
///</code>

