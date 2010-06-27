///license
//atCROSSLEVEL studios 2009,2010
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLPROGRESS
#define HH_CLPROGRESS
///*

///includes
#include "CLtypes.hh"
#include "CLbase.hh"
#include "CLguibase.hh"
#include "CLgfx.hh"
#include "CLmath.hh"
///*

///header
/* class name:	CLprogress
 * 
 * description:	A standard gui element progress bar
 * 
 * author:		atcl
 * 
 * notes:		...
 * 
 * version: 	0.2
 */
///*

///definitions
class CLprogress : public CLguibase
{
	private:
		CLgfx&    clgfx;
		CLmath&   clmath;
	protected:
		float progress;
		xlong start;
		xlong end;
		xlong pprogress;
		float punit;
		uxlong pcolor;
		bool  horver;
		inline float cpunit() const;
	public:
		CLprogress(xlong px,xlong py,xlong w,xlong h,xlong p,xlong s,xlong e,uxlong pc,bool hv,bool f,uxlong fc,uxlong bc,uxlong rc);
		~CLprogress() { };
		void draw() const;
		void setwidth(xlong w);
		void setheight(xlong h);
		void setstart(xlong s);
		void setend(xlong e);
		void setprogress(xlong p);
		void addprogress(xlong a);
		xlong getstart() const { return start; };
		xlong getend() const { return end; };
		xlong getprogress() const { return progress; };
};
///*

///implementation
inline float CLprogress::cpunit() const //! noncritical
{
	float t1 = clmath.absolute(end-start);
	float t2 = width*xlong(!horver) + height*xlong(horver);
	return (t2 / t1);
}

CLprogress::CLprogress(xlong px,xlong py,xlong w,xlong h,xlong p,xlong s,xlong e,uxlong pc,bool hv,bool f,uxlong fc,uxlong bc,uxlong rc) //! noncritical
: clgfx(CLgfx::instance()), clmath(CLmath::instance()), CLguibase(px,py,w,h,f,fc,bc,rc), progress(p),horver(hv),pcolor(pc)
{
	//set up attributes
	if(e>=s) { start = s; end  = e; }
	else { start = e; end  = s; }
	
	punit =  cpunit();
	pprogress = punit * progress;
	//*
}

void CLprogress::draw() const //! noncritical
{
	if(visible==0) { return; }

	//draw enclosing frame
	clgfx.drawguirectangle(posx,posy,posx+width,posy+height,bcolor,rcolor,!flat);
	//*

	switch(horver)
	{
		//draw horizontal progress
		case 0: clgfx.drawrectangle(posx+1,posy+1,posx+pprogress-1,posy+height-1,pcolor,1); break;
		//*
		
		//draw vertical progress
		case 1: clgfx.drawrectangle(posx+1,posy+height-pprogress+1,posx+width-1,posy+height-1,pcolor,1); break;
		//*
	}
}

void CLprogress::setwidth(xlong w) //! noncritical
{
	width = w;
	punit =  cpunit();
	pprogress = punit * progress;
}

void CLprogress::setheight(xlong h) //! noncritical
{
	height = h;
	punit =  cpunit();
	pprogress = punit * progress;
}

void CLprogress::setstart(xlong s) //! noncritical
{
	start = s;
	punit =  cpunit();
	pprogress = punit * progress;
}

void CLprogress::setend(xlong e) //! noncritical
{
	end = e;
	punit =  cpunit();
	pprogress = punit * progress;
}

void CLprogress::setprogress(xlong p) //! noncritical
{
	if(p>end) { progress = end; }
	else if(p<start) { progress = start; }
	else { progress = p; }
	pprogress = punit * progress;
}

void CLprogress::addprogress(xlong a) //! noncritical
{
	if(progress+a>end) { progress = end; }
	else if(progress+a<start) { progress = start; }
	else { progress += a; }
	pprogress = punit * progress;
}
///*

#endif

