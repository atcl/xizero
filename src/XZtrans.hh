// atCROSSLEVEL 2010,2011,2012
// released under zlib/libpng license
// XZtrans.hh
// Screen Transition Library 

///guard
#pragma once
//#pragma message "Compiling " __FILE__ "..." " TODO: ."
///*

///includes
#include "XZbasic.hh"
#include "XZmath.hh"
#include "XZskms.hh"
#include "XZgfx.hh"
#include "XZsystem.hh"
///*

///definitions
namespace trans
{
	void circleblend(sint x,sint y,sint r);
	void dissolve();
	void fadeout();
}
///*

///implementation
void trans::circleblend(sint x,sint y,sint r)
{
	for(uint i=0;i<YRES;++i)
	{
		const sint p  = (i - y)*(i - y);

		for(uint j=0;j<XRES;++j)
		{
			const sint q  = (j - x)*(j - x);
			screen::back[i*XRES+j] = math::set(screen::back[i*XRES+j],BLACK,math::sqr(p+q)<=r);
		}
	}
	screen::_flush();
	screen::sleep(1000);
}

void trans::dissolve()
{
	for(uint i=0;i<2000;++i)
	{
		for(uint j=0;j<32;++j)
		{
			const uint o = math::rnd(((XRES-2)*(YRES-2)));
			screen::back[o] = math::rnd(0x00FFFFFF);
			screen::back[o+1] = math::rnd(0x00FFFFFF);
			screen::back[o+XRES] = math::rnd(0x00FFFFFF);
			screen::back[o+XRES+1] = math::rnd(0x00FFFFFF);
		}
		screen::_flush();
	}
}

void trans::fadeout()
{
	for(uint i=0;i<256;++i)
	{
		for(uint j=0;j<XRES*YRES;++j)
		{
			packed c = { (uint)screen::back[j] };
			c.b[0] = (byte)math::max(0,c.b[0]-1);
			c.b[1] = (byte)math::max(0,c.b[1]-1);
			c.b[2] = (byte)math::max(0,c.b[2]-1);
			//c.b[3] = (byte)math::max(0,c.b[3]-1); //no need to fade alpha byte
			screen::back[j] = c.d;
		}
		screen::_flush();
		screen::sleep(10);
	}
}
///*

