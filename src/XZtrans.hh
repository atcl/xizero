// atCROSSLEVEL 2010,2011,2012
// licensed under zlib-license
// XZtrans.hh
// Screen Transition Library 

///guard
#ifndef HH_XZTRANS
#define HH_XZTRANS
//#pragma message "Compiling " __FILE__ "..." " TODO: ."
///*

///includes
#include "XZbasic.hh"
#include "XZmath.hh"
#include "XZscreen.hh"
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
void trans::circleblend(sint x,sint y,sint r) //fix
{
	for(uint i=0;i<YRES;++i)
	{
		const sint q  = (i - y)*(i - y);

		for(uint j=0;j<XRES;++j)
		{
			if(math::sqr(q)<=r) { screen::back[i*XRES+j] = BLACK; }
		}
	}

	system::slp(3000);
}

void trans::dissolve()
{
	for(uint i=0;i<256000;++i)
	{
		gfx::pix(math::rnd(XRES),math::rnd(YRES),math::rnd(-1)); 
		system::slp(8);
	}	
}

void trans::fadeout()
{
	for(uint i=0;i<256;++i)
	{
		for(uint j=0;j<XRES*YRES;++j)
		{
			packed c = { screen::back[j] };
			//c.b[0] = math::max(0,c.b[0]--); //no need to fade alpha byte
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

