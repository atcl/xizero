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

///definitions
class progress : public gui
{
	private:
		long _start;
		long _end;
		long _prog;
		bool _horver;		// 0:horizontal 1:vertical
		const long _coeff;
	public:
		progress(long s,long e,bool d,long l,long t,long w,long h,long c,long b,long f,bool v);
		~progress() { ; }
		INLINE void set(long x)   { _prog = math::min(math::max(x,0),_end); }
		INLINE void add(long x)   { _prog = math::min(math::max(_prog+x,0),_end); }
		       void draw() const;
};
///*

///implementation
progress::progress(long s,long e,bool d,long l,long t,long w,long h,long c,long b,long f,bool v) :
	gui(l,t,w,h,c,b,f,v),
	_start(s),
	_end(e),
	_prog(0),
	_horver(d),
	_coeff(math::set(((w-4)<<16)/(e-s),((h-4)<<16)/(e-s),d==0))	
{ ; }

void progress::draw() const
{
	guard(_visible==0);
	const long p = (_prog * _coeff)>>16;
	gfx::rect(_left,_top,_left+_width,_top+_height,_backcolor,_backcolor,1,0);
	gfx::rect(_left+2,_top+2+math::set(_height-4-p,_horver==1),_left+2+math::set(p,_width-4,_horver==0),_top+_height-2,_color,_color,1); 
}
///*

#endif

