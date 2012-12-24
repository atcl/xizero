///<header>
// atCROSSLEVEL 2010,2011,2012
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
		bool horver;		//hor = 0, ver = 1

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

	gfx::rect(left,top,left+width,top+height,backcolor,backcolor,1,0);
	gfx::rect(left+2,top+2+math::set(height-4-area,horver),left+2+math::set(area,width-4,!horver),top+height-2,color,color,1); 
}
///</code>

