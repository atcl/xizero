///<header>
// atCROSSLEVEL 2010,2011,2012,2013
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
		const char* text;					//Button Caption
		const xint textleft;					//Relative Y Coordinate of Caption	
		const xint texttop;					//Relative X Coordinate of Caption
		const bool depth;					//Flat or Relief
		xint (*action)();					//Onclick Function Pointer
		button(const button& b);				//Copy Constructor (not implemented to deny copy)
		button& operator=(const button& b);			//Assignment (not implemented to deny copy)
	public:
		button(const char* x,xint (*a)(),bool d,xint l,xint t,xint w,xint h,xint c,xint m,xint b,xint f,bool v); //Constructor
		~button() { delete text; };				//Destructor
		void draw(bool h=0) const;				//Draw Button	
		xint inline click() { return action(); };		//Trigger action
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

