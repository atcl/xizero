// atCROSSLEVEL 2010,2011,2012
// licensed under zlib-license
// XZgui.hh
// GUI base class 

///guard
#ifndef HH_XZGUI
#define HH_XZGUI
///*

///definitions
class gui
{
	protected:
		long _left;
		long _top;
		long _width;
		long _height;
		long _color;
		long _backcolor;
		long _framecolor;
		bool _visible;
	public:
		gui(long l,long t,long w,long h,long c=0x00000000,long b=0x00c0c0c0,long f=0x00000000,bool v=1) : _left(l),_top(t),_width(w),_height(h),_color(c),_backcolor(b),_framecolor(f),_visible(v) { ; } 
		virtual ~gui() { ; } //comment out for release
		virtual void draw() const = 0; //comment out for release
		INLINE void left(long x)       { _left = x; } //needed?
		INLINE void top(long x)        { _top = x; } //needed?
		INLINE void width(long x)      { _width = x; } //needed?
		INLINE void height(long x)     { _height = x; } //needed?
		INLINE void color(long x)      { _color = x; }
		INLINE void backcolor(long x)  { _backcolor = x; }
		INLINE void framecolor(long x) { _framecolor = x; }
		INLINE void visible(bool x)    { _visible = x; }
		INLINE bool visible() const    { return _visible; }
};
///*

#endif

