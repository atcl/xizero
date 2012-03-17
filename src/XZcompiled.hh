// atCROSSLEVEL 2010,2011,2012
// released under zlib/libpng license
// XZcompiled.hh
// Compiled Sprites Library 

///guard
#ifndef HH_XZCOMPILED
#define HH_XZCOMPILED
//#pragma message "Compiling " __FILE__ "..." " TODO: ."
///*

///includes
#include "XZbasic.hh"
#include "XZscreen.hh"
#include "XZgame.hh"
///*

///definitions
namespace compiled
{
	/*OK*/ void ammo(sint x,sint y,uint r,uint c);
	       void tracks(uint x,uint y,uint d,uint c);
}
///*

///implementation
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
	uint o = ((y-4)*XRES)+(x-3);

	switch(d/5)
	{
		case 0: ;

		case 1: ;

		case 2: ;

		case 3: ;

		case 4: ;
	}
}
///*

#endif

