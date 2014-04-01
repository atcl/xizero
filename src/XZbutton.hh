///<header>
// atCROSSLEVEL 2010-2014
// released under 2-clause BSD license
// XZbutton.hh
// GUI Button Library
#pragma once
///</header>

///<include>
#include "XZbasic.hh"
#include "XZmath.hh"
#include "XZgui.hh"
#include "XZgfx.hh"
#include "XZfont.hh"
#include "XZstring.hh"
///</include>

///<define>
class button : public gui
{
	private:
		const char* text;					// button caption
		const xint textleft;					// relative Y coordinate of caption	
		const xint texttop;					// relative X coordinate of caption
		const bool depth;					// flat or relief
		xint (*action)();					// onclick function pointer
		button(const button& b);				// copy constructor (not implemented to deny copy)
		button& operator=(const button& b);			// assignment (not implemented to deny copy)
	public:
		button(const char* x,xint (*a)(),bool d,xint l,xint t,xint w,xint h,xint c,xint m,xint b,xint f,bool v); // constructor
		//~button() { delete text; };				// destructor
		void draw(bool h=0) const;				// draw button	
		xint inline click() { return action(); };		// trigger action
};
///</define>

///<code>
button::button(const char* x,xint (*a)(),bool d,xint l,xint t,xint w,xint h,xint c,xint m,xint b,xint f,bool v) :
	gui(l,t,w,h,c,m,b,f,v),
	text(string::copy(x)),
	textleft( (math::max(w-font::width(x),0))/2 ),
	texttop( (math::max(h-font::height(x),0))/2 ),
	depth(d),
	action(a)
{ }

void button::draw(bool h) const
{
	guard(visible==0);
	gfx::rect(left,top,left+width,top+height,framecolor,backcolor,1,depth);
	font::draw(left+textleft,top+texttop,text,math::set(alertcolor,color,h),backcolor);
}
///</code>

