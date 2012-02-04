// atCROSSLEVEL 2010,2011,2012
// licensed under zlib-license
// XZgui.hh
// GUI base class 

///guard
#ifndef HH_XZGUI
#define HH_XZGUI
//#pragma message "Compiling " __FILE__ "..." " TODO: ."
///*

///includes
#include "XZbasic.hh"
///*

///definitions
class gui
{
	protected:
		sint _left;
		sint _top;
		sint _width;
		sint _height;
		sint _color;
		sint _backcolor;
		sint _framecolor;
		bool _visible;
	public:
		gui(sint l,sint t,sint w,sint h,sint c=0x00000000,sint b=0x00c0c0c0,sint f=0x00000000,bool v=1) : _left(l),_top(t),_width(w),_height(h),_color(c),_backcolor(b),_framecolor(f),_visible(v) { ; } 
		virtual ~gui() { ; } //comment out for release
		virtual void draw() const = 0; //comment out for release
		inline void left(sint x)       { _left = x; }
		inline void top(sint x)        { _top = x; }
		//inline void width(sint x)      { _width = x; }
		//inline void height(sint x)     { _height = x; }
		inline void color(sint x)      { _color = x; }
		inline void backcolor(sint x)  { _backcolor = x; }
		inline void framecolor(sint x) { _framecolor = x; }
		inline void visible(bool x)    { _visible = x; }
		inline bool visible() const    { return _visible; }
};
///*

#endif

