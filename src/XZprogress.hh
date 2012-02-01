// atCROSSLEVEL 2010,2011,2012
// licensed under zlib-license
// XZprogress.hh
// GUI Progressbar Library 

///guard
#ifndef HH_XZPROGRESS
#define HH_XZPROGRESS
///*

///includes
#include "XZutil.hh"
#include "XZgui.hh"
#include "XZgfx.hh"
#include "XZmath.hh"
///*

///declarations
#define HOR 0
#define VER 1
///*

///definitions
class progress : public gui
{
	private:
		sint _start;
		sint _end;
		sint _prog;
		bool _horver;
		const sint _coeff;
	public:
		progress(sint s,sint e,bool d,sint l,sint t,sint w,sint h,sint c,sint b,sint f,bool v);
		inline void set(long x) { _prog = math::min(math::max(x,0),_end); }
		inline void add(long x) { _prog = math::min(math::max(_prog+x,0),_end); }
		       void draw() const;
};
///*

///implementation
progress::progress(sint s,sint e,bool d,sint l,sint t,sint w,sint h,sint c,sint b,sint f,bool v) :
	gui(l,t,w,h,c,b,f,v),
	_start(s),
	_end(e),
	_prog(0),
	_horver(d),
	_coeff(math::set(math::set(((w-4)<<16)/(e-s+(e==s)),((h-4)<<16)/(e-s+(e==s)),d==0),e!=s))	
{ ; }

void progress::draw() const
{
	guard(_visible==0);
	const sint p = (_prog * _coeff)>>16;
	gfx::rect(_left,_top,_left+_width,_top+_height,_backcolor,_backcolor,1,0);
	gfx::rect(_left+2,_top+2+math::set(_height-4-p,_horver==1),_left+2+math::set(p,_width-4,_horver==0),_top+_height-2,_color,_color,1); 
}
///*

#endif

