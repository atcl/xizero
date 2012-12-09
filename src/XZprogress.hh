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
		sint start;
		sint end;
		sint prog;
		bool horver;
		const sint coeff;
	public:
		progress(sint s,sint e,bool d,sint l,sint t,sint w,sint h,sint c,sint b,sint f,bool v);
		inline void set(long x) { prog = math::lim(0,x,end); }
		inline void add(long x) { prog = math::lim(0,prog+x,end); }
		       void draw() const;
};
///</define>

///<code>
progress::progress(sint s,sint e,bool d,sint l,sint t,sint w,sint h,sint c,sint b,sint f,bool v) :
	gui(l,t,w,h,c,b,f,v),
	start(s),
	end(e),
	prog(0),
	horver(d),
	coeff(math::set(math::set(((w-4)<<16)/(e-s+(e==s)),((h-4)<<16)/(e-s+(e==s)),d==0),e!=s))	
{ ; }

void progress::draw() const
{
	guard(visible==0);
	const sint p = (prog * coeff)>>16;
	gfx::rect(left,top,left+width,top+height,backcolor,backcolor,1,0);
	gfx::rect(left+2,top+2+math::set(height-4-p,horver==1),left+2+math::set(p,width-4,horver==0),top+height-2,color,color,1); 
}
///</code>

