///<header>
// Ξ0 - xizero ( Version 0.2 ) 
// atCROSSLEVEL 2010-2014 ( http://atcrosslevel.de )
// released under 2-clause BSD license
// GUI Progressbar Library ( XZprogress.hh )
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

		xint prog;
		xint area;
		const xint start;
		const xint end;
		const xint coeff;
		const bool horver;
	public:
		progress(xint s,xint e,bool d,xint l,xint t,xint w,xint h,xint c,xint m,xint b,xint f,bool v);
		inline void set(xint x) { prog = math::lim(0,x,end);      area = (prog*coeff)>>16; }
		inline void add(xint x) { prog = math::lim(0,prog+x,end); area = (prog*coeff)>>16; }
		       void draw() const;
};
///</define>

///<code>
progress::progress(xint s,xint e,bool d,xint l,xint t,xint w,xint h,xint c,xint m,xint b,xint f,bool v) :
	gui(l,t,w,h,c,m,b,f,v),
	prog(0),
	area(0),
	start(s),
	end(e),
	coeff(math::set(math::set(((w-4)<<16)/(e-s+(e==s)),((h-4)<<16)/(e-s+(e==s)),d==0),e!=s)),
	horver(d)	
{ ; }

void progress::draw() const
{
	guard(visible==0);
	
	const yint h0 = math::set(height-2-area,2,horver);
	const yint v0 = math::set(area+2,width-2,!horver);
	const yint h1 = math::set(-area-1,horver);
	const yint v1 = math::set(area,!horver);

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

