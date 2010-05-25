///license
//atCROSSLEVEL studios 2009,2010
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
/* class name:	CLfsprog
 * 
 * description:	A full screen half transparent progress bar.
 * 
 * author:		atcl
 * 
 * notes:		...
 * 
 * version: 	0.2
 */
///*

///definitions
class CLfsprog : public CLbase<CLfsprog,1>
{
	friend class CLbase<CLfsprog,1>;
	friend class CLglobal;
	
	private:
		static CLscreen& clscreen;
	protected:
		xlong pprogress; //0-100 (%)
		xlong rprogress;
		uxlong pcolor;
		CLfsprog();
		~CLfsprog() { };
	public:
		void draw() const;
		void set(xlong p);
		void add(xlong a);
		void reset();
		xlong get() const { return pprogress; };
};

CLscreen& CLfsprog::clscreen = CLscreen::instance();
///*

///implementation
CLfsprog::CLfsprog() { pprogress = rprogress = 0; pcolor = 0x00FF0000;} //! noncritical

void CLfsprog::draw() const //! critical
{
	//draw full screen progress bar
	if(pprogress!=0)
	{
		xlong offset = 0;
		xlong diff = -rprogress + XRES;
		
		for(xlong i=0; i<YRES; i++,offset+=diff)
		{
			for(xlong j=0; j<rprogress; j++,offset++) { clscreen.cldoublebuffer[offset] = clscreen.cldoublebuffer[offset] & pcolor; }
		}
	}
	//*
}

void CLfsprog::set(xlong p) //! noncritical
{
	//set full screen progress bar to given value
	if(p>100) { pprogress = 100; rprogress = XRES; }
	else if(p<=0) { rprogress = 0; pprogress = 0; }
	else { pprogress = p; rprogress = (XRES / 100) * pprogress; }
	//*
}

void CLfsprog::add(xlong a) //! noncritical
{
	//add given value to progress of full screen progress bar
	if( (pprogress+a)>100) { pprogress = 100; rprogress = XRES; }
	else if( (pprogress+a)<=0) { rprogress = 0; pprogress = 0; }
	else { pprogress += a; rprogress = (XRES / 100) * pprogress; }
	//*
}

void CLfsprog::reset() { pprogress = rprogress = 0; } //! noncritical
///*

#endif
