// atCROSSLEVEL 2010,2011,2012
// licensed under zlib-license
// XZtrans.hh
// Screen Transition Library 

///guard
#ifndef HH_XZTRANS
#define HH_XZTRANS
///*

///includes
#include "XZmath.hh"
#include "XZscreen.hh"
#include "XZgfx.hh"
#include "XZsystem.hh"
///*

///definitions
namespace trans
{
	void circleblend(long x,long y,long r);
	void dissolve();
	void fadeout();
}
///*

///implementation
void trans::circleblend(long x,long y,long r) 
{
	for(ulong i=0;i<YRES;++i)
	{
		const long q  = (i - y)*(i - y);

		for(ulong j=0;j<XRES;++j)
		{
			screen::back[i*XRES+j] = math::set(screen::back[i*XRES+j],BLACK,math::sqr(q)<=r);	
		}
	}


	system::slp(3000);
}

void trans::dissolve()
{
	for(ulong i=0;i<256000;++i)
	{
		gfx::pix(math::rnd(XRES),math::rnd(YRES),math::rnd(-1)); 
		system::slp(8);
	}	
}

void trans::fadeout()
{
	packed c = { 0 };

	for(ulong i=0;i<256;++i)
	{
		for(ulong j=0;j<XRES*YRES;++j)
		{
			c.d = screen::back[j];
			//c.b[0] = math::max(0,c.b[0]--);
			c.b[1] = math::max(0,c.b[1]--);
			c.b[2] = math::max(0,c.b[2]--);
			c.b[3] = math::max(0,c.b[3]--);
			screen::back[j] = c.d;
		}
		system::slp(5);
	}
}
///*

#endif

