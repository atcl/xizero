//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
#ifndef HH_CLFSPROG
#define HH_CLFSPROG
#pragma message "Compiling " __FILE__ " ! TODO: "

#include "CLtypes.hh"
#include "CLglobal.hh"

class CLfsprogress : public virtual CLcl, public CLsingle<CLfsprogress>
{
	friend class CLsingle<CLfsprogress>;
	
	private:
		xlong pprogress;
		xlong rprogress;
		uxlong pcolor;
		CLfsprogress();
		~CLfsprogress();
	public:
		void draw();
		void set(xlong p);
		void add(xlong a);
		void reset();
		xlong get();
};

CLfsprogress::CLfsprogress() { pprogress = rprogress = 0; pcolor = 0x000000FF;}

CLfsprogress::~CLfsprogress() { } 

void CLfsprogress::draw()
{
	//draw full screen progress bar
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
	//*
}

void CLfsprogress::set(xlong p)
{
	//set full screen progress bar to given value
	if(p>100) { pprogress = 100; rprogress = xres; }
	else if(p<=0) { rprogress = 0; pprogress = 0; }
	else { pprogress = p; rprogress = (xres / 100) * pprogress; }
	//*
}

void CLfsprogress::add(xlong a)
{
	//add given value to progress of full screen progress bar
	if( (pprogress+a)>100) { pprogress = 100; rprogress = xres; }
	else if( (pprogress+a)<=0) { rprogress = 0; pprogress = 0; }
	else { pprogress += a; rprogress = (xres / 100) * pprogress; }
	//*
}

void CLfsprogress::reset() { pprogress = rprogress = 0; }

xlong CLfsprogress::get() { return pprogress; }

#endif
