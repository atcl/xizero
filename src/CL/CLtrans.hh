///license
//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLTRANS
#define HH_CLTRANS
///*

///idp includes
#include "CLtypes.hh"
#include "CLutils.hh"
#include "CLstruct.hh"
///api includes

///
#include "CLbuffer.hh"
#include "CLgfx.hh"
#include "CLwindow.hh"
///*

///header
/* class name:	CLtranstions
 * 
 * description:	some screen transitions
 * 
 * author:	atcl
 * 
 * notes:
 * 
 * version: 0.1
 */
///*

///definitions
class CLtransitions : public CLbase<CLtransitions,1>
{
	friend class CLbase<CLtransitions,1>;
	
	private:
		static CLwindow* clwindow;
		static CLsystem* clsystem;
		static CLmath*   clmath;
		static CLgfx*    clgfx;
		static CLscreen* clscreen;
	protected:
		CLtransitions() { };
		~CLtransitions() { };
		static void circleblend(xlong x,xlong y,xlong r,xlong t);
		static void dissolve();
		static void fadetoblack();
	public:
		void transition(xlong t) const;
};

CLwindow* CLtransitions::clwindow = CLwindow::instance();
CLsystem* CLtransitions::clsystem = CLsystem::instance();
CLmath*   CLtransitions::clmath   = CLmath::instance();
CLgfx*    CLtransitions::clgfx    = CLgfx::instance();
CLscreen* CLtransitions::clscreen = CLscreen::instance();
///*

///implementation
void CLtransitions::circleblend(xlong x,xlong y,xlong r,xlong t) //! critical
{
	clgfx->drawcircle(x,y,r,0x00FFFFFF);
	clgfx->fill(5,30,0,0x00FFFFFF);
	clwindow->draw();
	clwindow->sleep(t/1000);
}

void CLtransitions::dissolve() //! critical
{
	xlong rx = 0;
	xlong ry = 0;
	uxlong c = 0;
	
	for(xlong i=0; i<256; i++)
	{
		for(xlong j=0; j<1000; j++)
		{
			rx = clmath->random(800);
			ry = clmath->random(600);
			c = clmath->random(-1);
			clgfx->drawpixel(rx,ry,c,1);
		}
		clwindow->draw();
		clwindow->sleep(10);
	}
}

void CLtransitions::fadetoblack() //! critical
{
	doubleword comp = { 0 };
	
	for(xlong i=0; i<256; i++)
	{		
		for(xlong j=0; j<(XRES*YRES); j++)
		{
			comp.dd = clscreen->cldoublebuffer[j];
			if(comp.db[0] > 0) comp.db[0]--;
			if(comp.db[1] > 0) comp.db[1]--;
			if(comp.db[2] > 0) comp.db[2]--;
			if(comp.db[3] > 0) comp.db[3]--;
			clscreen->cldoublebuffer[j] = comp.dd;
		}
		clwindow->draw();
		clwindow->sleep(5);
	}
}

void CLtransitions::transition(xlong t) const
{
	switch(t)
	{
		case 0: clwindow->setdisplay(dissolve); break;
		case 1: clwindow->setdisplay(fadetoblack); break;
		case 2: clwindow->setdisplay(dissolve); break;
		default: return; break;
	}
}
///*

#endif
