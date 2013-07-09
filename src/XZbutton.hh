///<header>
// atCROSSLEVEL 2010,2011,2012,2013
// released under 2-clause BSD license
// XZbutton.hh
// GUI Button Library 
///</header>

///<include>
#pragma once
#include "XZbasic.hh"
#include "XZmath.hh"
#include "XZgui.hh"
#include "XZlist.hh"
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
		bool active;						//Usable
		xint (*action)();					//Onclick Function Pointer
		static list bl;						//List of all Buttons
		button(const button& b);				//Copy Constructor (not implemented to deny copy)
		button& operator=(const button& b);			//Assignment (not implemented to deny copy)
	public:
		button(const char* x,xint (*a)(),bool d,xint l,xint t,xint w,xint h,xint c,xint b,xint f,bool v); //Constructor
		~button();						//Destructor
		void draw() const;					//Draw Button	
		static xint check(uint m);				//Check If Button Clicked
		static void all(xint a,xint v);				//Activate/Show or Deactivate/Hide All Buttons
		static inline bool inside(const button& b,xint x,xint y);//Check if inside

		static button* current;					//Currently Highlighted Button 
};
///</define>

///<code>
list    button::bl      = list();
button* button::current = 0;

button::button(const char* x,xint (*a)(),bool d,xint l,xint t,xint w,xint h,xint c,xint b,xint f,bool v) :
	gui(l,t,w,h,c,b,f,v),
	text(string::copy(x)),
	textleft( (math::max(w-font::width(x),0))>>1 ),
	texttop( (math::max(h-font::height(x),0))>>1 ),
	depth(d),
	active(1),
	action(a)
{
	bl.append(this);
}

button::~button()
{
	bl.find(this);
	bl.delcurrent();
	delete text;
}

void button::draw() const
{
	guard(visible==0);
	gfx::rect(left,top,left+width,top+height,math::set(RED,framecolor,current),backcolor,1,depth);
	font::draw(left+textleft,top+texttop,text,color,backcolor);
}

xint button::check(uint m)
{
	guard(!MOUSEB(m),0);
	for(bl.first();bl.notlast();bl.next())
	{
		const button& temp = *(button*)bl.current();
		if( (temp.visible!=0) && (temp.active!=0) && inside(temp,MOUSEX(m),MOUSEY(m)) ) 
		{
			return temp.action();
		}
	}
	return 0;
}

void button::all(xint a,xint v)
{
	for(bl.first();bl.notlast();bl.next())
	{
		((button*)bl.current())->active = math::set(a,((button*)bl.current())->active,a>=0);
		((button*)bl.current())->visible = math::set(v,((button*)bl.current())->visible,v>=0);
	}
}

bool button::inside(const button& b,xint x,xint y)
{
	return (x>b.left) && (y>b.top) && (x<b.left+b.width) && (y<b.top+b.height);
}
///</code>

