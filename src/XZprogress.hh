///<header>
// atCROSSLEVEL 2010,2011,2012,2013
// released under 2-clause BSD license
// XZprogress.hh
// GUI Progressbar Library 
#pragma once
///</header>

///<include>
#include "XZbasic.hh"
#include "XZgui.hh"
#include "XZgfx.hh"
#include "XZmath.hh"
///</include>

///<declare>
#define HOR 0
#define VER 1
///</declare>

///<define>
class progress : public gui
{
	private:
		const sint coeff;
		sint start;
		sint end;
		sint prog;
		sint area;
		bool horver;

	public:
		progress(sint s,sint e,bool d,sint l,sint t,sint w,sint h,sint c,sint b,sint f,bool v);
		inline void set(long x) { prog = math::lim(0,x,end);      area = (prog*coeff)>>16; }
		inline void add(long x) { prog = math::lim(0,prog+x,end); area = (prog*coeff)>>16; }
		       void draw() const;
};
///</define>

///<code>
progress::progress(sint s,sint e,bool d,sint l,sint t,sint w,sint h,sint c,sint b,sint f,bool v) :
	gui(l,t,w,h,c,b,f,v),
	coeff(math::set(math::set(((w-4)<<16)/(e-s+(e==s)),((h-4)<<16)/(e-s+(e==s)),d==0),e!=s)),
	start(s),
	end(e),
	prog(0),
	area(0),
	horver(d)	
{ ; }

void progress::draw() const
{
	guard(visible==0);
	
	const uint h0 = math::set(height-2-area,2,horver);
	const uint v0 = math::set(area+2,width-2,!horver);
	const uint h1 = math::set(-area-1,horver);
	const uint v1 = math::set(area,!horver);

	gfx::line(left,top,left+width,top,backcolor);
	gfx::line(left,top+1,left+width,top+1,backcolor);
	gfx::line(left,top+height-1,left+width,top+height-1,backcolor);
	gfx::line(left,top+height,left+width,top+height,backcolor);

	gfx::line(left,top+2,left,top+height-2,backcolor);
	gfx::line(left+1,top+2,left+1,top+height-2,backcolor);
	gfx::line(left+width-1,top+2,left+width-1,top+height-2,backcolor);
	gfx::line(left+width,top+2,left+width,top+height-2,backcolor);

	gfx::rect(left+2,top+h0,left+v0,top+height-2,color,color,1,0);
	gfx::rect(left+2+v1,top+2,left+width-2,top+height-2+h1,backcolor,backcolor,1,0);
}
///</code>

