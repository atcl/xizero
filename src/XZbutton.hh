///<header>
// atCROSSLEVEL 2010,2011,2012
// released under zlib/libpng license
// XZbutton.hh
// GUI Button Library 
#pragma once
//#pragma message "Compiling " __FILE__ "..." " TODO: ."
///</header>

///<include>
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
		char* text;						//Button Caption
		sint (*action)();					//Onclick Function Pointer
		bool depth;						//Flat or Relief
		bool active;						//Usable
		const sint textleft;					//Relative Y Coordinate of Caption	
		const sint texttop;					//Relative X Coordinate of Caption
		static list bl;						//List of all Buttons
		button(const button& b);
		button& operator=(const button& b);
	public:
		button(const char* x,sint (*a)(),bool d,sint l,sint t,sint w,sint h,sint c,sint b,sint f,bool v); //Constructor
		~button();						//Destructor
		void draw() const;					//Draw Button
		static sint check(uint m);				//Check If Button Clicked
		static void all(bool b);				//Activate or Deactivate All Buttons
		static inline bool inside(button* b,sint x,sint y);	//Check if inside

		static button* current;					//Currently Highlighted Button 
};
///</define>

///<code>
list    button::bl      = list();
button* button::current = 0;

button::button(const char* x,sint (*a)(),bool d,sint l,sint t,sint w,sint h,sint c,sint b,sint f,bool v) :
	gui(l,t,w,h,c,b,f,v),
	text(string::copy(x)),
	action(a),
	depth(d),
	active(1),
	textleft( (math::max(w-font::width(x),0))>>1 ),
	texttop( (math::max(h-font::height(x),0))>>1 )
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
	gfx::rect(left,top,left+width,top+height,framecolor,backcolor,1,depth);
	font::draw(left+textleft,top+texttop,text,color,backcolor);
}

sint button::check(uint m)
{
	guard(!MOUSEB(m),0);
	for(bl.first();bl.notlast();bl.next())
	{
		button* temp = (button*)bl.current();
		if( (temp->visible!=0) && (temp->active!=0) && inside(temp,MOUSEX(m),MOUSEY(m)) ) 
		{
			return temp->action();
		}
	}
	return 0;
}

void button::all(bool b)
{
	for(bl.first();bl.notlast();bl.next())
	{
		((button*)bl.current())->active = b;
	}
}

bool button::inside(button* b,sint x,sint y)
{
	return (x>b->left) && (y>b->top) && (x<b->left+b->width) && (y<b->top+b->height);
}
///</code>

