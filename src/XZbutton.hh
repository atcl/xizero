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
#include "XZmath.hh"
#include "XZgui.hh"
#include "XZlist.hh"
#include "XZgfx.hh"
#include "XZfont.hh"
#include "XZstring.hh"
#include "XZgame.hh"
///*

///definitions
class button : public gui
{
	private:
		char* _text;						//Button Caption
		long (*_action)();					//Onclick Function Pointer
		bool _depth;						//Flat or Relief
		const long _textleft;					//Relative Y Coordinate of Caption	
		const long _texttop;					//Relative X Coordinate of Caption
		static list _bl;					//List of all Buttons
	public:
		button(const char* x,long (*a)(),bool d,long l,long t,long w,long h,long c,long b,long f,bool v); //Constructor
		~button();						//Destructor
		INLINE void action(long (*a)()) { _action = a; }	//Set Action
		       void draw() const;				//Draw Button
		static long check(long x,long y,long b);		//Check If Button Clicked
};
///*

///implementation
list button::_bl = list();

button::button(const char* x,long (*a)(),bool d,long l,long t,long w,long h,long c,long b,long f,bool v) :
	gui(l,t,w,h,c,b,f,v),
	_text(string::copy(x)),
	_action(a),
	_depth(d),
	_textleft( (math::max(w-font::width(x,SYSFONT),0))>>1 ),
	_texttop( (math::max(h-font::height(x,SYSFONT),0))>>1 )
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
	const long e = math::neg(_bl.length(),b==0);
	for(long i=_bl.first();i<e;i+=_bl.next())
	{
		const button* temp = (button*)_bl.current();
		if(temp->_visible!=0 && game::inside(x,y,temp->_left,temp->_top,temp->_left+temp->_width,temp->_top+temp->_height)) 
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

