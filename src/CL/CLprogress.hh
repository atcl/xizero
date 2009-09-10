//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLPROGRESS
#define HH_CLPROGRESS
#pragma message "Compiling " __FILE__ " ! TODO: constructor"

#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLguibase.hh"
#include "CLgfx1.hh"
#include "CLgfx2.hh"


class CLprogress : public CLguibase
{
	private:
		float progress;
		xlong start;
		xlong end;
		uxlong pcolor;
		bool  horver;
		
		xlong pprogress;
		float punit;
		
	public:
		CLprogress(xlong px,xlong py,xlong w,xlong h,xlong p,xlong s,xlong e,uxlong pc,bool hv,bool f,uxlong fc,uxlong bc,uxlong rc);
		CLprogress(xlong px,xlong py,xlong w,xlong h,xlong p,xlong s,xlong e,uxlong pc,bool hv);
		CLprogress();
		~CLprogress();

		void reset(xlong px,xlong py,xlong w,xlong h,xlong p,xlong s,xlong e,uxlong pc,bool hv,xlong f,uxlong fc,uxlong bc,uxlong rc);
		void draw();
		void setstart(xlong s);
		void setend(xlong e);
		void setprogress(xlong p);
		void addprogress(xlong a);
		xlong getstart();
		xlong getend();
		xlong getprogress();
};

CLprogress::CLprogress(xlong px,xlong py,xlong w,xlong h,xlong p,xlong s,xlong e,uxlong pc,bool hv,bool f,uxlong fc,uxlong bc,uxlong rc) : CLguibase(px,py,w,h,f,fc,bc,rc)
{
	//set up attributes
	progress = p;
	horver = hv;
	pcolor = pc;
	
	if(e>=s) { start = s; end  = e; }
	else { start = e; end  = s; }
	
	float temp = CLmath::absolute(end-start);
	float temp2 = (width*xlong(!horver)) + (height*xlong(horver));
	punit =  temp2 / temp;
	pprogress = punit * progress;
	//*
}

CLprogress::CLprogress(xlong px,xlong py,xlong w,xlong h,xlong p,xlong s,xlong e,uxlong pc,bool hv) : CLguibase(px,py,w,h)
{
	//set up attributes
	progress = p;
	horver = hv;
	pcolor = pc;
	
	if(e>=s) { start = s; end  = e; }
	else { start = e; end  = s; }
	
	xlong temp = float(CLmath::absolute(end-start));
	xlong temp2 = float(width*xlong(!horver)) + (height*xlong(horver));
	punit =  temp2 / temp;
	pprogress = punit * progress;
	//*
}

CLprogress::CLprogress() : CLguibase(0,0,0,0)
{
	start = end = progress = pcolor = horver = pprogress = punit = 0;
}

CLprogress::~CLprogress() { }

void CLprogress::reset(xlong px,xlong py,xlong w,xlong h,xlong p,xlong s,xlong e,uxlong pc,bool hv,xlong f,uxlong fc,uxlong bc,uxlong rc)
{
	basereset(px,py,w,h,f,fc,bc,rc);
	
	progress = p;
	horver = hv;
	pcolor = pc;
	
	if(e>=s) { start = s; end  = e; }
	else { start = e; end  = s; }
	
	xlong temp = float(CLmath::absolute(end-start));
	xlong temp2 = float(width*xlong(!horver)) + (height*xlong(horver));
	punit =  temp2 / temp;
	pprogress = punit * progress;
}

void CLprogress::draw()
{
	//draw enclosing frame
	CLgfx2::drawguirectangle(posx,posy,posx+width,posy+height,bcolor,rcolor,!flat);
	//*

	switch(horver)
	{
		//draw horizontal progress
		case 0:
			CLgfx1::drawfilledrectangle(posx+1,posy+1,posx+pprogress-1,posy+height-1,pcolor);
		break;
		//*
		
		//draw vertical progress
		case 1:
			CLgfx1::drawfilledrectangle(posx+1,posy+height-pprogress+1,posx+width-1,posy+height-1,pcolor);
		break;
		//*
	}
}

void CLprogress::setstart(xlong s)
{
	start = s;

	xlong temp = float(CLmath::absolute(end-start));
	xlong temp2 = float(width*xlong(!horver)) + (height*xlong(horver));
	punit =  temp2 / temp;
	pprogress = punit * progress;
}

void CLprogress::setend(xlong e)
{
	end = e;

	xlong temp = float(CLmath::absolute(end-start));
	xlong temp2 = float(width*xlong(!horver)) + (height*xlong(horver));
	punit =  temp2 / temp;
	pprogress = punit * progress;
}

void CLprogress::setprogress(xlong p)
{
	if(p>end) progress = end;
	else if(p<start) progress = start;
	else progress = p;

	pprogress = punit * progress;
}

void CLprogress::addprogress(xlong a)
{
	if(progress+a>end) progress = end;
	else if(progress+a<start) progress = start;
	else progress += a;

	pprogress = punit * progress;
}

xlong CLprogress::getstart()
{
	return start;
}

xlong CLprogress::getend()
{
	return end;
}

xlong CLprogress::getprogress()
{
	return progress;
}

#endif

