// atCROSSLEVEL 2010,2011,2012
// licensed under zlib-license
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
///*

///definitions
namespace compiled
{
	/*OK*/ void ammo(const sint x,const sint y,const uint r,const uint c);
	void tracks(uint x,uint y,uint d);
}
///*

///implementation
void compiled::ammo(const sint x,const sint y,const uint r,const uint c)
{
	guard(y<=0||x<=0);
	uint o = ((y-4)*XRES)+(x-3);

	screen::back[o++] = r;
	screen::back[o++] = r;
	screen::back[o++] = r;
	screen::back[o]   = r;
	o += XRES-4;
	screen::back[o++] = r;
	screen::back[o++] = c;
	screen::back[o++] = c;
	screen::back[o++] = c;
	screen::back[o++] = c;
	screen::back[o]   = r;
	o += XRES-5;
	screen::back[o++] = r;
	screen::back[o++] = c;
	screen::back[o++] = c;
	screen::back[o++] = c;
	screen::back[o++] = c;
	screen::back[o]   = r;
	o += XRES-5;
	screen::back[o++] = r;
	screen::back[o++] = c;
	screen::back[o++] = c;
	screen::back[o++] = c;
	screen::back[o++] = c;
	screen::back[o]   = r;
	o += XRES-5;
	screen::back[o++] = r;
	screen::back[o++] = c;
	screen::back[o++] = c;
	screen::back[o++] = c;
	screen::back[o++] = c;
	screen::back[o]   = r;
	o += XRES-4;
	screen::back[o++] = r;
	screen::back[o++] = r;
	screen::back[o++] = r;
	screen::back[o]   = r;
}

void compiled::tracks(uint x,uint y,uint d)
{

}
///*

#endif

