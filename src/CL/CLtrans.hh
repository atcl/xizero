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
	public:
		void circleblend(xlong x,xlong y,xlong r,xlong t) const;
		void dissolve() const;
		void fadetoblack() const;
};

CLwindow* CLtransitions::clwindow = CLwindow::instance();
CLsystem* CLtransitions::clsystem = CLsystem::instance();
CLmath*   CLtransitions::clmath   = CLmath::instance();
CLgfx*    CLtransitions::clgfx    = CLgfx::instance();
CLscreen* CLtransitions::clscreen = CLscreen::instance();
///*

///implementation
void CLtransitions::circleblend(xlong x,xlong y,xlong r,xlong t) const //! critical
{
	bool secondtime = 0;
	
	while(clwindow->run())
	{
		if(secondtime) { clwindow->sleep(t); break; }
		clgfx->drawcircle(x,y,r,0x00FFFFFF);
		clgfx->fill(5,30,0,0x00FFFFFF);
		secondtime = 1;
	}
}

void CLtransitions::dissolve() const //! critical
{
	xlong rx = 0;
	xlong ry = 0;
	uxlong c = 0;
	uxlong i = 0;
	
	while(clwindow->run() && i<256)
	{
		for(xlong j=0; j<1000; j++)
		{
			rx = clmath->random(800);
			ry = clmath->random(600);
			c = clmath->random(-1);
			clgfx->drawpixel(rx,ry,c,1);
		}
		i++;
		clwindow->sleep(10);
	}
}

void CLtransitions::fadetoblack() const //! critical
{
	doubleword comp = { 0 };
	uxlong i = 0;
	
	while(clwindow->run() && i<256)
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
		i++;
		clwindow->sleep(5);
	}
}
///*

#endif
