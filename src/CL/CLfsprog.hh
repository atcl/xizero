///license
//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLFSPROG
#define HH_CLFSPROG
///*

///includes
#include "CLtypes.hh"
#include "CLbase.hh"
#include "CLscreen.hh"
///*

///header
/* class name:	CLfsprogress
 * 
 * description:	A full screen half transparent progress bar.
 * 
 * author:	atcl
 * 
 * notes:	test more.
 * 
 * version: 0.1
 */
///*

///definitions
class CLfsprogress : public CLbase<CLfsprogress,1>
{
	friend class CLbase<CLfsprogress,1>;
		
	private:
		static CLscreen* clscreen;
	protected:
		xlong pprogress;
		xlong rprogress;
		uxlong pcolor;
		CLfsprogress();
		~CLfsprogress() { };
	public:
		void draw() const;
		void set(xlong p);
		void add(xlong a);
		void reset();
		xlong get() const { return pprogress; };
};

CLscreen* CLfsprogress::clscreen = CLscreen::instance();
///*

///implementation
CLfsprogress::CLfsprogress() { pprogress = rprogress = 0; pcolor = 0x00FF0000;} //! noncritical

void CLfsprogress::draw() const //! critical
{
	//draw full screen progress bar
	if(pprogress!=0)
	{
		xlong offset = 0;
		xlong diff = -rprogress + XRES;
		
		for(uxlong i=0; i<YRES; i++)
		{
			for(uxlong j=0; j<rprogress; j++,offset++) { clscreen->cldoublebuffer[offset] = clscreen->cldoublebuffer[offset] & pcolor; }
			offset += diff;
		}
	}
	//*
}

void CLfsprogress::set(xlong p) //! noncritical
{
	//set full screen progress bar to given value
	if(p>100) { pprogress = 100; rprogress = XRES; }
	else if(p<=0) { rprogress = 0; pprogress = 0; }
	else { pprogress = p; rprogress = (XRES / 100) * pprogress; }
	//*
}

void CLfsprogress::add(xlong a) //! noncritical
{
	//add given value to progress of full screen progress bar
	if( (pprogress+a)>100) { pprogress = 100; rprogress = XRES; }
	else if( (pprogress+a)<=0) { rprogress = 0; pprogress = 0; }
	else { pprogress += a; rprogress = (XRES / 100) * pprogress; }
	//*
}

void CLfsprogress::reset() { pprogress = rprogress = 0; } //! noncritical
///*

#endif
