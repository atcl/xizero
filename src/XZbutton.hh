// atCROSSLEVEL 2010,2011,2012
// licensed under zlib-license
// XZbutton.hh
// GUI Button Library 

///guard
#ifndef HH_XZBUTTON
#define HH_XZBUTTON
///*

///includes
#include "XZbasic.hh"
#include "XZutil.hh"
#include "XZgui.hh"
#include "XZlist.hh"
#include "XZgfx.hh"
#include "XZfont.hh"
#include "XZstring.hh"
///*

///definitions
class button : public gui
{
	private:
		char* _text;
		long (*_action)();
		bool _depth;
		long _textleft;
		long _texttop;
		static list _bl;
	public:
		button(char* x,long (*a)(),bool d,long l,long t,long w,long h,long c,long b,long f,bool v);
		~button();

		INLINE void action(long (*a)()) { _action = a; }
		       void draw() const;
		static long check(long x,long y,long b);
};
///*

///implementation
list button::_bl = list();

button::button(char* x,long (*a)(),bool d,long l,long t,long w,long h,long c,long b,long f,bool v) :
	gui(l,t,w,h,c,b,f,v),
	_text(string::copy(x)),
	_action(a),
	_depth(d),
	_textleft((math::max(w-font::width(x,SYSFONT),0))>>1),
	_texttop((math::max(h-font::height(x,SYSFONT),0))>>1)
{
	_bl.append(this);
}

button::~button()
{
	_bl.find(this);
	_bl.delcurrent();
	delete _text;
}

long button::check(long x,long y,long b)
{
	button* temp = 0;
	long e = math::neg(_bl.length(),b==0);
	for(long i=_bl.first();i<e;i+=_bl.next())
	{
		temp = (button*)_bl.current();
		if(temp->_visible!=0 && x>temp->_left && x<temp->_left+temp->_width && y>temp->_top && y<temp->_top+temp->_height)
		{
			return temp->_action();
		}
	}
	return 0;
}

void button::draw() const
{
	guard(_visible==0);
	gfx::rect(_left,_top,_left+_width,_top+_height,_framecolor,_backcolor,1,_depth);
	font::draw(_left+_textleft,_top+_texttop,_text,_color,_backcolor,SYSFONT);
}
///*

#endif

