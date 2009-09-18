//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
#ifndef HH_CLFLOOR
#define HH_CLFLOOR
#pragma message "Compiling " __FILE__ " ! TODO: "

#include "CLtypes.hh"
#include "CLstruct.hh"
#include "CLglobal.hh"
#include "CLmath.hh"

class CLfloor : public virtual CLcl, public CLsingle<CLfloor>
{
	friend class CLsingle<CLfloor>;
	
	private:
		CLmath* math;
		uxlong shade;
		xlong  zlevel;
		uxlong xstart;
		uxlong xend;
		uxlong width;
		CLfloor();
		~CLfloor();
	public:
		void init(xlong z,xlong w,uxlong c,bool s);
		void draw();
};

CLfloor::CLfloor() { math = CLmath::instance(); zlevel = zres; xstart = 0; xend = xres-1; width = xres; shade = 0; }

CLfloor::~CLfloor() { }

void CLfloor::init(xlong z,xlong w,uxlong c,bool s)
{
	//set floor z and width
	zlevel = z;
	width = w;
	//*
	
	//set floor start and end
	xstart = (xres-w)>>1;
	xend   = xres-((xres-w)>>1);
	//*
	
	//draw floor with shaded floor color
	if(s)
	{
		//shade floor
		doubleword argb = { 0 };
		float t = math->absolute((clplane * cllight) / ( !clplane * !cllight ));
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
}

void CLfloor::draw()
{
	//fast floor drawing
	uxlong runningfloorxstart = xstart;
	uxlong runningfloorxend = xend;
	uxlong i=0;
	uxlong j=0;
	for(i=0; i<yres; i++)
	{
		for(j=runningfloorxstart; j<runningfloorxend; j++)
		{
			(*CLdoublebuffer)[j] = shade;
			(*CLzbuffer)[j] = zlevel;
		}
		runningfloorxstart += xres;
		runningfloorxend += xres;
	}
	//*
}


#endif
