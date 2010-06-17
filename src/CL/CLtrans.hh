///license
//atCROSSLEVEL studios 2009,2010
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
/* class name:	CLtrans
 * 
 * description:	some screen transitions
 * 
 * author:		atcl
 * 
 * notes:		...
 * 
 * version:		0.2
 */
///*

///definitions
class CLtrans : public CLbase<CLtrans,1>
{
	friend class CLbase<CLtrans,1>;
	friend class CLglobal;
	
	private:
		CLwindow& clwindow;
		CLmath&   clmath;
		CLgfx&    clgfx;
		CLscreen& clscreen;
	protected:
		CLtrans();
		~CLtrans() { };
	public:
		void circleblend(xlong x,xlong y,xlong r) const;
		void dissolve() const;
		void fadetoblack() const;
};
///*

///implementation
CLtrans::CLtrans() //! noncritical
: clwindow(CLwindow::instance()), clmath(CLmath::instance()), clgfx(CLgfx::instance()), clscreen(CLscreen::instance())
{ }

void CLtrans::circleblend(xlong x,xlong y,xlong r) const //! critical
{
	sprite mask;
	mask.width = XRES;
	mask.height = YRES;
	mask.size = XRES*YRES;
	mask.data = new uxlong[XRES*YRES];
	
	xlong tx = 0;
	xlong ty = 0;
	xlong le = 0;
	
	for(xlong i=0; i<YRES; i++)
	{
		for(xlong j=0; j<XRES; j++)
		{
			tx = -x+j;
			ty = -y+i;
			le = clmath.sqrt(tx*tx+ty*ty);
			if(le<=r) { mask.data[i*XRES+j] = clscreen.cldoublebuffer[i*XRES+j]; }
		}
	}
	
	clgfx.drawscreen(&mask);
	clwindow.run();
	clwindow.sleep(3000);
}

void CLtrans::dissolve() const //! critical
{
	xlong rx = 0;
	xlong ry = 0;
	uxlong c = 0;
	
	for(xlong i=0; i<256; i++)
	{
		for(xlong j=0; j<1000; j++)
		{
			rx = clmath.random(800);
			ry = clmath.random(600);
			c = clmath.random(-1);
			clgfx.drawpixel(rx,ry,c,1);
		}
		clwindow.run();
		clwindow.sleep(8);
	}
}

void CLtrans::fadetoblack() const //! critical
{
	doubleword comp = { 0 };
	
	for(xlong i=0; i<256; i++)
	{		
		for(xlong j=0; j<clscreen.clpixelsize; j++)
		{
			comp.dd = clscreen.cldoublebuffer[j];
			if(comp.db[0] > 0) comp.db[0]--;
			if(comp.db[1] > 0) comp.db[1]--;
			if(comp.db[2] > 0) comp.db[2]--;
			if(comp.db[3] > 0) comp.db[3]--;
			clscreen.cldoublebuffer[j] = comp.dd;
		}
		clwindow.run();
		clwindow.sleep(5);
	}
}
///*

#endif
