//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
#ifndef HH_CLFSPROG
#define HH_CLFSPROG
#pragma message "Compiling " __FILE__ " ! TODO: "

#include "CLtypes.hh"
#include "CLglobal.hh"

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

class CLfsprogress : public virtual CLcl, public CLsingle<CLfsprogress>
{
	friend class CLsingle<CLfsprogress>;
	
	private:
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
		xlong get() const;
};

CLfsprogress::CLfsprogress() { pprogress = rprogress = 0; pcolor = 0x00FF0000;} 

void CLfsprogress::draw() const
{
	//draw full screen progress bar
	if(pprogress!=0)
	{
		xlong offset = 0;
		xlong diff = -rprogress + XRES;
		
		for(uxlong i=0; i<YRES; i++)
		{
			for(uxlong j=0; j<rprogress; j++,offset++) { cldoublebuffer[offset] = cldoublebuffer[offset] & pcolor; }
			offset += diff;
		}
	}
	//*
}

void CLfsprogress::set(xlong p)
{
	//set full screen progress bar to given value
	if(p>100) { pprogress = 100; rprogress = XRES; }
	else if(p<=0) { rprogress = 0; pprogress = 0; }
	else { pprogress = p; rprogress = (XRES / 100) * pprogress; }
	//*
}

void CLfsprogress::add(xlong a)
{
	//add given value to progress of full screen progress bar
	if( (pprogress+a)>100) { pprogress = 100; rprogress = XRES; }
	else if( (pprogress+a)<=0) { rprogress = 0; pprogress = 0; }
	else { pprogress += a; rprogress = (XRES / 100) * pprogress; }
	//*
}

void CLfsprogress::reset() { pprogress = rprogress = 0; }

xlong CLfsprogress::get() const { return pprogress; }

#endif
