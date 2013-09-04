///<header>
// atCROSSLEVEL 2010,2011,2012,2013
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
	void circleblend(xint x,xint y,xint r);
	void dissolve();
	void fadeout();
}
///</define>

///<code>
void trans::circleblend(xint x,xint y,xint r)
{
	for(yint i=0,o=0;i<YRES;++i)
	{
		const xint p  = (i - y)*(i - y);

		for(yint j=0;j<XRES;++j)
		{
			const xint q  = (j - x)*(j - x);
			screen::frame[o++] = math::set(screen::frame[i*XRES+j],BLACK,math::sqr(p+q)<=r);
		}
	}
	screen::flush();
	screen::sleep(1000);
}

void trans::dissolve()
{
	for(yint i=0;i<YRES;++i)
	{
		for(yint j=0;j<XRES;++j)
		{
			const yint o = math::rnd(((XRES-2)*(YRES-2)));
			screen::frame[o]        = math::rnd(0x00FFFFFF);
			screen::frame[o+1]      = math::rnd(0x00FFFFFF);
			screen::frame[o+XRES]   = math::rnd(0x00FFFFFF);
			screen::frame[o+XRES+1] = math::rnd(0x00FFFFFF);
		}
		screen::flush();
		screen::sleep(2);
	}
}

void trans::fadeout()
{
	for(yint i=0;i<256;++i)
	{
		for(yint j=0;j<XRES*YRES;++j)
		{
			rgba c = { (yint)screen::frame[j] };
			c.b[0] = (byte)math::max(0,c.b[0]-1);
			c.b[1] = (byte)math::max(0,c.b[1]-1);
			c.b[2] = (byte)math::max(0,c.b[2]-1);
			//c.b[3] = (byte)math::max(0,c.b[3]-1); //no need to fade alpha byte
			screen::frame[j] = c.d;
		}
		screen::flush();
		screen::sleep(10);
	}
}
///</code>

