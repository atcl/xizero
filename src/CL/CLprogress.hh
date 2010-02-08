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
///*

///header
/* class name:	CLprogress
 * 
 * description:	A standard gui element progress bar
 * 
 * author:	atcl
 * 
 * notes:	
 * 
 * version: 0.1
 */
///*

///definitions
class CLprogress : public CLguibase
{
	private:
		static CLstring* clstring;
		static CLgfx*    clgfx;
		static CLmath*   clmath;
	protected:
		float progress;
		xlong start;
		xlong end;
		xlong pprogress;
		float punit;
		uxlong pcolor;
		bool  horver;
	public:
		CLprogress(xlong px,xlong py,xlong w,xlong h,xlong p,xlong s,xlong e,uxlong pc,bool hv,bool f,uxlong fc,uxlong bc,uxlong rc);
		CLprogress(xlong px,xlong py,xlong w,xlong h,xlong p,xlong s,xlong e,uxlong pc,bool hv);
		CLprogress();
		~CLprogress() { };
		void reset(xlong px,xlong py,xlong w,xlong h,xlong p,xlong s,xlong e,uxlong pc,bool hv,xlong f,uxlong fc,uxlong bc,uxlong rc);
		void draw() const;
		void setstart(xlong s);
		void setend(xlong e);
		void setprogress(xlong p);
		void addprogress(xlong a);
		xlong getstart() const { return start; };
		xlong getend() const { return end; };
		xlong getprogress() const { return progress; };
};

CLstring* CLprogress::clstring = CLstring::instance();
CLgfx*    CLprogress::clgfx    = CLgfx::instance();
CLmath*   CLprogress::clmath   = CLmath::instance();
///*

///implementation
CLprogress::CLprogress(xlong px,xlong py,xlong w,xlong h,xlong p,xlong s,xlong e,uxlong pc,bool hv,bool f,uxlong fc,uxlong bc,uxlong rc) : CLguibase(px,py,w,h,f,fc,bc,rc) //! noncritical
{
	//set up attributes
	progress = p;
	horver = hv;
	pcolor = pc;
	
	if(e>=s) { start = s; end  = e; }
	else { start = e; end  = s; }
	
	float temp = clmath->absolute(end-start);
	float temp2 = (width*xlong(!horver)) + (height*xlong(horver));
	punit =  temp2 / temp;
	pprogress = punit * progress;
	//*
}

CLprogress::CLprogress(xlong px,xlong py,xlong w,xlong h,xlong p,xlong s,xlong e,uxlong pc,bool hv) : CLguibase(px,py,w,h) //! noncritical
{
	//set up attributes
	progress = p;
	horver = hv;
	pcolor = pc;
	
	if(e>=s) { start = s; end = e; }
	else { start = e; end = s; }
	
	float temp = float(clmath->absolute(end-start));
	float temp2 = float(width*xlong(!horver)) + (height*xlong(horver));
	punit = temp2 / temp;
	pprogress = xlong(punit * progress);
	//*
}

CLprogress::CLprogress() : CLguibase(0,0,0,0) //! noncritical
{
	start = end = progress = pcolor = horver = pprogress = punit = 0;
}

void CLprogress::reset(xlong px,xlong py,xlong w,xlong h,xlong p,xlong s,xlong e,uxlong pc,bool hv,xlong f,uxlong fc,uxlong bc,uxlong rc) //! noncritical
{
	basereset(px,py,w,h,f,fc,bc,rc);
	
	progress = p;
	horver = hv;
	pcolor = pc;
	
	if(e>=s) { start = s; end = e; }
	else { start = e; end = s; }
	
	float temp = float(clmath->absolute(end-start));
	float temp2 = float(width*xlong(!horver)) + (height*xlong(horver));
	punit = temp2 / temp;
	pprogress = xlong(punit * progress);
}

void CLprogress::draw() const //! noncritical
{
	if(visible==0) return;

	//draw enclosing frame
	clgfx->drawguirectangle(posx,posy,posx+width,posy+height,bcolor,rcolor,!flat);
	//*

	switch(horver)
	{
		//draw horizontal progress
		case 0: clgfx->drawrectangle(posx+1,posy+1,posx+pprogress-1,posy+height-1,pcolor,1); break;
		//*
		
		//draw vertical progress
		case 1: clgfx->drawrectangle(posx+1,posy+height-pprogress+1,posx+width-1,posy+height-1,pcolor,1); break;
		//*
	}
}

void CLprogress::setstart(xlong s) //! noncritical
{
	start = s;
	xlong temp = float(clmath->absolute(end-start));
	xlong temp2 = float(width*xlong(!horver)) + (height*xlong(horver));
	punit =  temp2 / temp;
	pprogress = punit * progress;
}

void CLprogress::setend(xlong e) //! noncritical
{
	end = e;
	xlong temp = float(clmath->absolute(end-start));
	xlong temp2 = float(width*xlong(!horver)) + (height*xlong(horver));
	punit =  temp2 / temp;
	pprogress = punit * progress;
}

void CLprogress::setprogress(xlong p) //! noncritical
{
	if(p>end) progress = end;
	else if(p<start) progress = start;
	else progress = p;
	pprogress = punit * progress;
}

void CLprogress::addprogress(xlong a) //! noncritical
{
	if(progress+a>end) progress = end;
	else if(progress+a<start) progress = start;
	else progress += a;
	pprogress = punit * progress;
}
///*

#endif

