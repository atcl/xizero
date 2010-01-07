///license
//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLFLOOR
#define HH_CLFLOOR
///*

///includes
#include "CLtypes.hh"
#include "CLstruct.hh"
#include "CLmath.hh"
#include "CLbuffer.hh"
#include "CLbase.hh"
///*

///header
/* class name:	CLfloor
 * 
 * description:	This class handles the level floor.
 * 
 * author:	atcl
 * 
 * notes:	make faster.
 * 
 * version: 0.1
 */
///*

///definitions
class CLfloor : public CLbase<CLfloor,1>
{
	friend class CLbase<CLfloor,1>;
	
	private:
		static CLmath*   clmath;
		static CLscreen* clscreen;
	protected:
		CLubuffer* cmask;
		CLfbuffer* zmask;
 		uxlong shade;
		xlong  zlevel;
		uxlong xstart;
		uxlong xend;
		uxlong width;
		CLfloor();
		~CLfloor();
	public:
		void init(xlong z,xlong w,uxlong c,bool s);
		void draw() const; //too slow!!!
};

CLmath*   CLfloor::clmath   = CLmath::instance();
CLscreen* CLfloor::clscreen = CLscreen::instance();
///*

///implementations
CLfloor::CLfloor() { zlevel = ZRES; xstart = 0; xend = XRES-1; width = XRES; shade = 0; } //! noncritical

CLfloor::~CLfloor() { delete cmask; delete zmask; }

void CLfloor::init(xlong z,xlong w,uxlong c,bool s) //! noncritical
{
	//set floor z and width
	zlevel = z;
	width = w;
	//*
	
	//set floor start and end
	xstart = (XRES-w)>>1;
	xend   = XRES-((XRES-w)>>1);
	//*
	
	//draw floor with shaded floor color
	if(s)
	{
		//shade floor
		doubleword argb = { 0 };
		float t = clmath->absolute((clplane * cllight) / ( !clplane * !cllight ));
		//if(t > 1) t = 1;
		//if(t < 0.2) s = nolight;
		uxchar zlevellighting = 128 - (z * (128/100));
		argb.dd = c;
		argb.db[0] = uxchar((float(uxchar(argb.db[0])))*t) + zlevellighting;
		argb.db[1] = uxchar((float(uxchar(argb.db[1])))*t) + zlevellighting;
		argb.db[2] = uxchar((float(uxchar(argb.db[2])))*t) + zlevellighting;
		shade = argb.dd;
		//*
	}
	//*
	
	//draw floor with unshaded color
	else shade = c;
	//*
	
	cmask = new CLubuffer((XRES*YRES),0);
	zmask = new CLfbuffer((XRES*YRES),ZRES);
	
	//draw floor mask
	uxlong i=0;
	uxlong j=0;
	for(i=0; i<YRES; i++)
	{
		for(j=xstart; j<xend; j++)
		{
			(*cmask)[j] = shade;
			(*zmask)[j] = zlevel;
		}
		xstart += XRES;
		xend += XRES;
	}
	//*
}

void CLfloor::draw() const //! critical
{
	clscreen->clstencilbuffer.clear(0);
	cmask->copy(clscreen->cldoublebuffer.getbuffer());
	zmask->copy(clscreen->clzbuffer.getbuffer());
}
///*

#endif
