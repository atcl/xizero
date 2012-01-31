// atCROSSLEVEL 2010,2011,2012
// licensed under zlib-license
// XZcompiled.hh
// Compiled Sprites Library 

///guard
#ifndef HH_XZCOMPILED
#define HH_XZCOMPILED
///*

///includes
#include "XZscreen.hh"
///*

///definitions
namespace compiled
{
	/*OK*/ void ammo(const ulong x,const ulong y,const ulong r,const ulong c);
	void tracks(ulong x,ulong y,ulong d);
}
///*

///implementation
void compiled::ammo(const ulong x,const ulong y,const ulong r,const ulong c)
{
	ulong o = ((y-4)*XRES)+(x-3);

	screen::back[o++] = screen::back[o++] = screen::back[o++] = screen::back[o] = r;
	o += XRES-4;
	screen::back[o++] = r;
	screen::back[o++] = screen::back[o++] = screen::back[o++] = screen::back[o++] = c;
	screen::back[o]   = r;
	o += XRES-5;
	screen::back[o++] = r;
	screen::back[o++] = screen::back[o++] = screen::back[o++] = screen::back[o++] = c;
	screen::back[o]   = r;
	o += XRES-5;
	screen::back[o++] = r;
	screen::back[o++] = screen::back[o++] = screen::back[o++] = screen::back[o++] = c;
	screen::back[o]   = r;
	o += XRES-5;
	screen::back[o++] = r;
	screen::back[o++] = screen::back[o++] = screen::back[o++] = screen::back[o++] = c;
	screen::back[o]   = r;
	o += XRES-4;
	screen::back[o++] = screen::back[o++] = screen::back[o++] = screen::back[o] = r;
}

void compiled::tracks(ulong x,ulong y,ulong d)
{

}
///*

#endif

