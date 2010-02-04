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
		void circleblend(xlong x,xlong y,xlong r) const;
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
void CLtransitions::circleblend(xlong x,xlong y,xlong r) const //! critical
{
	for(xlong i=YRES/2; i>r; i--)
	{
		clgfx->drawclipcircle(x,y,i,0x00FFFFFF);
		clgfx->drawclipcircle(x-1,y,i+1,0x00FFFFFF);
		clgfx->drawclipcircle(x+1,y,i+1,0x00FFFFFF);
		clgfx->drawclipcircle(x,y-1,i+1,0x00FFFFFF);
		clgfx->drawclipcircle(x,y+1,i+1,0x00FFFFFF);
		//...
		clwindow->run();
		clwindow->sleep(5);
	}
	clwindow->sleep(1000);
}

void CLtransitions::dissolve() const //! critical
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
		clwindow->run();
		clwindow->sleep(10);
	}
	//clwindow->sleep(1000);
}

void CLtransitions::fadetoblack() const //! critical
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
		clwindow->run();
		clwindow->sleep(5);
	}
	//clwindow->sleep(1000);
}
///*

#endif
