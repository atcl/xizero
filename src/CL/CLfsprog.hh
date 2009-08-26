//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
#ifndef HH_CLFSPROG
#define HH_CLFSPROG
//#pragma message "Compiling " __FILE__ " ! TODO: "

#include "CLtypes.hh"
#include "CLglobal.hh"


namespace CLfsprogress
{
	xlong pprogress = 0;
	xlong rprogress = 0;
	uxlong pcolor = 0x000000FF;
	
	void draw();
	void set(xlong p);
	void add(xlong a);
	xlong get();
};

void CLfsprogress::draw()
{
	if(pprogress!=0)
	{
		xlong offset = 0;
		xlong diff = -rprogress + xres;
		
		for(uxlong i=0; i<yres; i++)
		{
			for(uxlong j=0; j<rprogress; j++,offset++)
			{
				(*CLdoublebuffer)[offset] = (*CLdoublebuffer)[offset] & pcolor;
			}
			offset += diff;
		}
	}
}

void CLfsprogress::set(xlong p)
{
	if(p>100) { pprogress = 100; rprogress = xres; }
	else if(p<=0) { rprogress = 0; pprogress = 0; }
	else { pprogress = p; rprogress = (xres / 100) * pprogress; }
}

void CLfsprogress::add(xlong a)
{
	if( (pprogress+a)>100) { pprogress = 100; rprogress = xres; }
	else if( (pprogress+a)<=0) { rprogress = 0; pprogress = 0; }
	else { pprogress += a; rprogress = (xres / 100) * pprogress; }
}

xlong CLfsprogress::get()
{
	return pprogress;
}

#endif
