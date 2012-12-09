///<header>
// atCROSSLEVEL 2010,2011,2012
// released under 2-clause BSD license
// XZtrans.hh
// Screen Transition Library 
#pragma once
///</header>

///<include>
#include "XZbasic.hh"
#include "XZmath.hh"
#include "XZgfx.hh"
#include "XZsystem.hh"
#include "XZfrmbuf.hh"
///</include>

///<define>
namespace trans
{
	void circleblend(sint x,sint y,sint r);
	void dissolve();
	void fadeout();
}
///</define>

///<code>
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
	for(uint i=0;i<500;++i)
	{
		for(uint j=0;j<800;++j)
		{
			const uint o = math::rnd(((XRES-2)*(YRES-2)));
			screen::frame[o] = math::rnd(0x00FFFFFF);
			screen::frame[o+1] = math::rnd(0x00FFFFFF);
			screen::frame[o+XRES] = math::rnd(0x00FFFFFF);
			screen::frame[o+XRES+1] = math::rnd(0x00FFFFFF);
		}
		screen::sleep(2);
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
///</code>

